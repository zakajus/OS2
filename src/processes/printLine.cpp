#include <iostream>
#include "process.h"
#include "../components/channelDevice.h"

using namespace std;

class PrintLine : public Process {
private:
	uint8_t step;
public:
	PrintLine(uint32_t vardas, uint32_t isorinisVardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys): Process(vardas, isorinisVardas, prioritetas,
            kokiamSarasuiPriklauso, tevas,
            nuorodaIProcesoriu, branduolys), step(0){}
	~PrintLine();

	Busena execute() {
		switch (step) {
		case 0:{
			if(arTuriResursa(ResourceType::eiluteAtmintyje)){
				step = 1;
				return Busena::READY;
			}
			return Busena::BLOCKED;
		}
		case 1:{
			if(arTuriResursa(ResourceType::kanaluIrenginys)){
				step = 2;
				return Busena::READY;
			}
			return Busena::BLOCKED;
		}
		case 2:{
			ChannelDevice* channelDevice = branduolys->realiMasina->getPointerToChannelDevice(); //pasiimt tikra channel device nuoroda
			channelDevice->setST(6); //is atmintyje esanciu pranesimu?
			channelDevice->setDT(3); //isvedimo srautas
			channelDevice->setSB(0); //pakeist i ta skaiciu, kuris pranesimas
			channelDevice->setRNUM(1); //pakeist i tiek baitu ar zodziu kiek reikia
			channelDevice->xchg();

			step = 3;
			return Busena::READY;
		}
		case 3:{
			ResursuPrimityvai::atlaisvintiResursa(ResourceType::kanaluIrenginys, this, 1); //pakeist ta vienetuka i tikra id
			//atlaisvinti kanalu irenginio resursa
			step = 0;
			return Busena::BLOCKED;
		}
		default:
			return Busena::BLOCKED;
		}
	}
};