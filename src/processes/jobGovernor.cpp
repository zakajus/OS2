#include <iostream>
#include "process.h"

using namespace std;

class JobGovernor : public Process{
private:
	uint8_t step;
public:
	JobGovernor(uint32_t vardas, uint32_t isorinisVardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, isorinisVardas, prioritetas,
            kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(1){}
	~JobGovernor();

    Busena execute(){
        switch(step){
            case 1:{
                Resource* resursas = arTuriResursa(ResourceType::vartotojoAtmintis);
                if(resursas){
                    step = 2;
                    return Busena::READY;
                }
                return Busena::BLOCKED;
            }
            case 2:{
                Resource* resursas = arTuriResursa(ResourceType::kanaluIrenginys);
                if(resursas){
                    step = 3;
                    return Busena::READY;
                }
                return Busena::BLOCKED;
            }
            case 3:{
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
                step = 9;
                return Busena::READY;
            }
            case 9:{
                Resource* resursas = arTuriResursa(ResourceType::isInterrupt);
                if(resursas){
                    step = 10;
                    return Busena::READY;
                }
                return Busena::BLOCKED;
            }
            case 10:{
                //proceso virtuali masina stabdymas
                step = 11;
                return Busena::READY;
            }
            case 11:{
                if(/*Programinis pertraukimas*/1){
                    step = 12;
                    return Busena::READY;
                }
                else{
                    step = 17;
                    return Busena::READY;
                }
            }
            case 12:{
                //atlaisvinti resursa eilute atmintyje
                step = 13;
                return Busena::READY;
            }
            case 13:{
                //naikinam procesa virtuali masina
                step = 14;
                return Busena::READY;
            }
            case 14:{
                //atlaisvinti resursa "vartotojo atmintis"
                step = 15;
                return Busena::READY;
            }
            case 15:{
                //Atlaisvinti fiktyvų “Užduotis būgne” resursą (15)
                step = 16;
                return Busena::READY;
            }
            case 16:{
                Resource* resursas = arTuriResursa(ResourceType::neegzistuojantis);
                if(resursas){
                    step = 9;
                    return Busena::READY;
                }
                return Busena::BLOCKED;
            }
            case 17:{
                if(/*Sisteminis pertraukimas*/1){
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
                if(/*halt*/1){
                    step = 13;
                    return Busena::READY;
                }
                else{
                    step = 20;
                    return Busena::READY;
                }
            }
            case 20:{
                if(/*EXEx*/1){
                    step = 21;
                    return Busena::READY;
                }
                else{
                    step = 22;
                    return Busena::READY;
                }
            }
            case 22:{
                if(/*PTXT*/1){
                    step = 23;
                    return Busena::READY;
                }
                else{
                    step = 27;
                    return Busena::READY;
                }
            }
            case 23:{
                Resource* resursas = arTuriResursa(ResourceType::kanaluIrenginys);
                if(resursas){
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
                step = 26;
                return Busena::READY;
            }
            case 26:{
                //Procesas Virtuali mašina nustatomas kaip pasiruošęs (26)
                step = 9;
                return Busena::READY;
            }
            case 27:{
                if(/*PNUM*/1){
                    step = 23;
                    return Busena::READY;
                }
                else{
                    step = 28;
                    return Busena::READY;
                }
            }
            case 28:{
                if(/*READ*/1){
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
                step = 9;
                return Busena::READY;
            }
            default: 
                return Busena::BLOCKED;
        }
    }

};