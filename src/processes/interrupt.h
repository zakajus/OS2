#ifndef interrupt_h
#define interrupt_h


#include <iostream>
#include "process.h"
#include "jobGovernor.h"

using namespace std;

class Interrupt : public Process{
private:
	uint8_t step;
    uint8_t pi;
    uint8_t si;
    uint8_t ti;
public:
	Interrupt(uint32_t vardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, prioritetas,
            kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(0){}
	~Interrupt();

    Busena execute();
};

#endif