#ifndef channelDevice_h
#define channelDevice_h

#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>

#include "realMachine.h"

class RealMachine;
class Monitor;
class Keyboard;

using namespace std;

enum class STisKurKopijuojam : uint8_t{
    vartotojoAtmintis = 1,
    supervizorineAtmintis,
    isorineAtmintis,
    ivedimoSrautas,
    registrasRbx,
    eiluteAtmintyjeResursai
};

enum class DTiKurKopijuojam : uint8_t{
    vartotojoAtmintis = 1,
    supervizorineAtmintis,
    isvedimoSrautas,
    registrasRbx
};

class ChannelDevice {
private:
    uint16_t sb;
    uint16_t db;
    uint32_t off;
    uint32_t rnum; //how many words  to copy
    uint32_t name;
    uint8_t st;
    uint8_t dt;
    uint8_t isNumber; // 0 - false, 1 -true
    uint32_t reg; //spausdinimui/irasymo nes nieko negali zinot apis os registrus


    uint32_t* userMemory;
    uint32_t* supervisorMemory;
    RealMachine* realMachine;
    Monitor* monitor;
    Keyboard* keyboard;
    const size_t userMemSize = 1632; 
    const size_t supervisorMemSize = 512; 
    const size_t BLOCK_SIZE = 16; 

    string eilutesAtmintyje[10] = {"Įvesta sisteminė komanda neegzistuoja\n", "Tokia komanda neegzistuoja, įveskite >boom, norėdami išjungti operacinę sistemą, arba .XXXX norėdami paleisti programą, kur XXXX - programos pavadinimas.\n",
    "Programinis pertraukimas: neteisingas adresas.\n", "Programinis pertraukimas: neteisingas operacijos kodas", "Programinis pertraukimas: neteisingas priskyrimas", "Programinis pertraukimas: dalyba iš 0.\n", 
    "Neatpažintas pertraukimas\n", "Bloko $AMJ trūkumas, atmintis: supervizorinė.\n", "Bloko $END trūkumas, atmintis: supervizorinė.\n",  "Nuskaityta tuščia programa, atmintis: supervizorinė.\n"}; 

public:
     ChannelDevice(RealMachine* realMachine, uint32_t* userMemory, uint32_t* supervisorMemory, Monitor* monitor, Keyboard* keyboard) 
        : sb(0), db(0), off(0), rnum(0), name(0), st(1), dt(1), isNumber(0),
          userMemory(userMemory), supervisorMemory(supervisorMemory), realMachine(realMachine),
          monitor(monitor), keyboard(keyboard) {}
    ~ChannelDevice(){}

    void setReg(uint32_t value) { reg = value; }
    uint32_t getReg(){return reg;}
    void setSB(uint16_t value) { sb = value; }
    void setDB(uint16_t value) { db = value; }
    void setOFF(uint32_t value) { off = value; }
    void setRNUM(uint32_t value) { rnum = value; }
    void setNAME(uint32_t value) { name = value; }
    void setIsNumber(uint8_t value) {isNumber = value;}
    void setST(uint8_t value) { 
        if (value < 1 || value > 6) 
            realMachine->changePI(3);
        st = value; 
    }
    void setDT(uint8_t value) { 
        if (value < 1 || value > 4) 
            realMachine->changePI(3);
        dt = value; 
    }

    void copyFromUserMemory(uint32_t* dest, uint32_t offset);
    void copyFromSupervisorMemory(uint32_t* dest, uint32_t offset); 
    void copyFromExternalMemory(uint32_t* dest); 
    void copyFromInputStream(uint32_t* dest); 
    void copyFromRbx(uint32_t* dest);
    void copyFromEiluteAtmintyje();

    void copyToUserMemory(uint32_t offset, const uint32_t* src); 
    void copyToSupervisorMemory(uint32_t offset, const uint32_t* src); 
    void copyToOutputStream(uint32_t* src); 
    void copyToRbx(const uint32_t* src); 
    void xchg();
    
};
 
#endif
