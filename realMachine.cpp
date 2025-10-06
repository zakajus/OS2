#include <iostream>
#include <ctime>
#include <vector>

#include "virtualMachine.h"

using namespace std;

class realMachine{
    private:
        uint32 rax;
        uint32 rbx;
        uint8 mode;
        uint16 ds;
        uint16 cs;
        uint16 pc;
        uint16 ti;
        uint8 di;
        uint8 si;
        StatusFlag sf;
        uint32 ptr;
        VirtualMachine virtualMachine;
        ChannelDevice channelDevice;
        uint32 userMemory[1632]; //102 blokai po 16 žodžių
        uint32 supervisorMemory[512]; //32 blokai po 16 žodžių 
    
        vector<int> freeBlocks;
        vector<int> occupiedBlocks;
    public:
        realMachine(/* args */){
            for (int i = 0; i <= 101; ++i) {
                freeBlocks.push_back(i);
            }
        }
        ~realMachine();
        int translateLocalAdrressToRealAddress(uint8 x, uint8 y){
            uint32 pageTable[16] = userMemory[PTR*16];
            int realAddress = pageTable[x] * 16 + y;
            return realAddress;
        }

        void printAllRegisterValues(){
            cout << "RAX: " << rax << " RBX: " << rbx << " MODE: " << mode << " DS: " << ds << " CS: " << cs << " PC: " << pc << " TI: " << ti << " DI: " << di << " SI: " << si << " PTR: " << ptr << endl;
            cout << "Status flag: CF: " << sf.cf << " PR: " << sf.pr << " AF: " << sf.af << " ZF: " << sf.zf << endl;

        }
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

        uint32 getWordFromMemory(int number){
            return userMemory[number];
        }

        uint32 getNextWord(){
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
