#ifndef printLine_h
#define printLine_h

#include <iostream>
#include "process.h"
#include "../components/channelDevice.h"

using namespace std;

class PrintLine : public Process {
private:
	uint8_t step;
public:
	PrintLine(uint32_t vardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, prioritetas,
            kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(0){}
	~PrintLine();

	Busena execute();
};

#endif