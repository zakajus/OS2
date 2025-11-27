#ifndef jobGovernor_h
#define jobGovernor_h

#include <iostream>
#include "process.h"
#include "../procesuPrimityvai.h"
#include "../OS2/src/components/channelDevice.h"
//#include "../OS2/src/components/virtualMachine.h"
#include "virtualMachineProcess.h"

using namespace std;

enum class KokiaKomanda{
    PTXT,
    PNUM,
    READ
};

class JobGovernor : public Process{
private:
	uint8_t step;
    uint8_t pi;
    uint8_t si;
    uint8_t ti;
    KokiaKomanda komanda;
    VirtualMachineProcess* virtualMachine;
    uint32_t pageTable[16];


public:
	JobGovernor(uint32_t vardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys);
	~JobGovernor();

    void setInterruptRegisters(uint8_t pi, uint8_t si, uint8_t ti);
    Busena execute();

};

#endif