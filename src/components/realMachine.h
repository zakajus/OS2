#ifndef realMachine_h
#define realMachine_h

 #include <iostream>
#include <ctime>
#include <vector>
#include <cstdint>
#include <iomanip>

#include "keyboard.h"
#include "monitor.h"

#include "virtualMachine.h"
#include "../OS2/src/processes/process.h"


class ChannelDevice;

using namespace std;

class RealMachine {
private:
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
    VirtualMachine *virtualMachine;
    ChannelDevice *channelDevice;
    uint32_t userMemory[1632];      //102 blokai po 16 žodžių
    uint32_t supervisorMemory[512]; //32  blokai po 16 žodžių

    vector<int> freeBlocks;
    vector<int> occupiedBlocks;

public:
    RealMachine(Monitor &monitor, Keyboard &keyboard);

    uint32_t translateLocalAdressToRealAddress(uint8_t x, uint8_t y);
    void printAllRegisterValues();
    void printCurrentPage();
    void printVirtualMemory();
    void printRealMemory();
    void printAsHex(uint32_t* memory, int size);
    void printAsASCII(uint32_t *memory, int size);

    void changeSI(uint8_t i);
    void changePI(uint8_t i);
    void changeTI(uint8_t i){
        this->ti = i;
    }
    uint8_t getSI();
    uint8_t getPI();
    uint8_t getTI();
    uint32_t getRbx(){
        return rbx;
    }

    void setRbx(uint32_t newRbx){
        this->rbx = newRbx;
    }

    ChannelDevice* getPointerToChannelDevice(){
        return channelDevice;
    }

    VirtualMachine* createVirtualMachine(){
        return new VirtualMachine(rax, rbx, ds, cs, pc, sf, *this);
    }

    void setRegisters(SavedRegisters registrai){
        this->rax = registrai.rax;
        this->rbx = registrai.rbx;
        this->mode = registrai.mode;
        this->ds = registrai.ds;
        this->cs = registrai.cs;
        this->pc = registrai.pc;
        this->ti = registrai.ti;
        this->pi = registrai.pi;
        this->si = registrai.si;
        this->sf = registrai.sf;
        this->ptr = registrai.ptr;
    }

    SavedRegisters getRegisterValues(){
        return SavedRegisters(rax, rbx, mode, ds, cs, pc, ti, pi, si, sf, ptr);
    }



    void rm_run(uint32_t name);
    void setEverythingForSteppingMode(uint32_t name);
    int stepIntoNextCommand();
    int validateProgram();
    int convertTextToProgram();
    uint8_t convertCharToRealHexValue(uint8_t value);
    void reverseBytesInWord(uint32_t &word);
    
    void allocateMemoryForVirtualMachine();
    void freeMemoryFromVirtualMachine();
    void setMode(uint8_t mode){
        this->mode = mode;
    }

    uint32_t getWordFromMemory(int number);
    uint32_t getWordFromMemory(){
        int i = pc / 16;
        int j = pc % 16; 

        return userMemory[(ptr*16+i) * 16 + j]; //tikiuos veiks XDD
        //uint32_t number = userMemory[ptr*16+i]
        //return userMemory[number*16+j];
        
    }
    uint32_t getNextWord();
    uint32_t getPtr(){
        return ptr;
    }

    VirtualMachineRegisters getRealMachineRegisterPointers(){
        return VirtualMachineRegisters(rax, rbx, ds, cs, pc, sf);
    }

    bool isInterruptNeeded(){
        if(si + pi > 0 || ti == 0){ //
            return true;
        }
        return false;
    }

    void saveWordToMemoryFromAx(int number);
    void saveWordToMemoryFromBx(int number);

    void testavimui();

    int test_();
};

#endif
