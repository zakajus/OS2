#ifndef channelDevice_h
#define channelDevice_h

#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>

class RealMachine;
class Monitor;
class Keyboard;

using namespace std;

class ChannelDevice {
private:
    uint16_t sb;
    uint16_t db;
    uint32_t off;
    uint32_t rnum; //how many words to copy
    uint32_t name;
    uint8_t st;
    uint8_t dt;

    uint32_t* userMemory;
    uint32_t* supervisorMemory;
    RealMachine* realMachine;
    Monitor* monitor;
    Keyboard* keyboard;
    const size_t userMemSize = 1024; //16 blocks * 16 words * 4 bytes
    const size_t supervisorMemSize = 2048; //32 blocks * 16 words * 4 bytes
    const size_t BLOCK_SIZE = 64; // 16 words × 4 bytes

public:
     ChannelDevice(RealMachine* realMachine, uint32_t* userMemory, uint32_t* supervisorMemory, Monitor* monitor, Keyboard* keyboard) 
        : sb(0), db(0), off(0), rnum(0), name(0), st(1), dt(1),
          realMachine(realMachine), userMemory(userMemory), supervisorMemory(supervisorMemory),
          monitor(monitor), keyboard(keyboard) {}
    ~ChannelDevice(){}

    void setSB(uint16_t value) { sb = value; }
    void setDB(uint16_t value) { db = value; }
    void setOFF(uint32_t value) { off = value; }
    void setRNUM(uint32_t value) { rnum = value; }
    void setNAME(uint32_t value) { name = value; }
    void setST(uint8_t value) { 
        if (value < 1 || value > 4) 
            realMachine->changePI(3);
        st = value; 
    }
    void setDT(uint8_t value) { 
        if (value < 1 || value > 3) 
            realMachine->changePI(3);
        dt = value; 
    }

    void copyFromUserMemory(uint8_t* dest, uint32_t offset);
    void copyFromSupervisorMemory(uint8_t* dest, uint32_t offset);
    void copyFromExternalMemory(uint8_t* dest);
    void copyFromInputStream(uint8_t* dest);

    void copyToUserMemory(uint32_t offset, const uint8_t* src);
    void copyToSupervisorMemory(uint32_t offset, const uint8_t* src);
    void copyToOutputStream(const uint8_t* src);
    void xchg();
    
};
 
#endif