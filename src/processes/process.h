#ifndef process_h
#define process_h
#include <iostream>
#include <vector>
#include <cstdint>

#include "../OS2/src/resursuPrimityvai.h"
#include "../OS2/src/components/realMachine.h"
#include "../OS2/src/processes/resource.h"

using namespace std;

class Procesor;
class Resource;
class Element;
class RealMachine;

enum class Busena : uint8_t {
	EXECUTING,
	READY,
	BLOCKED,
	BLOCKED_STOPPED,
	READY_STOPPED
};

enum class ProcesuTipai : uint8_t{
    startStop,
    interrupt,
    virtualMachineProcess,
    jobGovernor, 
    mainProc,
    loadNewProgram, 
    printLine, 
    idle, 
    readFromInterface,
    jcl
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

    SavedRegisters(uint32_t rax, uint32_t rbx, uint8_t mode, uint16_t ds, uint16_t cs, uint16_t pc, uint16_t ti, uint8_t pi, uint8_t si, StatusFlag sf, uint32_t ptr){
        this->rax = rax;
        this->rbx = rbx;
        this->mode = mode;
        this->ds = ds;
        this->cs = cs;
        this->pc = pc;
        this->ti = ti;
        this->pi = pi;
        this->si = si;
        this->sf = sf;
        this->ptr = ptr;
    }
    SavedRegisters() : rax(0), rbx(0), mode(1), ds(0), cs(48), pc(0), ti(10), pi(0), si(0), ptr(0){}
};




class Process{
    public:
    uint32_t vardas;
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

    Process(uint32_t vardas, uint8_t prioritetas,
            ProcessList* kokiamSarasuiPriklauso, Process* tevas,
            Procesor* nuorodaIProcesoriu, Kernel* branduolys)
        : vardas(vardas),
          prioritetas(prioritetas),
          busena(Busena::BLOCKED),
          kokiamSarasuiPriklauso(kokiamSarasuiPriklauso),
          tevas(tevas),
          nuorodaIProcesoriu(nuorodaIProcesoriu),
          branduolys(branduolys){
        registrai = branduolys->realiMasina->getRegisterValues();
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

    void saveRegistersToMemory(){
        this->registrai = branduolys->realiMasina->getRegisterValues();
    }

    void setRegistersInRealMachine(){
        branduolys->realiMasina->setRegisters(registrai);
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