#include <iostream>
#include <vector>

using namespace std;

class Process{
    ProcessList* kokiamSarasuiPriklauso; //procesų sąrašas, kuriam priklauso procesas.
        // Tai gali būti pasiruošusių procesų sąrašas arba blokuotų,
        // dėl kokio nors resurso, procesų sąrašas. 
    uint32_t vardas; //Vidinis proceso vardas. 
    //Kiekvienas sisteminis objektas turi savo unikalų numerį
    SavedRegisters registrai; //Proceso išsaugota procesoriaus būsena.
    Procesor* nuorodaIProcesoriu; 
    vector<Resource> sukurtiResursai; //Proceso sukurti resursai
    vector<Element> perduotiResursai; //Procesui kūrimo metu perduoti resursai.
    uint8_t busena; 
    uint32_t prioritetas;
    Process* tevas; //nuoroda į procesą–tėvą.
    vector<Process> vaikai; //procesų–vaikų sąrašas.
    Kernel* branduolys; //Nuoroda į branduolį.
    uint32_t isorinisVardas;
};

class ProcessList{
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
    vector<Process> procesai; //Bendras visų, esančių sistemoje, procesų sąrašas
    vector<Resource> sukurtiResursai; //Bendras visų, sistemoje esančių, resursų sąrašas
    vector<Process> pasiruoseProcesai; //Pasiruošusių procesų sąrašas
    vector<Process> einamiProcesai; //Einami procesai. Jų gali būti ne vienas, jei realioje
    //mašinoje yra keli procesoriai.
    RealMachine* realiMasina; //Nuoroda į realią mašiną.
};

int main(){

    return 0;
}