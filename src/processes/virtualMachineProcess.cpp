#include "virtualMachineProcess.h"

using namespace std;

Busena VirtualMachineProcess::execute(){
    switch (step)
    {
    case 1:{
        this->setRegistersInRealMachine();
        branduolys->realiMasina->setMode(0);
        step = 2;
        return Busena::READY;
    }
    case 2:{
        uint16_t pc = virtualMachine->getPc();
        
        uint32_t command = branduolys->realiMasina->getWordFromMemory();
        branduolys->realiMasina->reverseBytesInWord(command);
        virtualMachine->incrementPc();
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
            ResourceType tipas = ResourceType::pertraukimas;
            ResursuPrimityvai::kurtiResursa(tipas, this, branduolys, PertraukimoTipas::sisteminis, si);
            branduolys->realiMasina->changeSI(0);
        }

        uint8_t pi = branduolys->realiMasina->getPI();
        if(pi > 0){
            ResourceType tipas = ResourceType::pertraukimas;
            ResursuPrimityvai::kurtiResursa(tipas, this, branduolys, PertraukimoTipas::programinis, pi);
            branduolys->realiMasina->changePI(0);
        }

        uint8_t ti = branduolys->realiMasina->getTI();
        if(ti == 0){
            ResourceType tipas = ResourceType::pertraukimas;
            ResursuPrimityvai::kurtiResursa(tipas, this, branduolys, PertraukimoTipas::taimerio, ti);
            branduolys->realiMasina->changeTI(10);
        }
    
        this->saveRegistersToMemory();
        branduolys->realiMasina->setMode(1);
        step = 1; //not sure ar sitas tikrai reikalingas
        return Busena::READY;
    }
        
    
    default:
        return Busena::BLOCKED;
    }
}

