#ifndef resource_h
#define resource_h

#include <iostream>
#include <cstdint>

using namespace std;

class Process;
class Kernel;

enum class ResourceType : uint16_t {
	isVartotojoSasajos,
	kanaluIrenginys,
	vartotojoAtmintis,
	supervizorineAtmintis,
	isorineAtmintis,
	uzduotisSupervizorinejeAtmintyje,
	uzduotisBugne, 
	MOSpabaiga,
	eiluteAtmintyje,
	isInterrupt, 
	neegzistuojantis,
	pertraukimas, 
	naujosProgramosPaleidimas
};

enum class PertraukimoTipas : uint8_t{
    sisteminis, 
    programinis,
    taimerio
};


enum class EiluteAtmintyje{
    neegzistuojantiSistemineKomanda,
    galimosSisteminesKomandos,
    programinisPertraukimas1, //ar nereikia variantu
    programinisPertraukimas2,
    programinisPertraukimas3,
    programinisPertraukimas4,
    neatpazintasPertraukimas,
    AMJtrukumasSupervizorine,
    haltTrukumasSupervizorine, 
    programosTrukumasSupervizorine, 
};

// class Element{
//     public:
//     ElementList* savininkas;
//     Process* gavejas;
//     Process* siuntejas;
//     bool grazintiSarasa;
// };

// class ElementList{
//     public:
//     Resource* savininkas;
//     uint8_t skaicius;
//     vector<Element> elementai;
// };

// class ResourceList {
//     public:
//     uint8_t skaicius;
//     Kernel* branduolys;
//     vector<Resource> resursai;
// };

class Resource{
public:
    uint32_t vardas;
    ResourceType tipas;
    Process* sukurejas;
    
    Process* laukiantysProcesai;
    uint8_t laukianciuKiekis;
    Kernel* branduolys;
    // ResourceList* resursai;
    // ElementList* elementai;
    // Element* laukianciuProcesuResursai;

    Resource(uint32_t vardas, ResourceType tipas, Process* sukurejas, Kernel* branduolys){
        this->vardas = vardas;
        this->tipas = tipas;
        this->sukurejas = sukurejas;
        this->branduolys = branduolys;
    }
};



class ProgramosPaleidimoResursas: public Resource{
    public:
        uint32_t programosPavadinimas;
        uint8_t arYraParametrai;
        uint8_t parametruBlokoNr;
        ProgramosPaleidimoResursas(uint32_t vardas, ResourceType tipas, Process* sukurejas, Kernel* branduolys, uint8_t arYraParametrai, uint32_t programosPavadinimas, uint8_t parametruBlokoNr) 
        : Resource(vardas, tipas, sukurejas, branduolys){
            this->programosPavadinimas = programosPavadinimas;
            this->arYraParametrai = arYraParametrai;
            this->parametruBlokoNr = parametruBlokoNr;
        }
};

class PertraukimuResursas : public Resource{
    public:
        PertraukimoTipas pertraukimoTipas;
        uint8_t pertraukimoReiksme;

        PertraukimuResursas(uint32_t vardas, ResourceType tipas, Process* sukurejas, Kernel* branduolys, PertraukimoTipas PertraukimoTipas, uint8_t pertraukimoReiksme) 
        : Resource(vardas, tipas, sukurejas, branduolys){
            this->pertraukimoTipas = pertraukimoTipas;
            this->pertraukimoReiksme = pertraukimoReiksme;
        }
};



class EiluteAtmintyjeResursas : public Resource{
    public:
    EiluteAtmintyje pranesimas;


    EiluteAtmintyjeResursas(uint32_t vardas, ResourceType tipas, Process* sukurejas, Kernel* branduolys, EiluteAtmintyje pranesimas) 
        : Resource(vardas, tipas, sukurejas, branduolys){
            this->pranesimas = pranesimas;
        }
};

#endif