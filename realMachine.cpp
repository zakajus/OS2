#include <iostream>
#include <ctime>
#include <vector>
#include <cstdint>

#include "virtualMachine.h"

using namespace std;

class realMachine{
    private:
        uint32_t rax;
        uint32_t rbx;
        uint8_t mode;
        uint16_t ds;
        uint16_t cs;
        uint16_t pc;
        uint16_t ti;
        uint8_t di;
        uint8_t si;
        StatusFlag sf;
        uint32_t ptr;
        virtualMachine virtualiMasina;
        uint32_t userMemory[1632]; //102 blokai po 16 žodžių
        uint32_t supervisorMemory[512]; //32 blokai po 16 žodžių 
    
        vector<int> freeBlocks;
        vector<int> occupiedBlocks;
    public:
        realMachine(/* args */){
            for (int i = 0; i <= 101; ++i) {
                freeBlocks.push_back(i);
            }
        }
        ~realMachine();
        int translateLocalAdrressToRealAddress(uint8_t x, uint8_t y){
            uint32_t pageTable[16] = userMemory[PTR*16];
            int realAddress = pageTable[x] * 16 + y;
            return realAddress;
        }

        void printAllRegisterValues();
        void printCurrentPage();
        void printVirtualMemory();
        void printRealMemory();

//Vartotojo sąsajoje turi būti komandų atlikimo demonstracija bei visų VM komponentų būsenų kaita vykdant programą žingsniniu režimu.
//Registrų reikšmės.
//Sekanti vykdoma komanda.
//Išorinių įrenginių būsenos.
//Vykdomos komandos VM puslapio reikšmės.


        void allocateMemoryForVirtualMachine(){
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

        uint32_t getWordFromMemory(int number){
            return userMemory[number];
        }

        uint32_t getNextWord(){
            //implementuoooot
            return 0;
        }

        void saveWordToMemoryFromAx(int number){ //sita paziuret ar gerai
            userMemory[number] = rax;
        }

        void saveWordToMemoryFromBx(int number){ //sita paziuret ar gerai
            userMemory[number] = rbx;
        }

        void test_(){
            //pries iskvieciant paprogrames issaugoti registru reiksmes kazkur
            if(si > 0){
                switch (si):
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
            if(pi > 0){
                switch (pi):
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
            if(ti == 0){
                //change to other program
            }
            //atstatyti registru reiksmes
        }
};
