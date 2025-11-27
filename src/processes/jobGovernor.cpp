#include "jobGovernor.h"


JobGovernor::JobGovernor(uint32_t vardas, uint8_t prioritetas,
    ProcessList* kokiamSarasuiPriklauso, Process* tevas,
    Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, prioritetas,
    kokiamSarasuiPriklauso, tevas,
    nuorodaIProcesoriu, branduolys), step(1){
        pi = 0; 
        si = 0; 
        ti = 10; 
        memset(pageTable, 0, sizeof(pageTable));
    }

void JobGovernor::setInterruptRegisters(uint8_t pi, uint8_t si, uint8_t ti){
    this->pi = pi;
    this->si = si;
    this->ti = ti;
}

Busena JobGovernor::execute(){
    switch(step){
        case 1:{
            if(arTuriResursa(ResourceType::vartotojoAtmintis)){
                step = 2;
                return Busena::READY;
            }
            return Busena::BLOCKED;
        }
        case 2:{
            if(arTuriResursa(ResourceType::kanaluIrenginys)){
                step = 3;
                return Busena::READY;
            }
            return Busena::BLOCKED;
        }
        case 3:{
            branduolys->realiMasina->allocateMemoryForVirtualMachine();
            //Išskiriama atmintis virtualiai mašinai ir sukuriama puslapių lentelė 
            
            //VirtualMachineRegisters registers = branduolys->realiMasina->getRealMachineRegisterPointers();
            //virtualMachine = new VirtualMachine(registers, *branduolys->realiMasina); 
            

            uint32_t ptr = branduolys->realiMasina->getPtr();
            for(int i = 0; i < 16; i++){
                pageTable[i] = branduolys->realiMasina->getWordFromMemory(ptr * 16 + i);
            } 
        
            step = 4;
            return Busena::READY;
        }
        case 4:{
            //Nustatinėjami kanalų įrenginio registrai ir įvykdoma komanda XCHG (4)
            ChannelDevice* channelDevice = branduolys->realiMasina->getPointerToChannelDevice();
            channelDevice->setST(2); //is supervizorines
            channelDevice->setDT(1); //i vartotojo
            channelDevice->setRNUM(16);
            for(int i = 0; i < 16; ++i){
                channelDevice->setSB(i); //is kurio takelio kopijuojam
                channelDevice->setDB(pageTable[i]); //i kuri takeli kopijuojam
                channelDevice->xchg();
            }
            step = 5;
            return Busena::READY;
        }
        case 5:{
            //Atlaisvinamas "Kanalų įrenginio" resursas (5)
            ResursuPrimityvai::atlaisvintiResursa(ResourceType::kanaluIrenginys, this);
            step = 6;
            return Busena::READY;
        }
        
        case 6:{
            //Atlaisvinamas resursas "Supervizorinė atmintis" (6)
            ResursuPrimityvai::atlaisvintiResursa(ResourceType::supervizorineAtmintis, this);
            step = 7;
            return Busena::READY;
        }
        case 7:{
            //Atlaisvinamas resursas "Užduotis būgne" (7)
            ResursuPrimityvai::atlaisvintiResursa(ResourceType::uzduotisBugne, this);
            step = 8;
            return Busena::READY;
        }
        case 8:{
            //Proceso virtuali masina kurimas
            ProcesuPrimityvai::kurtiProcesa(this, ProcesuTipai::virtualMachineProcess, nuorodaIProcesoriu, branduolys); 
            
            step = 9;
            return Busena::READY;
        }
        case 9:{
            if(arTuriResursa(ResourceType::isInterrupt)){
                step = 10;
                return Busena::READY;
            }
            return Busena::BLOCKED;
        }
        case 10:{
            //proceso virtuali masina stabdymas
            ProcesuPrimityvai::stabdytiProcesa(this, ProcesuTipai::virtualMachineProcess); //PAPILDYT
            step = 11;
            return Busena::READY;
        }
        case 11:{
            if(pi > 0){
                step = 12;
                return Busena::READY;
            }
            else{
                step = 17;
                return Busena::READY;
            }
        }
        case 12:{
            //atlaisvinti resursa eilute atmintyje su atitinkama pi reiksme 
            ResursuPrimityvai::atlaisvintiResursa(ResourceType::eiluteAtmintyje, this, pi); 

            step = 13;
            return Busena::READY;
        }
        case 13:{
            //naikinam procesa virtuali masina
            ProcesuPrimityvai::naiktintiProcesa();
            step = 14;
            return Busena::READY;
        }
        case 14:{
            //atlaisvinti resursa "vartotojo atmintis"
            ResursuPrimityvai::atlaisvintiResursa(ResourceType::vartotojoAtmintis, this); 
            step = 15;
            return Busena::READY;
        }
        case 15:{
            //Atlaisvinti fiktyvų “Užduotis būgne” resursą (15)
            ResursuPrimityvai::atlaisvintiResursa(ResourceType::uzduotisBugne, this);
            step = 16;
            return Busena::READY;
        }
        case 16:{
            if(arTuriResursa(ResourceType::neegzistuojantis)){
                step = 9;
                return Busena::READY;
            }
            return Busena::BLOCKED;
        }
        case 17:{
            if(si > 0){
                step = 19;
                return Busena::READY;
            }
            else{
                step = 18;
                return Busena::READY;
            }
        }
        case 18:{
            //taimerio pertraukimas
            step = 9;
            return Busena::READY;
        }
        case 19:{
            if(si == 1){ //HALT
                step = 13;
                return Busena::READY;
            }
            else{
                step = 20;
                return Busena::READY;
            }
        }
        case 20:{
            if(si == 5){ //EXEx
                step = 21;
                return Busena::READY;
            }
            else{
                step = 22;
                return Busena::READY;
            }
        }
        case 21:{
            uint32_t vardas = virtualMachine->registrai.rbx; //pasiimt rbx registro reiksme
            uint8_t x = static_cast<uint8_t>(virtualMachine->registrai.rbx); //is komandos issitraukt kuris cia parametru blokas
            //kiek atorodo is kodo nutarem EXEx, x laikyti rax 

            ResursuPrimityvai::kurtiResursa(ResourceType::naujosProgramosPaleidimas, this, branduolys, vardas, 1, x);
        }
        case 22:{
            if(si == 4){ //PTXT
                komanda = KokiaKomanda::PTXT;
                step = 23;
                return Busena::READY;
            }
            else{
                step = 27;
                return Busena::READY;
            }
        }
        case 23:{
            if(arTuriResursa(ResourceType::kanaluIrenginys)){
                step = 24;
                return Busena::READY;
            }
            return Busena::BLOCKED;
        }
        case 24:{
            //Nustatomos kanalų įrenginio registrų reikšmės ir įvykdoma komanda XCHG (24)
            ChannelDevice* channelDevice = branduolys->realiMasina->getPointerToChannelDevice(); 
            if(komanda == KokiaKomanda::PNUM){
                channelDevice->setST(static_cast<uint8_t>(STisKurKopijuojam::registrasRbx));
                channelDevice->setDT(static_cast<uint8_t>(DTiKurKopijuojam::isvedimoSrautas));
                channelDevice->setIsNumber(1);
                channelDevice->setDB(0);
                channelDevice->setSB(0);
                channelDevice->setOFF(0);
                channelDevice->setRNUM(1);
                channelDevice->setReg(virtualMachine->registrai.rbx); //ammm kaip ta rbx registra pasiimt XD
                channelDevice->xchg(); 
            }
            if(komanda == KokiaKomanda::PTXT){
                channelDevice->setST(static_cast<uint8_t>(STisKurKopijuojam::registrasRbx));
                channelDevice->setDT(static_cast<uint8_t>(DTiKurKopijuojam::isvedimoSrautas));
                channelDevice->setIsNumber(0);
                channelDevice->setDB(0);
                channelDevice->setSB(0);
                channelDevice->setOFF(0);
                channelDevice->setRNUM(1);
                channelDevice->setReg(virtualMachine->registrai.rbx); //ammm kaip ta rbx registra pasiimt XD
                channelDevice->xchg(); 
            }
            if(komanda == KokiaKomanda::READ){
                channelDevice->setST(static_cast<uint8_t>(STisKurKopijuojam::ivedimoSrautas));
                channelDevice->setDT(static_cast<uint8_t>(DTiKurKopijuojam::registrasRbx));
                channelDevice->setIsNumber(1);
                channelDevice->setDB(0);
                channelDevice->setSB(0);
                channelDevice->setOFF(0);
                channelDevice->setRNUM(1);
                channelDevice->xchg(); 
                virtualMachine->registrai.rbx = channelDevice->getReg(); //kur padet ta rbx
            }

            step = 25;
            return Busena::READY;
        }
        case 25:{
            //Atlaisvinamas "Kanalų įrenginys" (25)
            ResursuPrimityvai::atlaisvintiResursa(ResourceType::kanaluIrenginys, this);
            step = 26;
            return Busena::READY;
        }
        case 26:{
            //Procesas Virtuali mašina nustatomas kaip pasiruošęs (26) ir zingsnis nustatomas i 1
            ProcesuPrimityvai::aktyvuotiProcesa(this, ProcesuTipai::virtualMachineProcess); //papildyti vm
            step = 9;
            return Busena::READY;
        }
        case 27:{
            if(si == 3){ //PNUM
                komanda = KokiaKomanda::PNUM;
                step = 23;
                return Busena::READY;
            }
            else{
                step = 28;
                return Busena::READY;
            }
        }
        case 28:{
            if(si == 2){//READ
                komanda = KokiaKomanda::READ;
                step = 23;
                return Busena::READY;
            }
            else{
                step = 29;
                return Busena::READY;
            }
        }
        case 29:{
            //Atalisvinamas resursas "Eilutė atmintyje" su pranešimu apie neatpažintą pertraukimą (29)
            ResursuPrimityvai::atlaisvintiResursa(ResourceType::eiluteAtmintyje, this, EiluteAtmintyje::neatpazintasPertraukimas); // papildyti pranesima
            step = 9;
            return Busena::READY;
        }
        default: 
            return Busena::BLOCKED;
    }
}

