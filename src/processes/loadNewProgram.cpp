#include "loadNewProgram.h"


Busena LoadNewProgram::execute(){
    switch (step){
    case 1:{
        if(arTuriResursa(ResourceType::naujosProgramosPaleidimas)){
            step = 2;
            return Busena::READY;
        }
        return Busena::BLOCKED;
    }
    case 2:{
        if(arTuriResursa(ResourceType::kanaluIrenginys)){
            step = 3;
            return Busena::READY;
        }
        return Busena::BLOCKED;
    }
    case 3:{
        if(arTuriResursa(ResourceType::supervizorineAtmintis)){
            step = 4;
            return Busena::READY;
        }
        return Busena::BLOCKED;
    }
    case 4:{
        if(arTuriResursa(ResourceType::isorineAtmintis)){
            step = 5;
            return Busena::READY;
        }
        return Busena::BLOCKED;
    }
    case 5:{
        //Nustatinėjami kanalų įrenginio registrai ir įvykdoma komanda XCHG (5)
        ChannelDevice* channelDevice = branduolys->realiMasina->getPointerToChannelDevice(); 

        channelDevice->setST(static_cast<uint8_t>(STisKurKopijuojam::isorineAtmintis));
        channelDevice->setDT(static_cast<uint8_t>(DTiKurKopijuojam::supervizorineAtmintis));
        channelDevice->setDB(0);
        channelDevice->setSB(0); 
        channelDevice->setNAME(programosPavadinimas); //per naujos programos paleidima gal paduot
        channelDevice->setOFF(0);
        channelDevice->setRNUM(256);
        channelDevice->xchg(); 
        step = 6;
        return Busena::READY;
    }
    case 6:{
        if(arYraParametrai){
            step = 7;
            return Busena::READY;
        }
        else{
            step = 8;
            return Busena::READY;
        }
    }
    case 7:{
        //Nustatinėjami kanalų įrenginio registrai ir įvykdoma komanda XCHG 
        ChannelDevice* channelDevice = branduolys->realiMasina->getPointerToChannelDevice(); 
        channelDevice->setDB(31);
        channelDevice->setDT(static_cast<uint8_t>(DTiKurKopijuojam::supervizorineAtmintis));
        uint32_t ptr = branduolys->realiMasina->getPtr();
        uint16_t number = branduolys->realiMasina->getWordFromMemory(ptr * 16 + parametruBlokoNr);
        channelDevice->setSB(number);
        channelDevice->setST(static_cast<uint8_t>(STisKurKopijuojam::vartotojoAtmintis));
        channelDevice->setOFF(0);
        channelDevice->setRNUM(16);
        channelDevice->xchg();

        //reiketu gal ne i paskutini parametru bloka kopijuot o iskart ten kur baiges programa??
        step = 8;
        return Busena::READY;
    }
    case 8:{
        //Atlaisvinamas “Kanalų įrenginys” resursas (8)
        ResursuPrimityvai::atlaisvintiResursa(ResourceType::kanaluIrenginys, this);
        step = 9;
        return Busena::READY;
    }
    case 9:{
        //Atlaisvinamas "Išorinės atminties" resursas (9)
        ResursuPrimityvai::atlaisvintiResursa(ResourceType::isorineAtmintis, this);
        step = 10;
        return Busena::READY;
    }
    case 10:{
        //Atlaisvinamas resursas “Užduotis supervizorinėje atmintyje”. Skirtas procesui JCL (10)
        ResursuPrimityvai::atlaisvintiResursa(ResourceType::uzduotisSupervizorinejeAtmintyje, this);
        step = 1;
        return Busena::READY;
    }
        
    
    default:
        return Busena::BLOCKED;
    }
}
