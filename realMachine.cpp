#include <iostream>
#include <ctime>
#include <vector>

#include "virtualMachine.h"

using namespace std;

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
        uint32 userMemory[1632]; //102 blokai po 16 žodžių
        uint32 supervisorMemory[512]; //32 blokai po 16 žodžių 
        bool isUserMemoryBlockOccupied[102]; // true - užimtas, false - laisvas

        vector<int> freeBlocks;
        vector<int> occupiedBlocks;
    public:
        realMachine(/* args */){
            for (int i = 0; i <= 101; ++i) {
                freeBlocks.push_back(i);
            }
        }
        ~realMachine();
        translateLocalAdrressToRealAdrress();

        void allocateMemoryForVirtualMachineVectors(){
            int temp[17];
            srand(time(0));
            int randomNumber;
            for(int i = 0; i < 17; ++i){
                randomNumber = rand() % freeBlocks.size();
                occupiedBlocks.push_back(randomNumber);
                temp[i] = randomNumber;
                freeBlocks.erase( remove(freeBlocks.begin(), freeBlocks.end(), randomNumber), freeBlocks.end());
            }
        }

        void allocateMemoryForVirtualMachine(){
            //ar geriau daryt du vektorius, kuriu vienas pradzioje tuscias, kitame skaiciai nuo 0 iki 101 ir tik is jo renkames blokus
            int temp[17];
            srand(time(0));
            int randomNumber;
            for(int i = 0; i < 17; ++i){
                randomNumber = rand() % 102;
                if(!isUserMemoryBlockOccupied[randomNumber]){
                    isUserMemoryBlockOccupied[randomNumber] = true;
                    temp[i] = randomNumber;
                }
                else{
                    --i;
                }
            }  
        }

        

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
