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

void RealMachine::printAsHex(uint32_t* memory, int size) {
    cout << "=== HEXADECIMAL ===" << endl;
    for (int i = 0; i < size; i++) {
        cout << "0x" << hex << setw(8) << setfill('0') << memory[i] << " ";
        if ((i + 1) % 4 == 0) cout << endl; 
    }
    cout << dec << endl; 
}

void RealMachine::printAsASCII(uint32_t *memory, int size) {
    cout << "=== ASCII ===" << endl;
    uint8_t* bytes = (uint8_t*)memory;
    int totalBytes = size * 4; 
    
    for (int i = 0; i < totalBytes; i++) {
        if(i % 64 == 0){
            cout << endl  << i / 64 << " ";
        }
        if (bytes[i] >= 32 && bytes[i] <= 126) { 
            cout << (char)bytes[i];
        } else if (bytes[i] == 10) { 
            cout << endl;
        } else {
            cout << '.'; 
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
    //printAsASCII(supervisorMemory, 256);
    //printAsHex(supervisorMemory, 256);

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


    for(int i = 0; i < 16; ++i){
        channelDevice->setSB(i); 
        channelDevice->setDB(pageTable[i]); 
        channelDevice->xchg();
    }

    if(test_() != 0){
        cout << "interupttas po perkopijavmo" << endl;
    }

    pi = 0;
    pc = 48;
    while(1){
        int i = pc / 16;
        int j = pc % 16;
        
        uint32_t command = userMemory[pageTable[i]*16+j];
        reverseBytesInWord(command);
        // command = ((command & 0x000000FF) << 24) |  // Byte 0 -> Byte 3
        //   ((command & 0x0000FF00) << 8)  |  // Byte 1 -> Byte 2
        //   ((command & 0x00FF0000) >> 8)  |  // Byte 2 -> Byte 1
        //   ((command & 0xFF000000) >> 24);
        //cout << "0x" << std::hex << command << std::dec << endl;
       // cout << "Program counter: " << "0x" << std::hex << pc << std::dec << endl;
        ++pc;
        virtualMachine->runNextCommand(command);
        //printAllRegisterValues();
        //next - taisyt ka irasom i rbx, nes cia buvo tekstas - rbx irgi reik atvaizduto kaip tekst
        if(test_() != 0){
            //cout << "interupttas ye" << endl;
            si = 0;
            pi = 0;
            break;
        }
        si = 0;
    }
    
}

void RealMachine::reverseBytesInWord(uint32_t &word) {
    word = (word & 0x000000FF) << 24 | // Byte 0 -> Byte 3
        (word & 0x0000FF00) << 8 | // Byte 1 -> Byte 2
        (word & 0x00FF0000) >> 8 | // Byte 2 -> Byte 1
        (word & 0xFF000000) >> 24; // Byte 3 -> Byte 0
}

uint8_t RealMachine::convertCharToRealHexValue(uint8_t value) {
    if (value >= 48 && value <= 57) {
        value -= 48;
        return value;
    }
    if (value >= 65 && value <= 70) {
        value -= 55;
        return value;
    }
    if (value >= 97 && value <= 102) {
        value -= 87;
        return value;
    }
    return 0;
}


bool isValidHexDigit(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

bool isValidInstruction(const char* word) {
    if (word[0] == '$' && isValidHexDigit(word[1]) && 
        word[2] == '0' && word[3] == '0') {
        return true;
    }
    const char* twoParamOps[] = {
        "JP", "JZ", "JN", "JB", "JA", 
        "MA", "MB", "SA", "SB", "AD",
        "SU", "MU", "DI"};
    
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
        "READ", "PNUM", "PTXT", 
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

int RealMachine::convertTextToProgram(){
    uint32_t newMemory[256] = {0};
    uint32_t firstWord = supervisorMemory[0];
    uint8_t x = 0, y = 0;
    int isValidProgram = 0;
    
    if (memcmp(&firstWord, "$AMJ", 4) != 0) {
        ++isValidProgram;
        return -1; 
    }
    for(int i = 1; i < 256; ++i){
        uint32_t instruction = supervisorMemory[i];
        uint8_t* word = (uint8_t*)&instruction;
        char* wordTocheckInstruction = (char*)&instruction;
        if(word[0] == '$' && word[1] == 'x' && isValidHexDigit(word[2]) && isValidHexDigit(word[3])){
            x = convertCharToRealHexValue(word[2]);
            y = convertCharToRealHexValue(word[3]);
            continue;
        }

        //ideti i x ir y vieta komanda/skaiciu
        if(isValidInstruction(wordTocheckInstruction)){
            newMemory[x*16 + y] = instruction;
        }
        else if(isValidHexDigit(word[0]) && isValidHexDigit(word[1]) && isValidHexDigit(word[2]) && isValidHexDigit(word[3])){
            word[0] = convertCharToRealHexValue(word[0]);
            word[1] = convertCharToRealHexValue(word[1]);
            word[2] = convertCharToRealHexValue(word[2]);
            word[3] = convertCharToRealHexValue(word[3]);

            uint32_t value = (static_cast<uint32_t>(word[0]) << 12) |  (static_cast<uint32_t>(word[1]) << 8) | 
                     (static_cast<uint32_t>(word[2]) << 4)  |  (static_cast<uint32_t>(word[3]));
            newMemory[x*16 + y] = value;
            
        }
        
        else if(word[0] == '!' && word[1] == '!' && word[2] == '!' && word[3] == '!'){
            break;
        }
        else{ //nuo 33 iki 126 gali but characteriai kur tiesiog tekstines kontantos
            int o = 0;
            for(int j = 0; j < 4; ++j){
                if(word[j] >= 32 && word[j] <=126){
                    ++o;
                }
            }
            if(o == 4){
                newMemory[x*16 + y] = instruction;
            }
        }
        ++y;
        if(y == 16){
            y = 0; 
            ++x;
        }
    }
    memcpy(supervisorMemory, newMemory, 256 * sizeof(uint32_t));
    return 0;
}

int RealMachine::stepIntoNextCommand(){
    uint32_t pageTable[16];
    for(int i = 0; i < 16; i++){
        pageTable[i] = userMemory[ptr * 16 + i];
    }

    int i = pc / 16;
    int j = pc % 16;
    uint32_t command = userMemory[pageTable[i]*16+j];
    reverseBytesInWord(command);
    ++pc;
    virtualMachine->runNextCommand(command);
    if(test_() != 0){
        return 1;
    }
    si = 0;
    return 0;
}

void RealMachine::rm_run(uint32_t name){ // ar nereiktu kaip parametro paduot pavadinimo 
    mode = 1;
    channelDevice->setST(3); //is isorines
    channelDevice->setDT(2); //i supervizorine
    channelDevice->setNAME(name); //PVZ1
    channelDevice->setRNUM(256); //16
    channelDevice->setOFF(0);
    channelDevice->setDB(0);
    channelDevice->setSB(0);
    channelDevice->xchg();

    if(convertTextToProgram() != 0){ //validacija
        return;
    }

    allocateMemoryForVirtualMachine();
    channelDevice->setST(2); //is supervizorines
    channelDevice->setDT(1); //i vartotojo
    channelDevice->setRNUM(16);
    virtualMachine = new VirtualMachine(rax, rbx, ds, cs, pc, sf, *this);
    
    uint32_t pageTable[16];
    for(int i = 0; i < 16; i++){
        pageTable[i] = userMemory[ptr * 16 + i];
    }
    

    for(int i = 0; i < 16; ++i){
        channelDevice->setSB(i); //is kurio takelio kopijuojam
        channelDevice->setDB(pageTable[i]); //i kuri takeli kopijuojam
        channelDevice->xchg();
    }

    if(test_() != 0){
        cout << "interupttas po perkopijavmo" << endl;
        return;
    }
    pc = 48;
    pi = 0;

    while(1){
        int i = pc / 16;
        int j = pc % 16;
        uint32_t command = userMemory[pageTable[i]*16+j];
        reverseBytesInWord(command);
        ++pc;
        virtualMachine->runNextCommand(command);
        if(test_() != 0){
            break;
        }
        si = 0;
    }
}

void RealMachine::setEverythingForSteppingMode(uint32_t name){
    channelDevice->setST(3); //is isorines
    channelDevice->setDT(2); //i supervizorine
    channelDevice->setNAME(name); //PVZ1
    channelDevice->setRNUM(256); //16
    channelDevice->setOFF(0);
    channelDevice->setDB(0);
    channelDevice->setSB(0);
    channelDevice->xchg();

    if(convertTextToProgram() != 0){ //validacija
        return;
    }

    allocateMemoryForVirtualMachine();
    channelDevice->setST(2); //is supervizorines
    channelDevice->setDT(1); //i vartotojo
    channelDevice->setRNUM(16);
    virtualMachine = new VirtualMachine(rax, rbx, ds, cs, pc, sf, *this);
    
    uint32_t pageTable[16];
    for(int i = 0; i < 16; i++){
        pageTable[i] = userMemory[ptr * 16 + i];
    }
    

    for(int i = 0; i < 16; ++i){
        channelDevice->setSB(i); //is kurio takelio kopijuojam
        channelDevice->setDB(pageTable[i]); //i kuri takeli kopijuojam
        channelDevice->xchg();
    }

    if(test_() != 0){
        cout << "interupttas po perkopijavmo" << endl;
        return;
    }
    pc = 48;
    pi = 0;
    si = 0;
}

uint32_t RealMachine::translateLocalAdressToRealAddress(uint8_t x, uint8_t y){
    uint32_t pageTable[16] = {0};
    
    for(int i = 0; i < 16; i++){
        pageTable[i] = userMemory[ptr * 16 + i];
    }
    return pageTable[x] * 16 + y;
}

void RealMachine::printAllRegisterValues(){
    cout << "RAX: " << rax << " RBX: " << rbx << " MODE: " << mode << " DS: " << ds << " CS: " << cs << " PC: " << pc << " TI: " << ti << " DI: " << pi << " SI: " << si << " PTR: " << ptr << endl;
    cout << "Status flag: CF: " << sf.cf << " OF: " << sf.of << " AF: " << sf.af << " ZF: " << sf.zf << endl;
}

void RealMachine::printCurrentPage(){
    int x = pc / 16;
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

uint32_t RealMachine::getNextWord(){
    //++pc;
    int i = pc / 16;
    int j = pc % 16;
    
    int realAddress = translateLocalAdressToRealAddress(i, j);
    uint32_t command = userMemory[realAddress];
    uint8_t* word = (uint8_t*)&command;

    if(word[2] != 0 && word[3] != 0){
        command = ((command & 0x000000FF) << 24) |  // Byte 0 -> Byte 3
        ((command & 0x0000FF00) << 8)  |  // Byte 1 -> Byte 2
        ((command & 0x00FF0000) >> 8)  |  // Byte 2 -> Byte 1
        ((command & 0xFF000000) >> 24);
    }
    return command;
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
    si = i;
}

void RealMachine::changePI(uint8_t i){
    pi = i;
}


void RealMachine::allocateMemoryForVirtualMachine(){
    int temp[17];
    srand(time(nullptr));
    int randomIndex;
    for(int i = 0; i < 17; ++i){ 
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

void RealMachine::saveWordToMemoryFromAx(int number){ 
    userMemory[number] = rax;
}

void RealMachine::saveWordToMemoryFromBx(int number){ 
    userMemory[number] = rbx;
}

int RealMachine::test_(){
    if(si > 0){
        switch (si){
            case 1:{
                //HALT
                return 1; //paleist kita programa arba baigt darba
            }
                
            case 2:{ //READ
                channelDevice->setDT(4); // 4 - kopijuojam i rbx
                channelDevice->setST(4); // 4- kopijuojam is klaviaturos
                channelDevice->setRNUM(1);
                channelDevice->xchg();
                rbx = channelDevice->getReg();
                si = 0;
                break;
            }
                
            case 3:{ //PNUM
                channelDevice->setReg(rbx);
                channelDevice->setDT(3); // 3 -isvedimo srautas
                channelDevice->setST(5); // 5 - kopijuojam is RBX
                channelDevice->setRNUM(1);
                channelDevice->setIsNumber(1);
                channelDevice->xchg();
                si = 0;
                break;
            }
                
            case 4:{ //PTXT
                channelDevice->setReg(rbx); //man rods neprisiskiria
                channelDevice->setDT(3); // 3 -isvedimo srautas
                channelDevice->setST(5); // 5 - kopijuojam is RBX
                channelDevice->setRNUM(1);
                channelDevice->setIsNumber(0);
                channelDevice->xchg();
                si = 0;
                break;;
            }
                
            case 5:{
                //EXEx
                //rbx failo pavadinimas
                //x - parametru blokas
                //paleidziam nauja programa
                si = 0;
                break;
            }
                
            default:
                break;
        }
    }
    if(pi > 0){
        switch (pi){ 
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
    return 0;
}
