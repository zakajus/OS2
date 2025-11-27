#include "printLine.h"
Busena PrintLine::execute() {
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
		ChannelDevice* channelDevice = branduolys->realiMasina->getPointerToChannelDevice(); 
		channelDevice->setST(6); //is atmintyje esanciu pranesimu?
		channelDevice->setDT(3); //isvedimo srautas
		channelDevice->setSB(0); //pakeist i ta skaiciu, kuris pranesimas //is resource.h EiluteAtmintyje pranesimu
		channelDevice->setRNUM(1); //pakeist i tiek baitu ar zodziu kiek reikia
		channelDevice->xchg();

		step = 3;
		return Busena::READY;
	}
	case 3:{
		ResursuPrimityvai::atlaisvintiResursa(ResourceType::kanaluIrenginys, this); 
		step = 0;
		return Busena::BLOCKED;
	}
	default:
		return Busena::BLOCKED;
	}
}