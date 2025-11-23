#include "process.h"

class LoadNewProgram : public Process{
private:
	uint8_t step;
public:
	LoadNewProgram(uint32_t vardas, uint32_t isorinisVardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, isorinisVardas, prioritetas,
            kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(1){}
	~LoadNewProgram();
    Busena execute(){
        switch (step){
        case 1:{
            if(arTuriResursa(ResourceType::naujosProgramosPaleidimas)){
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
            if(arTuriResursa(ResourceType::supervizorineAtmintis)){
                step = 4;
                return Busena::READY;
            }
            return Busena::BLOCKED;
        }
        case 4:{
            if(arTuriResursa(ResourceType::isorineAtmintis)){
                step = 5;
                return Busena::READY;
            }
            return Busena::BLOCKED;
        }
        case 5:{
            //Nustatinėjami kanalų įrenginio registrai ir įvykdoma komanda XCHG (5)
            step = 6;
            return Busena::READY;
        }
        case 6:{
            if(/*yra parametru blokas*/1){
                step = 7;
                return Busena::READY;
            }
            else{
                step = 8;
                return Busena::READY;
            }
        }
        case 7:{
            //Nustatinėjami kanalų įrenginio registrai ir įvykdoma komanda XCHG 
            step = 8;
            return Busena::READY;
        }
        case 8:{
            //Atlaisvinamas “Kanalų įrenginys” resursas (8)
            step = 9;
            return Busena::READY;
        }
        case 9:{
            //Atlaisvinamas "Išorinės atminties" resursas (9)
            step = 10;
            return Busena::READY;
        }
        case 10:{
            //Atlaisvinamas resursas “Užduotis supervizorinėje atmintyje”. Skirtas procesui JCL (10)
            step = 1;
            return Busena::READY;
        }
           
        
        default:
            return Busena::BLOCKED;
        }
    }
};