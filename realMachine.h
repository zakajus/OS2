#ifndef realMachine_h
#define realMachine_h

#include <iostream>
#include <ctime>
#include <vector>
#include <cstdint>
#include <iomanip>

#include "keyboard.h"
#include "monitor.h"


class ChannelDevice;
#include "virtualMachine.h"

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
    void changeSI(uint8_t i);
    void changePI(uint8_t i);

    void rm_run();
    int validateProgram();
    int convertTextToProgram();

    static void reverseBytesInWord(uint32_t &word);
    static uint8_t convertCharToRealHexValue(uint8_t value);

    void allocateMemoryForVirtualMachine();
    void freeMemoryFromVirtualMachine();
    void testas2();

    uint32_t getWordFromMemory(int number);

    uint32_t getNextWord();

    void saveWordToMemoryFromAx(int number);

    void saveWordToMemoryFromBx(int number);

    void printAsASCII(uint32_t *memory, int size);

    void testavimui();

    int test_();
};

#endif
