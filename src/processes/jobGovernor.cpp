#include <iostream>
#include "process.h"
#include "../procesuPrimityvai.h"

using namespace std;

class JobGovernor : public Process{
private:
	uint8_t step;
    uint8_t pi;
    uint8_t si;
    uint8_t ti;


public:
	JobGovernor(uint32_t vardas, uint32_t isorinisVardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, isorinisVardas, prioritetas,
            kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(1){pi = 0; si = 0; ti = 10;}
	~JobGovernor();

    void setInterruptRegisters(uint8_t pi, uint8_t si, uint8_t ti){
        this->pi = pi;
        this->si = si;
        this->ti = ti;
    }

    Busena execute(){
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
                //ar reik kazkur pasidet ta puslapiu lentele ir ptr registra
                //Išskiriama atmintis virtualiai mašinai ir sukuriama puslapių lentelė 
                step = 4;
                return Busena::READY;
            }
            case 4:{
                //Nustatinėjami kanalų įrenginio registrai ir įvykdoma komanda XCHG (4)
                step = 5;
                return Busena::READY;
            }
            case 5:{
                //Atlaisvinamas "Kanalų įrenginio" resursas (5)
                step = 6;
                return Busena::READY;
            }
            
            case 6:{
                //Atlaisvinamas resursas "Supervizorinė atmintis" (6)
                step = 7;
                return Busena::READY;
            }
            case 7:{
                //Atlaisvinamas resursas "Užduotis būgne" (7)
                step = 8;
                return Busena::READY;
            }
            case 8:{
                //Proceso virtuali masina kurimas
                ProcesuPrimityvai::kurtiProcesa();
                
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
                ProcesuPrimityvai::stabdytiProcesa();
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
                ResursuPrimityvai::atlaisvintiResursa(ResourceType::vartotojoAtmintis, this, 1); //pakeisti id
                step = 15;
                return Busena::READY;
            }
            case 15:{
                //Atlaisvinti fiktyvų “Užduotis būgne” resursą (15)
                ResursuPrimityvai::atlaisvintiResursa(ResourceType::uzduotisBugne, this, 1); //pakeisti id
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
            case 22:{
                if(si == 4){ //PTXT
                    // kaip nustatyti kanalu irenginio rieksmes
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
                step = 25;
                return Busena::READY;
            }
            case 25:{
                //Atlaisvinamas "Kanalų įrenginys" (25)
                ResursuPrimityvai::atlaisvintiResursa(ResourceType::kanaluIrenginys, this, 1); //pakeisti id
                step = 26;
                return Busena::READY;
            }
            case 26:{
                //Procesas Virtuali mašina nustatomas kaip pasiruošęs (26) ir zingsnis nustatomas i 1
                step = 9;
                return Busena::READY;
            }
            case 27:{
                if(si == 3){ //PNUM
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
                ResursuPrimityvai::atlaisvintiResursa(ResourceType::eiluteAtmintyje, this, 1); //pakeisti id ir papildyti pranesima
                step = 9;
                return Busena::READY;
            }
            default: 
                return Busena::BLOCKED;
        }
    }

};