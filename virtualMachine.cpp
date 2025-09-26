#include <iostream>

class virtualMachine
{
    private:
        uint32 RAX; //ar cia pointeriai ar ne
        uint32 RBX;
        uint16 DS;
        uint16 CS;
        uint16 PC;
        uint8 SF;
    public:
        virtualMachine(/* args */);
        ~virtualMachine();
        add(int x, int y);
        substract(int x, int y);
        multiply(int x, int y);
        divide(int x, int y);
        compare();
        and();
        or();
        not();
        jumpZero(int x, int y);
        jumpNotCarry(int x, int y); //pagalvot ar nereiks pakeist i jump not zero?
        jumpBelow(int x, int y);
        jumpAbove(int x, int y);
        moveToAX(int x, int y);
        moveToBX(int x, int y);
        saveFromAX(int x, int y);
        saveFromBX(int x, int y);
        execute(int x);
        readFromKeyboard();
        printNumber();
        printText();


};
