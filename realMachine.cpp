#include <iostream>

#include "virtualMachine.h"

class realMachine{
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
        virtualMachine virtualiMasina;
        uint32 userMemory[1632];
        uint32 supervisorMemory[512]; //32 blokai po 16 žodžių
        int userMemoryBlocks[102]; //0 - niekam nepriklauso, 1-6, kurios virtualios masinos blokas
    public:
        realMachine(/* args */);
        ~realMachine();
        translateLocalAdrressToRealAdrress();
        void test_(){
            if(SI > 0){
                switch (SI):
                    case 1:
                        //HALT
                        break;
                    case 2:
                        //READ
                        break;
                    case 3:
                        //PNUM
                        break;
                    case 4:
                        //PTXT
                        break;
                    case 5:
                        //EXEx
                        break;
                    default:
                        break;
                
            }
            if(PI > 0){
                switch (PI):
                    case 1:
                        //wrong address
                        break;
                    case 2:
                        //wrong operation code
                        break;
                    case 3:
                        //wrong inicialization/assignment
                        break;
                    case 4:
                        // division from 0
                        break;
                    default:
                        break;
                
            }
            if(TI == 0){
                //change to other program
            }
        }
};
