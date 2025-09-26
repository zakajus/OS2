#include <iostream>


class realMachine
{
    private:
        uint32 RAX;
        uint32 RBX;
        uint8 MODE;
        uint16 DS;
        uint16 CS;
        uint16 PC;
        uint16 TI;
        uint8 DI;
        uint8 SI;
        uint8 SF;
        uint32 PTR;
    public:
        realMachine(/* args */);
        ~realMachine();
};
