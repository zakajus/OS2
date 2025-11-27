#ifndef loadNewProgram_h
#define loadNewProgram_h

#include "process.h"
#include "../OS2/src/components/channelDevice.h"
#include "../OS2/src/resursuPrimityvai.h"

class LoadNewProgram : public Process{
private:
	uint8_t step;
    uint8_t arYraParametrai;
    uint8_t parametruBlokoNr;
    uint32_t programosPavadinimas;
public:
	LoadNewProgram(uint32_t vardas, uint8_t prioritetas, ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys, uint8_t arYraParametrai, uint8_t parametruBlokoNr, uint32_t programosPavadinimas):
             Process(vardas, prioritetas,kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(1){
                this->arYraParametrai = arYraParametrai; 
                this->parametruBlokoNr = parametruBlokoNr;
                this->programosPavadinimas = programosPavadinimas;
            }
	~LoadNewProgram();
    Busena execute();
};

#endif