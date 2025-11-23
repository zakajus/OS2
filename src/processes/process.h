#ifndef process_h
#define process_h
#include <iostream>
#include <vector>
#include <cstdint>

#include "../resursuPrimityvai.h"
#include "../src/resource.h"
#include "../OS2/src/components/realMachine.h"

using namespace std;

enum class Busena : uint8_t {
	EXECUTING,
	READY,
	BLOCKED,
	BLOCKED_STOPPED,
	READY_STOPPED
};

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



class Process{
    public:
    uint32_t vardas;
    uint32_t isorinisVardas;
    Busena busena; 
    uint8_t prioritetas;

    ProcessList* kokiamSarasuiPriklauso;  
    Process* tevas; 
    vector<Process*> vaikai; 

    Procesor* nuorodaIProcesoriu; 
    Kernel* branduolys;


    SavedRegisters registrai;
    
    vector<Resource> sukurtiResursai;
    vector<Resource> perduotiResursai;
    vector<Resource*> turimiResursai;

    Process(uint32_t vardas, uint32_t isorinisVardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys)
        : vardas(vardas),
          isorinisVardas(isorinisVardas),
          prioritetas(prioritetas),
          busena(Busena::BLOCKED),
          kokiamSarasuiPriklauso(kokiamSarasuiPriklauso),
          tevas(tevas),
          nuorodaIProcesoriu(nuorodaIProcesoriu),
          branduolys(branduolys){
        registrai = {};
    }

    bool arTuriResursa(ResourceType resourcetype){
        Resource* resursas = ResursuPrimityvai::prasytiResurso(resourcetype, this);
        if(resursas != nullptr){
            turimiResursai.push_back(resursas);
            return true;
        }
        else{
            return false;
        }
        

    }

    
    
};

class ProcessList{
    public:
    Kernel* branduolys; //Nuoroda į branduolį
    vector<Process> procesai; //Dinaminis procesų sąrašas. 
    uint32_t procesuSkaicius;  //Procesų skaičius procesų masyve.
};

struct StatusFlag{
    unsigned int cf : 1; //carry flag
    unsigned int of : 1; //overflow flag
    unsigned int af : 1; //auxiliary flag
    unsigned int zf : 1; //zero flag
};

class SavedRegisters{
    public:
    uint32_t rax;   // bendros paskirties
    uint32_t rbx;   // bendros paskirties
    uint8_t mode;   // darbo rezimas: 0 - vartotojas, 1 - supervizorius
    uint16_t ds;    // duomenu segmentas
    uint16_t cs;    // kodo segmentas
    uint16_t pc;    // programos counteris
    uint16_t ti;    // timeris
    uint8_t pi;     // programiniu pertraukimu registras
    uint8_t si;     // supervizoriniu pertraukimu registras
    StatusFlag sf;  // pozymiu registras
    uint32_t ptr;   // puslapiu lentele
};

class Procesor;
class Resource;
class Element;
class RealMachine;

class Kernel{
    public:
    vector<Process> procesai; //Bendras visų, esančių sistemoje, procesų sąrašas
    vector<Resource> sukurtiResursai; //Bendras visų, sistemoje esančių, resursų sąrašas
    vector<Process> pasiruoseProcesai; //Pasiruošusių procesų sąrašas
    vector<Process> einamiProcesai; //Einami procesai. Jų gali būti ne vienas, jei realioje
    //mašinoje yra keli procesoriai.
    RealMachine* realiMasina; //Nuoroda į realią mašiną.
};

#endif