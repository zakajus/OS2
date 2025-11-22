#ifndef resource_h
#define resource_h

#include <iostream>
#include <cstdint>
#include "../src/processes/process.h"

using namespace std;

class Element{
    ElementList* savininkas;
    Process* gavejas;
    Process* siuntejas;
    bool grazintiSarasa;
};

class ElementList{
    Resource* savininkas;
    uint8_t skaicius;
    vector<Element> elementai;
};

class ResourceList {
    uint8_t skaicius;
    Kernel* branduolys;
    vector<Resource> resursai;
};

class Resource{
    uint32_t vardas;
    ResourceType tipas;
    Process* sukurejas;
    ElementList* elementai;
    Process* laukiantysProcesai;
    uint8_t laukianciuKiekis;
    Kernel* branduolys;
    ResourceList* resursai;
    Element* laukianciuProcesuResursai;

};

#endif