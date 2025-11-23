#include <iostream>
#include "process.h"

using namespace std;

class Interrupt : public Process{
private:
	uint8_t step;
public:
	Interrupt(uint32_t vardas, uint32_t isorinisVardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, isorinisVardas, prioritetas,
            kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(0){}
	~Interrupt();

    Busena execute(){
        switch (step)
        {
        case 0:{
            Resource* resursas = arTuriResursa(ResourceType::pertraukimas);
            if(resursas){
                step = 1;
				return Busena::READY;
            }
            return Busena::BLOCKED;
        }
        case 1:{
            //pertraukimo tipo identifikacimas
            break;
        }  
        case 2:{
            //JobGovernor, atsakingo už pertrauktosios VM darbą, nustatymas (3)
        }
        case 3:{
            //Atlaisvinamas “Iš Interrupt” resursas skirtas konkrečiam JobGovernor (4)
        }
        default:
            return Busena::BLOCKED;
        }
    }
};