#include "realMachine.h"
#include "channelDevice.h"
#include "hardDisk.h"

using namespace std;

RealMachine::RealMachine(Monitor& monitor, Keyboard& keyboard, HardDisk& hardDisk) 
    : rax(0), rbx(0), mode(0), ds(0), cs(0), pc(0), ti(0), pi(0), si(0), ptr(0), virtualMachine(nullptr) {

    memset(userMemory, 0, sizeof(userMemory));
    memset(supervisorMemory, 0, sizeof(supervisorMemory));

    channelDevice = new ChannelDevice(
        this, userMemory, supervisorMemory, &monitor, &keyboard, &hardDisk);
    
    for (int i = 0; i < 102; i++) {
        freeBlocks.push_back(i);
    }
}

int RealMachine::translateLocalAdrressToRealAddress(uint8_t x, uint8_t y){
    uint32_t pageTable[16];
    
    for(int i = 0; i < 16; i++){
        pageTable[i] = userMemory[ptr * 16 + i];
    }
    
    int realAddress = pageTable[x] * 16 + y;
    return realAddress;
}

void RealMachine::printAllRegisterValues(){
    cout << "RAX: " << rax << " RBX: " << rbx << " MODE: " << mode << " DS: " << ds << " CS: " << cs << " PC: " << pc << " TI: " << ti << " DI: " << pi << " SI: " << si << " PTR: " << ptr << endl;
    cout << "Status flag: CF: " << sf.cf << " OF: " << sf.of << " AF: " << sf.af << " ZF: " << sf.zf << endl;
}
void RealMachine::printCurrentPage(){

}
void RealMachine::printVirtualMemory(){

}
void RealMachine::printRealMemory(){

}

void RealMachine::changeSI(int i){
    si = i;
}

void RealMachine::changePI(int i){
    pi = i;
}


void RealMachine::allocateMemoryForVirtualMachine(){
    int temp[17];
    srand(time(0));
    int randomIndex;
    for(int i = 0; i < 17; ++i){
        randomIndex = rand() % freeBlocks.size();
        occupiedBlocks.push_back(randomIndex);
        temp[i] = randomIndex;
        freeBlocks[randomIndex] = freeBlocks.back();
        freeBlocks.pop_back();
    }
}

uint32_t RealMachine::getWordFromMemory(int number){
    return userMemory[number];
}

uint32_t RealMachine::getNextWord(){
    //implementuoooot
    return 0;
}

void RealMachine::saveWordToMemoryFromAx(int number){ //sita paziuret ar gerai
    userMemory[number] = rax;
}

void RealMachine::saveWordToMemoryFromBx(int number){ //sita paziuret ar gerai
    userMemory[number] = rbx;
}

int RealMachine::test_(){
    //pries iskvieciant paprogrames issaugoti registru reiksmes kazkur
    if(si > 0){
        switch (si){
            case 1:
                //HALT
                return 1; //paleist kita programa arba baigt darba
            case 2:
                //READ
                channelDevice->setDT(4); // 4 - kopijuojam i rbx
                channelDevice->setST(4); // 4- kopijuojam is klaviaturos
                channelDevice->setRNUM(4);
                channelDevice->xchg();
                rbx = channelDevice->getReg();
                break;
            case 3:
                //PNUM
                channelDevice->setReg(rbx);
                channelDevice->setDT(3); // 3 -isvedimo srautas
                channelDevice->setST(5); // 5 - kopijuojam is RBX
                channelDevice->setRNUM(4);
                channelDevice->setIsNumber(1);
                channelDevice->xchg();
                break;
            case 4:
                //PTXT
                channelDevice->setReg(rbx);
                channelDevice->setDT(3); // 3 -isvedimo srautas
                channelDevice->setST(5); // 5 - kopijuojam is RBX
                channelDevice->setRNUM(4);
                channelDevice->setIsNumber(0);
                channelDevice->xchg();
                break;
            case 5:
                //EXEx
                //rbx failo pavadinimas
                //x - parametru blokas
                //paleidziam nauja programa
                break;
            default:
                break;
        }
    }
    if(pi > 0){
        switch (pi){ //galima arba ignoruot arba isjungt viska lauk ir pritnit viska kas ne taip
            case 1:
                cout << "Address out of bounds." << endl;
                return -1;
            case 2:
                cout << "Wrong operation code." << endl;
                return -1;
            case 3:
                cout << "Wrong inicialization or assigment. " << endl;
                return -1;
            case 4:
                cout << "Division from 0." << endl;
                return -1;
            default:
                break;
        }
        
    }
    if(ti == 0){
        //change to other program
    }
    //atstatyti registru reiksmes
}