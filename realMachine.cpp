#include "realMachine.h"
#include "channelDevice.h"
#include <algorithm>
using namespace std;

RealMachine::RealMachine(Monitor& monitor, Keyboard& keyboard) 
    : rax(0), rbx(0), mode(0), ds(0), cs(0), pc(0), ti(0), pi(0), si(0), ptr(0), virtualMachine(nullptr) {

    memset(userMemory, 0, sizeof(userMemory));
    memset(supervisorMemory, 0, sizeof(supervisorMemory));

    channelDevice = new ChannelDevice(this, userMemory, supervisorMemory, &monitor, &keyboard);
    
    for (int i = 0; i < 102; i++) {
        freeBlocks.push_back(i);
    }
}

void printAsHex(uint32_t* memory, int size) {
    cout << "=== HEXADECIMAL ===" << endl;
    for (int i = 0; i < size; i++) {
        cout << "0x" << hex << setw(8) << setfill('0') << memory[i] << " ";
        if ((i + 1) % 4 == 0) cout << endl; // 4 words per line
    }
    cout << dec << endl; // Reset to decimal
}

// Print as ASCII (interpreting bytes as characters)
void printAsASCII(uint32_t* memory, int size) {
    cout << "=== ASCII ===" << endl;
    uint8_t* bytes = (uint8_t*)memory;
    int totalBytes = size * 4; // 4 bytes per uint32_t
    
    for (int i = 0; i < totalBytes; i++) {
        if(i % 64 == 0){
            cout << endl  << i / 64 << " ";
        }
        if (bytes[i] >= 32 && bytes[i] <= 126) { // Printable ASCII
            cout << (char)bytes[i];
        } else if (bytes[i] == 10) { // Newline
            cout << endl;
        } else {
            cout << '.'; // Non-printable characters
        }
    }
    cout << endl;
}

void RealMachine::testavimui(){
    channelDevice->setST(3); //is isorines
    channelDevice->setDT(2); //i supervizorine
    channelDevice->setNAME(0x315A5650); //pakeist i tikra pavadinima
    channelDevice->setRNUM(256); //16
    channelDevice->setOFF(0);
    channelDevice->setDB(0);
    channelDevice->setSB(0);
    channelDevice->xchg();

    //printAsHex(supervisorMemory, 256);
    // printAsASCII(supervisorMemory, 256);
    // printAsHex(supervisorMemory, 256);
    convertTextToProgram();
    // printAsASCII(supervisorMemory, 256);
    // printAsHex(supervisorMemory, 256);

    allocateMemoryForVirtualMachine();
    //ptr dabar rodo i puslapiu lentele
    channelDevice->setST(2); //is supervizorines
    channelDevice->setDT(1); //i vartotojo
    channelDevice->setRNUM(16);

    virtualMachine = new VirtualMachine(rax, rbx, ds, cs, pc, sf, *this);
    uint32_t pageTable[16];
    
    for(int i = 0; i < 16; i++){
        pageTable[i] = userMemory[ptr * 16 + i];
    }


    for(int i = 0; i < 16; ++i){ //cia galimai reiks pakeist kad nebekopijuotu ten kkur nieko nebera
        cout << endl;
        channelDevice->setSB(i); //is kurio takelio kopijuojam
        channelDevice->setDB(pageTable[i]); //i kuri takeli kopijuojam
        cout << pageTable[i] << endl;
        channelDevice->xchg();
    }

    if(test_() != 0){
            cout << "interupttas po perkopijavmo" << endl;
        }

    pi = 0;
    
    printAsASCII(userMemory, 1632);

    int kazkas = 0;
    pc = 17;
    while(1){
        int i = pc / 16;
        int j = pc % 16;
        
        uint32_t command = userMemory[pageTable[i]*16+j];
        cout << "0x" << std::hex << command << std::dec << endl;
        command = ((command & 0x000000FF) << 24) |  // Byte 0 -> Byte 3
          ((command & 0x0000FF00) << 8)  |  // Byte 1 -> Byte 2
          ((command & 0x00FF0000) >> 8)  |  // Byte 2 -> Byte 1
          ((command & 0xFF000000) >> 24);
        cout << "0x" << std::hex << command << std::dec << endl;
        ++pc;
        virtualMachine->runNextCommand(command);
        cout << "Praejo komanda??" << endl;
        if(test_() != 0){
            cout << "interupttas ye" << endl;
            si = 0;
            pi = 0;
            break;
        }
        ++kazkas;
        if(kazkas == 50){
            break;
        }
        
    }
    
}



