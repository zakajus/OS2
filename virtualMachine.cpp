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
};
