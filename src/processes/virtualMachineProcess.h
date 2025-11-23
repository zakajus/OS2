#ifndef virtualMachineProcess_h
#define virtualMachineProcess_h

#include "process.h"
#include "virtualMachine.h"

using namespace std;



class VirtualMachineProcess : public Process{
private:
	uint8_t step;
    VirtualMachine* virtualMachine;
    uint32_t pageTable[16];


public:
	VirtualMachineProcess(uint32_t vardas, uint32_t isorinisVardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, isorinisVardas, prioritetas,
            kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(1){
                virtualMachine = branduolys->realiMasina->createVirtualMachine();
                uint32_t ptr = branduolys->realiMasina->getPtr();
                for(int i = 0; i < 16; i++){
                    pageTable[i] = branduolys->realiMasina->getWordFromMemory(ptr *16 + i);
                }
            }
	~VirtualMachineProcess();

    Busena execute(){
        switch (step)
        {
        case 1:{
            //perjungti prcesoriu i vartotojo rezima
            branduolys->realiMasina->setMode(0);
            step = 2;
            return Busena::READY;
        }
        case 2:{
            uint16_t pc = virtualMachine->getPc();
            int i = pc / 16;
            int j = pc % 16;
            uint32_t command = branduolys->realiMasina->getWordFromMemory(pageTable[i]*16+j);
            branduolys->realiMasina->reverseBytesInWord(command);
            ++pc;
            virtualMachine->runNextCommand(command);
            if(branduolys->realiMasina->isInterruptNeeded()){
                step = 3;
            }
            else{
                step = 2;
            }
            return Busena::READY;
        }
        case 3:{
            //sukurti resursa "Pertraukimas"
             branduolys->realiMasina->setMode(1);
            uint8_t si = branduolys->realiMasina->getSI();
            if(si > 0){
                uint32_t vardas = 1; //pakeist
                ResourceType tipas = ResourceType::pertraukimas;
                ResursuPrimityvai::kurtiResursa(vardas, tipas, this, branduolys, PertraukimoTipas::sisteminis, si);
            }

            uint8_t pi = branduolys->realiMasina->getPI();
            if(pi > 0){
                uint32_t vardas = 1; //pakeist
                ResourceType tipas = ResourceType::pertraukimas;
                ResursuPrimityvai::kurtiResursa(vardas, tipas, this, branduolys, PertraukimoTipas::programinis, pi);
            }

            uint8_t ti = branduolys->realiMasina->getTI();
            if(ti == 0){
                uint32_t vardas = 1; //pakeist
                ResourceType tipas = ResourceType::pertraukimas;
                ResursuPrimityvai::kurtiResursa(vardas, tipas, this, branduolys, PertraukimoTipas::taimerio, ti);
            }
        
            
            
            step = 1; //not sure ar sitas tikrai reikalingas
            return Busena::READY;
        }
            
        
        default:
            return Busena::BLOCKED;
        }
    }
};


#endif