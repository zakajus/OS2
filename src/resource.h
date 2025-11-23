#ifndef resource_h
#define resource_h

#include <iostream>
#include <cstdint>
#include "../src/processes/process.h"

using namespace std;

class Element{
    public:
    ElementList* savininkas;
    Process* gavejas;
    Process* siuntejas;
    bool grazintiSarasa;
};

class ElementList{
    public:
    Resource* savininkas;
    uint8_t skaicius;
    vector<Element> elementai;
};

class ResourceList {
    public:
    uint8_t skaicius;
    Kernel* branduolys;
    vector<Resource> resursai;
};

class Resource{
public:
    uint32_t vardas;
    ResourceType tipas;
    Process* sukurejas;
    ElementList* elementai;
    Process* laukiantysProcesai;
    uint8_t laukianciuKiekis;
    Kernel* branduolys;
    ResourceList* resursai;
    Element* laukianciuProcesuResursai;

    Resource(uint32_t vardas, ResourceType tipas, Process* sukurejas, Kernel* branduolys){
        this->vardas = vardas;
        this->tipas = tipas;
        this->sukurejas = sukurejas;
        this->branduolys = branduolys;
    }
};

enum class PertraukimoTipas{
    sisteminis, 
    programinis,
    taimerio
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

#endif