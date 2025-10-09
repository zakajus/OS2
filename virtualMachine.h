#ifndef virtualMachine_h
#define virtualMachine_h
#include <iostream>
#include <cstdint>


class RealMachine;

struct StatusFlag{
    // one-bit unsigned field, allowed values are 0-1
    unsigned int cf : 1; //carry flag
    unsigned int of : 1; //overflow flag
    unsigned int af : 1; //auxiliary flag
    unsigned int zf : 1; //zero flag
};

class VirtualMachine
{
    private:
        uint32_t* rax;
        uint32_t* rbx;
        uint16_t* ds;
        uint16_t* cs;
        uint16_t* pc;
        StatusFlag* sf;
        RealMachine* realMachine;
    public:
        VirtualMachine(uint32_t &rax, uint32_t &rbx,  uint16_t &ds, uint16_t &cs, uint16_t &pc, StatusFlag &sf, RealMachine &realMachine);

        void add(uint8_t x, uint8_t y);
        void substract(uint8_t x, uint8_t y);
        void multiply(uint8_t x, uint8_t y);
        void divide(uint8_t x, uint8_t y);
        void compare();
        void and_();
        void or_();
        void not_();
        void jumpZero(uint8_t x, uint8_t y);
        void jumpNotCarry(uint8_t x, uint8_t y); //pagalvot ar nereiks pakeist i jump not zero?
        void jumpBelow(uint8_t x, uint8_t y);
        void jumpAbove(uint8_t x, uint8_t y);
        void moveToAX(uint8_t x, uint8_t y);
        void moveToBX(uint8_t x, uint8_t y);
        void saveFromAX(uint8_t x, uint8_t y);
        void saveFromBX(uint8_t x, uint8_t y);
        void execute(uint8_t x);
        void runNextCommand();
        void readFromKeyboard();
        void printNumber();
        void printText();
        void _halt();

};

#endif