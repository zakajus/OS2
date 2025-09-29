#include <iostream>

class virtualMachine
{
    private:
        uint32* rax;
        uint32* rbx;
        uint16* ds;
        uint16* cs;
        uint16* pc;
        uint8* sf;
    public:
        virtualMachine(/* args */);
        ~virtualMachine();

        void add(int x, int y);
        void substract(int x, int y);
        void multiply(int x, int y);
        void divide(int x, int y);
        void compare();
        void and_();
        void or_();
        void not_();
        void jumpZero(int x, int y);
        void jumpNotCarry(int x, int y); //pagalvot ar nereiks pakeist i jump not zero?
        void jumpBelow(int x, int y);
        void jumpAbove(int x, int y);
        void moveToAX(int x, int y);
        void moveToBX(int x, int y);
        void saveFromAX(int x, int y);
        void saveFromBX(int x, int y);
        void execute(int x);
        void runNextCommand();
        void readFromKeyboard();
        void printNumber();
        void printText();


};
