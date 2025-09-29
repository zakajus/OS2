#include <iostream>

class VirtualMachine
{
    private:
        uint32* rax;
        uint32* rbx;
        uint16* ds;
        uint16* cs;
        uint16* pc;
        uint8* sf;
        RealMachine* realMachine;
    public:
        VirtualMachine(uint32 &rax, uint32 &rbx,  uint16 &ds, uint16 &cs, uint16 &pc, uint8 &sf, RealMachine &realMachine){
            this->rax = &rax;
            this->rbx = &rbx;
            this->ds = &ds;
            this->cs = &cs;
            this->pc = &pc;
            this->sf = &sf;  
            this->realMachine = &realMachine;
        }
        ~VirtualMachine(){

        }

        void add(uint8 x, uint8 y);
        void substract(uint8 x, uint8 y);
        void multiply(uint8 x, uint8 y);
        void divide(uint8 x, uint8 y);
        void compare(){
            if(rax > rbx){
                //CF = 0, ZF = 0
            }
            else if(rax < rbx){
                //CF = 1
            }
            else{ //rax = rbx
                //ZF = 0
            }

        }
        void and_();
        void or_();
        void not_();
        void jumpZero(uint8  x, uint8  y);
        void jumpNotCarry(uint8 x, uint8 y); //pagalvot ar nereiks pakeist i jump not zero?
        void jumpBelow(uint8 x, uint8 y);
        void jumpAbove(uint8 x, uint8 y);
        void moveToAX(uint8 x, uint8 y){
            int realAddress = realMachine.translateLocalAdrressToRealAddress(x, y);
            rax = getWordFromMemory(realAddress);
        }
        void moveToBX(uint8 x, uint8 y){
            int realAddress = realMachine.translateLocalAdrressToRealAddress(x, y);
            rbx = getWordFromMemory(realAddress);
        }
        void saveFromAX(uint8 x, uint8 y){ //patikrint
            int realAddress = realMachine.translateLocalAdrressToRealAddress(x, y);
            realMachine.saveWordToMemory(realAddress, rax);
        }
        void saveFromBX(uint8 x, uint8 y){ //patikritn
            int realAddress = realMachine.translateLocalAdrressToRealAddress(x, y);
            realMachine.saveWordToMemory(realAddress, rbx);
        }
        void execute(uint8 x);
        void runNextCommand();
        void readFromKeyboard();
        void printNumber();
        void printText();


};
