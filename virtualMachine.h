#ifndef virtualMachine_h
#define virtualMachine_h
#include <iostream>
#include <cstdint>

// Dar nenaudoju nes kazkoki errora meta, bet paliekus
enum class Opcode : uint32_t {
    // 4-byte opcodes
    HALT = 0x48414C54,
    CMP  = 0x434D505F,
    AND  = 0x414E445F,
    OR   = 0x4F525F5F,
    NOT  = 0x4E4F545F,
    READ = 0x52454144,
    PNUM = 0x504E554D,
    PTXT = 0x50545854,

    // 2-byte opcodes
    AD = 0x4144,
    SU = 0x5355,
    MU = 0x4D55,
    DI = 0x4449,
    JZ = 0x4A5A,
    JN = 0x4A4E,
    JB = 0x4A42,
    JA = 0x4A41,
    JP = 0x4A50,
    MA = 0x4D41,
    MB = 0x4D42,
    SA = 0x5341,
    SB = 0x5342,
    EX = 0x4558
};

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

        void add(uint8_t x, uint8_t y) const;
        void substract(uint8_t x, uint8_t y) const;
        void multiply(uint8_t x, uint8_t y) const;
        void divide(uint8_t x, uint8_t y) const;
        void compare() const;
        void and_() const;
        void or_() const;
        void not_() const;
        void jump(uint8_t x, uint8_t y) const;
        void jumpZero(uint8_t x, uint8_t y) const;
        void jumpNotCarry(uint8_t x, uint8_t y) const; //pagalvot ar nereiks pakeist i jump not zero?
        void jumpBelow(uint8_t x, uint8_t y) const;
        void jumpAbove(uint8_t x, uint8_t y) const;
        void moveToAX(uint8_t x, uint8_t y) const;
        void moveToBX(uint8_t x, uint8_t y) const;
        void saveFromAX(uint8_t x, uint8_t y) const ;
        void saveFromBX(uint8_t x, uint8_t y) const;
        void execute(uint8_t x) const;
        void runNextCommand(uint32_t) const;
        void readFromKeyboard() const;
        void printNumber() const;
        void printText() const;
        void _halt() const;

};

#endif