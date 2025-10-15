#include "realMachine.h"
#include "channelDevice.h"
#include "hardDisk.h"
#include <algorithm>
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

int RealMachine::validateProgram(){
    //eit per supervizorine ir kazkaip tikrint
    return 0;
}

//nebaigta funkcija reik taisyt bet dabar smegenys neveikia
void RealMachine::rm_run(){
    channelDevice->setST(3); //is isorines
    channelDevice->setDT(2); //i supervizorine
    channelDevice->setNAME(0); //pakeist i tikra pavadinima
    channelDevice->setRNUM(256); //16
    channelDevice->setOFF(0);
    channelDevice->setDB(0);
    channelDevice->setSB(0);
    channelDevice->xchg();
    //ivykdyt validacija
    if(validateProgram() == 1){
        return;
    }


    allocateMemoryForVirtualMachine();
    //ptr dabar rodo i puslapiu lentele
    channelDevice->setST(2); //is supervizorines
    channelDevice->setDT(1); //i vartotojo
    channelDevice->setRNUM(16);

    uint32_t pageTable[16];
    
    for(int i = 0; i < 16; i++){
        pageTable[i] = userMemory[ptr * 16 + i];
    }

    for(int i = 0; i < 16; ++i){
        channelDevice->setSB(i); //is kurio takelio kopijuojam
        channelDevice->setDB(pageTable[i]); //i kuri takeli kopijuojam
        channelDevice->xchg();
    }
    pc = 0;
    //cs = 48; //0x30
    cs = 0; //?
    ds = 0;
    virtualMachine = new VirtualMachine(rax, rbx, ds, cs, pc, sf, *this);

    for(int i = 0; i < 16; ++i){
        for(int j = 0; j < 16; ++j){
            uint32_t command = userMemory[pageTable[i]+j];
            virtualMachine->runNextCommand(command);
            if(test_() != 0){
                break;
            }
        }
    }
}

int RealMachine::translateLocalAdressToRealAddress(uint8_t x, uint8_t y){
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
void RealMachine::printCurrentPage(int x){
    //x - virtualios masinos puslapis
    int realPageAddress = userMemory[ptr * 16 + x];
    for(int i = 0; i < 16; ++i){
        uint32_t word = userMemory[realPageAddress+i];
        printf("0x%08X\n", word);
    }
    cout << endl;
}
void RealMachine::printVirtualMemory(){
    for(int i = 0; i < 16; ++i){
        for(int j = 0; j < 16; ++j){
            int address = translateLocalAdressToRealAddress(i, j);
            uint32_t word = userMemory[address];
            printf("0x%08X\n", word);
        }
        cout << endl;
    }
}
void RealMachine::printRealMemory(){
    for(int i = 0; i < 102; ++i){
        for(int j = 0; j < 16; ++j){
            uint32_t word = userMemory[i*16+j];
            printf("0x%08X\n", word);
        }
    }
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
    ptr = temp[0];
    for(int i = 1; i < 17; ++i){
        userMemory[ptr*16 + i-1] = temp[i];
    }
}

void RealMachine::freeMemoryFromVirtualMachine(){
    uint32_t temp[16];
    for(int i = 0; i < 16; ++i){
        uint32_t valueToFree = userMemory[ptr*16 + i];
        freeBlocks.emplace_back(valueToFree);

        auto it = std::find(occupiedBlocks.begin(), occupiedBlocks.end(), valueToFree);
        if(it != occupiedBlocks.end()){
            occupiedBlocks.erase(it);
        }
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
    return 0;
}