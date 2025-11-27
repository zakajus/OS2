#ifndef virtualMachineProcess_h
#define virtualMachineProcess_h

#include "process.h"
#include "virtualMachine.h"

using namespace std;



class VirtualMachineProcess : public Process{
private:
	uint8_t step;
    VirtualMachine* virtualMachine;
    uint32_t command;


public:
	VirtualMachineProcess(uint32_t vardas, uint8_t prioritetas, ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, prioritetas,
            kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(1), command(0){
                virtualMachine = branduolys->realiMasina->createVirtualMachine();
            }
	~VirtualMachineProcess();

    Busena execute();
};


#endif