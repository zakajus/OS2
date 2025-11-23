#ifndef resursuPrimityvai_h
#define resursuPrimityvai_h

#include "../src/processes/process.h"

class ResursuPrimityvai{
public:
    static void kurtiResursa();
    static void naikintiResursa();
    static Resource* prasytiResurso(ResourceType tipas, Process* prasantisProcesas);
    static void atlaisvintiResursa(ResourceType tipas, Process* atlaisvinantisProcesas, uint32_t id);
};

#endif