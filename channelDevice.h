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
    const size_t userMemSize = 256*16; //16 blocks * 16 words no idea kodel butent tiek ok..
    const size_t supervisorMemSize = 512; //32 blocks * 16 words 
    const size_t BLOCK_SIZE = 16; // 16 words 

public:
     ChannelDevice(RealMachine* realMachine, uint32_t* userMemory, uint32_t* supervisorMemory, Monitor* monitor, Keyboard* keyboard) 
        : sb(0), db(0), off(0), rnum(0), name(0), st(1), dt(1), isNumber(0),
          userMemory(userMemory), supervisorMemory(supervisorMemory), realMachine(realMachine),
          monitor(monitor), keyboard(keyboard) {}
    ~ChannelDevice(){}

    void setReg(uint32_t value) { rnum = value; }
    uint32_t getReg(){return reg;}
    void setSB(uint16_t value) { sb = value; }
    void setDB(uint16_t value) { db = value; }
    void setOFF(uint32_t value) { off = value; }
    void setRNUM(uint32_t value) { rnum = value; }
    void setNAME(uint32_t value) { name = value; }
    void setIsNumber(uint8_t value) {isNumber = value;}
    void setST(uint8_t value) { 
        if (value < 1 || value > 5) 
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
    void copyFromExternalMemory(uint32_t* dest); //veikia
    void copyFromInputStream(uint32_t* dest); //KVIECIANT NURODYT BAIT
    void copyFromRbx(uint32_t* dest);

    void copyToUserMemory(uint32_t offset, const uint32_t* src);
    void copyToSupervisorMemory(uint32_t offset, const uint32_t* src); //veikia
    void copyToOutputStream(const uint32_t* src);
    void copyToRbx(const uint32_t* src);
    void xchg();
    
};
 
#endif