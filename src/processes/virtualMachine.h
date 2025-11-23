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
    //loginimo flagas arba klassee
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
        uint32_t memory[256];
    public:
        VirtualMachine(uint32_t &rax, uint32_t &rbx,  uint16_t &ds, uint16_t &cs, uint16_t &pc, StatusFlag &sf, RealMachine &realMachine);


        uint16_t getPc(){
            return *pc;
        }
        void add(uint8_t x, uint8_t y) ;
        void substract(uint8_t x, uint8_t y) ;
        void multiply(uint8_t x, uint8_t y) ;
        void divide(uint8_t x, uint8_t y) ;
        void compare() ;
        void and_() ;
        void or_() ;
        void not_() ;
        void jump(uint8_t x, uint8_t y) ;
        void jumpZero(uint8_t x, uint8_t y) ;
        void jumpNotCarry(uint8_t x, uint8_t y) ; 
        void jumpBelow(uint8_t x, uint8_t y) ;
        void jumpAbove(uint8_t x, uint8_t y) ;
        void moveToAX(uint8_t x, uint8_t y) ;
        void moveToBX(uint8_t x, uint8_t y) ;
        void saveFromAX(uint8_t x, uint8_t y)  ;
        void saveFromBX(uint8_t x, uint8_t y) ;
        void execute(uint8_t x) ;
        void runNextCommand(uint32_t);
        void readFromKeyboard() ;
        void printNumber() ;
        void printText() ;
        void _halt() ;

};

#endif