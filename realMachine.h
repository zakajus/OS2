#ifndef realMachine_h
#define realMachine_h

#include <iostream>
#include <ctime>
#include <vector>
#include <cstdint>


class ChannelDevice;
#include "virtualMachine.h"

using namespace std;

class RealMachine{
    private:
        uint32_t rax;
        uint32_t rbx;
        uint8_t mode;
        uint16_t ds;
        uint16_t cs;
        uint16_t pc;
        uint16_t ti;
        uint8_t pi;
        uint8_t si;
        StatusFlag sf;
        uint32_t ptr;
        VirtualMachine* virtualMachine;
		ChannelDevice* channelDevice;
        uint32_t userMemory[1632]; //102 blokai po 16 žodžių
        uint32_t supervisorMemory[512]; //32 blokai po 16 žodžių 
    
        vector<int> freeBlocks;
        vector<int> occupiedBlocks;
    public:
        RealMachine(Monitor& monitor, Keyboard& keyboard);
        int translateLocalAdrressToRealAddress(uint8_t x, uint8_t y);

        void printAllRegisterValues();
        void printCurrentPage();
        void printVirtualMemory();
        void printRealMemory();

        void changeSI(int i);
        void changePI(int i);


        void allocateMemoryForVirtualMachine();

        uint32_t getWordFromMemory(int number);

        uint32_t getNextWord();

        void saveWordToMemoryFromAx(int number);
        void saveWordToMemoryFromBx(int number);
        void nextStep();

        void test_();
};

#endif