#ifndef resursuPrimityvai_h
#define resursuPrimityvai_h




// Forward declarations instead of includes
class Process;
class Kernel;

class ResursuPrimityvai{
public:
    static void kurtiResursa(ResourceType tipas, Process* sukurejas, Kernel* branduolys, PertraukimoTipas PertraukimoTipas, uint8_t pertraukimoReiksme);
    static void kurtiResursa(ResourceType tipas, Process* sukurejas, Kernel* branduolys, Process* nuorodaVM, uint8_t pi, uint8_t si, uint8_t ti, Process* kokiamJobGovernor);
    static void kurtiResursa(ResourceType tipas, Process* sukurejas, Kernel* branduolys, uint32_t pavadinimas, uint8_t yraParametruBlokas, uint8_t x);
    static void naikintiResursa();
    static Resource* prasytiResurso(ResourceType tipas, Process* prasantisProcesas);
    static void atlaisvintiResursa(ResourceType tipas, Process* atlaisvinantisProcesas);
    //static void atlaisvintiResursa(ResourceType tipas, Process* atlaisvinantisProcesas, uint8_t arYraBlokuPrametras, uint8_t parametruBlokoNr); //kazka dar turbut reiktu paduot
    static void atlaisvintiResursa(ResourceType tipas, Process* atlaisvinantisProcesas, uint8_t pi);
    static void atlaisvintiResursa(ResourceType tipas, Process* atlaisvinantisProcesas, EiluteAtmintyje pranesimas);
};

#endif