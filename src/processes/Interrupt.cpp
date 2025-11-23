#include <iostream>
#include "process.h"

using namespace std;

class Interrupt : public Process{
private:
	uint8_t step;
    uint8_t pi;
    uint8_t si;
    uint8_t ti;
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
            if(arTuriResursa(ResourceType::pertraukimas)){
                step = 1;
				return Busena::READY;
            }
            return Busena::BLOCKED;
        }
        case 1:{
            //pertraukimo tipo identifikacimas
            //pasiimt iš realios mašinos SI PI ir TI reikšmes ir kažkur išsaugot ką surandam
            pi = branduolys->realiMasina->getPI();
            si = branduolys->realiMasina->getSI();
            ti = branduolys->realiMasina->getTI();


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