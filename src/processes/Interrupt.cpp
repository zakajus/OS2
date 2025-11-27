#include "interrupt.h"

using namespace std;

Busena Interrupt::execute(){
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
        //?
        JobGovernor* jobGovernor = static_cast<JobGovernor*>(perduotiResursai.at(0).sukurejas->tevas);
        if (jobGovernor != nullptr) {
            jobGovernor->setInterruptRegisters(pi, si, ti);
        } else {
            cerr << "Error: Process is not a JobGovernor!" << endl;
        }
        
    }
    case 3:{
        //Atlaisvinamas “Iš Interrupt” resursas skirtas konkrečiam JobGovernor (4)
        Process* nuorodaIVM = perduotiResursai.at(0).sukurejas; 
        JobGovernor* jobGovernor = static_cast<JobGovernor*>(perduotiResursai.at(0).sukurejas->tevas);
        ResursuPrimityvai::kurtiResursa(ResourceType::isInterrupt, this, branduolys, nuorodaIVM, pi, si, ti, jobGovernor);
        
    }
    default:
        return Busena::BLOCKED;
    }
}