bool isValidHexDigit(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

bool isValidInstruction(const char* word) {
    if (word[0] == '$' && isValidHexDigit(word[1]) && 
        word[2] == '0' && word[3] == '0') {
        return true;
    }

    // if(word[0] == '$' && word[1] == 'x' && isValidHexDigit(word[2]) && isValidHexDigit(word[3])){
    //     return true;
    // }

    // if(isValidHexDigit(word[0]) && isValidHexDigit(word[1]) && isValidHexDigit(word[2]) && isValidHexDigit(word[3])){
    //     return true;
    // }
    
    const char* twoParamOps[] = {
        "JP", "JZ", "JN", "JB", "JA", 
        "MA", "MB", "SA", "SB",
        "SU", "MU", "DI"
    };
    
    for (const char* op : twoParamOps) {
        if (word[0] == op[0] && word[1] == op[1] && 
            isValidHexDigit(word[2]) && isValidHexDigit(word[3])) {
            return true;
        }
    }
    
    if (word[0] == 'E' && word[1] == 'X' && 
        word[2] == 'E' && isValidHexDigit(word[3])) {
        return true;
    }
    const char* zeroParamOps[] = {
        "READ", "PNUM", "PTXX", 
        "AND_", "OR__", "NOT_", 
        "HALT", "CMP_"
    };
    
    for (const char* op : zeroParamOps) {
        if (memcmp(word, op, 4) == 0) {
            return true;
        }
    }
    
    return false;
}

uint8_t convertCharToRealHexValue1(uint8_t value){
    if(value >= 48 && value <= 57){
        value -= 48;
        return value;
    }
    if(value >= 65 && value <= 70){
        value -= 65;
        return value;
    }
    if(value >= 97 && value <= 102){
        value -= 97;
        return value;
    }
}

int RealMachine::convertTextToProgram(){
    uint32_t newMemory[256] = {0};
    uint32_t firstWord = supervisorMemory[0];
    uint8_t x, y;
    
    if (memcmp(&firstWord, "$AMJ", 4) != 0) {
        return -1; 
    }
    for(int i = 1; i < 256; ++i){
        uint32_t instruction = supervisorMemory[i];
        uint8_t* word = (uint8_t*)&instruction;
        char* wordTocheckInstruction = (char*)&instruction;
        if(word[0] == '$' && word[1] == 'x' && isValidHexDigit(word[2]) && isValidHexDigit(word[3])){
            //word yra char, tai jei noriu skaiciaus reik pakeist
            //kazkodel sitas ne visada istrina bet pradinius visailaik tuos $x30 pvz pasalina
            //cout << word[2] << " " << word[3] << endl;
            x = convertCharToRealHexValue1(word[2]);
            y = convertCharToRealHexValue1(word[3]);
            continue;
        }

        //ideti i x ir y vieta komanda/skaiciu
        if(isValidHexDigit(word[0]) && isValidHexDigit(word[1]) && isValidHexDigit(word[2]) && isValidHexDigit(word[3])){
            word[0] = convertCharToRealHexValue1(word[0]);
            word[1] = convertCharToRealHexValue1(word[1]);
            word[2] = convertCharToRealHexValue1(word[2]);
            word[3] = convertCharToRealHexValue1(word[3]);

            uint32_t value = (static_cast<uint32_t>(word[0]) << 24) | 
                     (static_cast<uint32_t>(word[1]) << 16) | 
                     (static_cast<uint32_t>(word[2]) << 8)  | 
                     (static_cast<uint32_t>(word[3]));
    
            newMemory[x*16 + y] = value;
            //cout << "0x" << hex << setw(8) << setfill('0') << value << " ";
            
        }
        else if(isValidInstruction(wordTocheckInstruction)){
            newMemory[x*16 + y] = instruction;
        }
        else if (i < 48){ //nuo 33 iki 126 gali but characteriai kur tiesiog tekstines kontantos
            int o = 0;
            for(int j = 0; j < 4; ++j){
                if(word[j] >= 32 && word[j] <=126){
                    ++o;
                }
            }
            //cout << instruction << endl;
            //cout << x << endl;
            if(o == 4){
                newMemory[x*16 + y] = instruction;
            }
            
        }
        ++y;
        if(y > 16){
            y = 0; 
            ++x;
        }
    }
    memcpy(supervisorMemory, newMemory, 256);
}

int RealMachine::validateProgram() {
    uint32_t firstWord = supervisorMemory[0];
    
    if (memcmp(&firstWord, "$AMJ", 4) != 0) {
        return -1; 
    }
    
    for (int i = 1; i < 256; i++) { //pakeist ta 256 i kintamaji
        uint32_t word = supervisorMemory[i];
        char* instruction = (char*)&word;
        
        if (!isValidInstruction(instruction)) {
            return i;
        }
    }
    
    return 0; 
}

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
    if(validateProgram() != 0){
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

    while(1){
        int i = pc / 16;
        int j = pc % 16;
        uint32_t command = userMemory[pageTable[i]+j];
        cout << "Komanda: " << command << endl;
        ++pc;
        virtualMachine->runNextCommand(command);
        if(test_() != 0){
            break;
        }
    }
}

uint32_t RealMachine::translateLocalAdressToRealAddress(uint8_t x, uint8_t y){
    uint32_t pageTable[16] = {0};
    
    for(int i = 0; i < 16; i++){
        pageTable[i] = userMemory[ptr * 16 + i];
        cout << pageTable[i] << endl;
    }
    
    cout << "x " << (int)x << endl;  // Cast to int for printing
    uint32_t value = x;  // This works fine
    cout << value << endl;

    cout << "pagetable[1] " << pageTable[1] << endl; 
    cout << "page table[x] " << pageTable[value] << " y " << y << endl;
    uint32_t realAddress = pageTable[x] * 16 + y;
    
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

void RealMachine::changeSI(uint8_t i){
    cout << "jehe" << endl;
    si = i;
    cout << "Jei cia tai si pasikeite i  "  << i << endl;
}

void RealMachine::changePI(uint8_t i){
    //cout << "Alioo"
    pi = i;
    cout << "Jei cia tai pi pasikeite i " << i << endl;
}


void RealMachine::allocateMemoryForVirtualMachine(){
    int temp[17];
    srand(time(0));
    int randomIndex;
    for(int i = 0; i < 17; ++i){ //tikiuos dabar bus ok
        randomIndex = rand() % freeBlocks.size();
        uint32_t newPageNumber = freeBlocks.at(randomIndex);
        occupiedBlocks.push_back(newPageNumber);
        temp[i] = newPageNumber;
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
    cout << "hello how are u " << endl;
    if(si > 0){
        switch (si){
            case 1:{
                //HALT
                return 1; //paleist kita programa arba baigt darba
            }
                
            case 2:{
                //READ
                channelDevice->setDT(4); // 4 - kopijuojam i rbx
                channelDevice->setST(4); // 4- kopijuojam is klaviaturos
                channelDevice->setRNUM(1);
                channelDevice->xchg();
                rbx = channelDevice->getReg();
                break;
            }
                
            case 3:{
                //PNUM
                channelDevice->setReg(rbx);
                channelDevice->setDT(3); // 3 -isvedimo srautas
                channelDevice->setST(5); // 5 - kopijuojam is RBX
                channelDevice->setRNUM(1);
                channelDevice->setIsNumber(1);
                channelDevice->xchg();
                break;
            }
                
            case 4:{
                //PTXT
                cout << "Nuo cia iki kito cout klaida"<< endl;
                int x = pi;
                cout << "Pi " << x << endl;
                channelDevice->setReg(rbx);
                channelDevice->setDT(3); // 3 -isvedimo srautas
                channelDevice->setST(5); // 5 - kopijuojam is RBX
                channelDevice->setRNUM(1);
                channelDevice->setIsNumber(0);
                channelDevice->xchg();
                x = pi;
                cout << "Pi " << x << endl;
                cout << "Channel device xchng ivyko" << endl;
                //break;
                return 0;
            }
                
            case 5:{
                //EXEx
                //rbx failo pavadinimas
                //x - parametru blokas
                //paleidziam nauja programa
                break;
            }
                
            default:
                break;
        }
    }
    if(pi > 0){
        switch (pi){ //galima arba ignoruot arba isjungt viska lauk ir pritnit viska kas ne taip
            case 1:{
                cout << "Address out of bounds." << endl;
                return -1;
            }
                
            case 2:{
                cout << "Wrong operation code." << endl;
                return -1;
            }
                
            case 3:{
                cout << "Wrong inicialization or assigment. " << endl;
                return -1;
            }
                
            case 4:{
                cout << "Division from 0." << endl;
                return -1;
            }
                
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