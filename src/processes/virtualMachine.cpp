#include "virtualMachine.h"
#include "../components/realMachine.h"

using namespace std;

VirtualMachine::VirtualMachine(uint32_t &rax, uint32_t &rbx,  uint16_t &ds, uint16_t &cs, uint16_t &pc, StatusFlag &sf, RealMachine &realMachine){
    this->rax = &rax;
    this->rbx = &rbx;
    this->ds = &ds;
    this->cs = &cs;
    this->pc = &pc;
    this->sf = &sf;  
    this->realMachine = &realMachine;
}


void VirtualMachine::add(uint8_t x,  uint8_t y)  { 
    int realAddress = realMachine->translateLocalAdressToRealAddress(x, y);
    uint32_t valueToAdd = realMachine->getWordFromMemory(realAddress);

    uint32_t originalRax = *rax;
    *rax += valueToAdd;
    sf->cf = 0;
    sf->cf = (*rax < originalRax) ? 1 : 0; 
    sf->zf = (*rax == 0) ? 1 : 0;  
    sf->af = (((originalRax & 0x0F) + (valueToAdd & 0x0F)) > 0x0F) ? 1 : 0; 
}

void VirtualMachine::substract( uint8_t x,  uint8_t y)  {
    int realAddress = realMachine->translateLocalAdressToRealAddress(x, y);
    uint32_t valueToSubstract = realMachine->getWordFromMemory(realAddress);
    *rax -= valueToSubstract;
    if(*rax == 0){
        sf->zf = 0;
    }
}

void VirtualMachine::multiply( uint8_t x,  uint8_t y)  { 
    int realAddress = realMachine->translateLocalAdressToRealAddress(x, y);
    uint32_t valueToMultiply = realMachine->getWordFromMemory(realAddress);
    uint32_t originalRax = *rax;
    *rax *= valueToMultiply;
    
    if (valueToMultiply != 0) {
        sf->cf = (*rax / valueToMultiply != originalRax) ? 1 : 0;
        sf->of = sf->cf; // For MUL instruction, OF = CF
    } else {
        sf->cf = 0;
        sf->of = 0;
    }

    sf->zf = (*rax == 0) ? 1 : 0;
}

void VirtualMachine::divide( uint8_t x,  uint8_t y)  {
    int realAddress = realMachine->translateLocalAdressToRealAddress(x, y);
    uint32_t valueToDivideFrom = realMachine->getWordFromMemory(realAddress);
    *rax /= valueToDivideFrom;

    if(valueToDivideFrom == 0){
        realMachine->changePI(4);
        return;
    }
        if(*rax == 0){
        sf->zf = 0;
    }
}

void VirtualMachine::compare()  {
    if(*rax > *rbx){//CF = 0, ZF = 0
        sf->cf = 0;
        sf->zf = 0;
    }
    else if(*rax < *rbx){  //CF = 1
        sf->cf = 1;
        sf->zf = 0;
    }
    else{ //rax = rbx //ZF = 0
        sf->zf = 1;
        sf->cf = 0;
    }
}

void VirtualMachine::and_()  {
    uint32_t word = realMachine->getNextWord();
    *pc += 1;
    *rbx = *rax & word;
    sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
    sf->cf = 0; // CF is typically cleared for logical operations
}
void VirtualMachine::or_()  {
    uint32_t word = realMachine->getNextWord();
    *pc += 1;
    *rbx = *rax | word;  // Bitwise OR, store result in RBX
    sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
    sf->cf = 0;  // CF typically cleared for logical operations
}
void VirtualMachine::not_()  {
    *rbx = ~(*rax);  // Bitwise NOT of RAX, store result in RBX
    sf->zf = (*rbx == 0) ? 1 : 0;
    sf->cf = 0;
}

void VirtualMachine::jump(uint8_t x,  uint8_t y)  {
    *pc = x*16 + y;
}

void VirtualMachine::jumpZero( uint8_t x,  uint8_t y)  {
    if (sf->zf) {
        jump(x, y);
    }
}
void VirtualMachine::jumpNotCarry( uint8_t x,  uint8_t y)  {
    if (!sf->cf) {
        jump(x, y);
    }

} //pagalvot ar nereiks pakeist i jump not zero?
void VirtualMachine::jumpBelow( uint8_t x,  uint8_t y)  {
    if(sf->cf){
        jump(x, y);
    }
}
void VirtualMachine::jumpAbove( uint8_t x,  uint8_t y)  {
    if(!sf->zf && !sf->cf){
        jump(x, y);
    }
}
void VirtualMachine::moveToAX( uint8_t x,  uint8_t y)  {
    int realAddress = realMachine->translateLocalAdressToRealAddress(x, y);
    *rax = realMachine->getWordFromMemory(realAddress);
}
void VirtualMachine::moveToBX( uint8_t x,  uint8_t y)  {
    int realAddress = realMachine->translateLocalAdressToRealAddress(x, y);
    uint32_t test = realMachine->getWordFromMemory(realAddress);
    *rbx = realMachine->getWordFromMemory(realAddress);
}
void VirtualMachine::saveFromAX( uint8_t x,  uint8_t y)  { //patikrint
    int realAddress = realMachine->translateLocalAdressToRealAddress(x, y);
    realMachine->saveWordToMemoryFromAx(realAddress);
}
void VirtualMachine::saveFromBX( uint8_t x,  uint8_t y)  { //patikrint
    int realAddress = realMachine->translateLocalAdressToRealAddress(x, y);
    realMachine->saveWordToMemoryFromBx(realAddress);
}

void VirtualMachine::execute(uint8_t x) { // TODO: implement
    //Paleidžiama nauja programa, kurios failo pavadinimas yra nurodomas RBX registre.
    //Yra galimybe paduoti bent vieną bloką duomenu programai kaip parametrą. Baitas x nurodo parametru˛ bloką.
    (*rax) = static_cast<uint32_t>(x);
    realMachine->changeSI(5);
}

void VirtualMachine::runNextCommand( uint32_t cmd) {
    /* implementuojant pacias funkcijas, uztikrinti, kad
     * jos rekursyviai viena kitos nekviestu
     * kad neatsitiktu stackoverflow.com :thumbs_up:
     */

    // TODO: komentarai #
    // TODO: patikrinti $

    // 4 baitu opcodes (nera xy dalies)
    switch (cmd) {
        // Procesoriaus
        case 0x48414C54: // HALT
            _halt();
            return;
        case 0x434D505F: // CMP_
            compare();
            return;

        // Logines
        case 0x414E445F: // AND_
            and_();
            return;
        case 0x4F525F5F: // OR__
            or_();
            return;
        case 0x4E4F545F: // NOT_
            not_();
            return;

        // I/O
        case 0x52454144: // READ
            readFromKeyboard();
            return;
        case 0x504E554D: // PNUM
            printNumber();
            return;
        case 0x50545854: // PTXT
            printText(); //sita taisyt, neisspausdinooo
            return;
    }

    // 2 baitu opcodes (yra xy dalis)
    
    const uint16_t opcode = (cmd >> 16) & 0xFFFF; // & xFF(FF) bitmaskas pravalo prasiftintus bitus
     uint8_t x = (cmd >> 8) & 0xFF;
     uint8_t y = cmd & 0xFF;
     x = realMachine->convertCharToRealHexValue(x);
     y = realMachine->convertCharToRealHexValue(y);

    switch (opcode) {
        // Procesoriaus
        case 0x4144: // ADxy
            add(x, y);
            break;
        case 0x5355: // SUxy
            substract(x, y);
            break;
        case 0x4D55: // MUxy
            multiply(x, y);
            break;
        case 0x4449: // DIxy
            divide(x, y);
            break;

        // Suoliai
        case 0x4A5A: // JZxy
            jumpZero(x, y);
            break;
        case 0x4A4E: // JNxy
            jumpNotCarry(x, y);
            break;
        case 0x4A42: // JBxy
            jumpBelow(x, y);
            break;
        case 0x4A41: // JAxy
            jumpAbove(x, y);
            break;
        case 0x4A50: // JPxy
            jump(x, y);
            break;

        // Darbas su duomenimis
        case 0x4D41: // MAxy
            moveToAX(x, y);
            break;
        case 0x4D42: // MBxy
            moveToBX(x, y);
            break;
        case 0x5341: // SAxy
            saveFromAX(x, y);
            break;
        case 0x5342: // SBxy
            saveFromBX(x, y);
            break;
        case 0x4558: // EXEx - kazkodel turim viena 3 baitu opkoda xd? ziurima tik i EX
            execute(y);
            break;

        default:
            realMachine->changePI(2); // neteisingas operacijos kodas
            /* Teoriskai cia neturetu nusigauti, nes pries tai su test() patikrinama
             * ar opcode yra validus, bet palieku cerr debugginimui */
            cerr << "Error: Invalid opcode " << hex << opcode << dec << endl;
    }
}


void VirtualMachine::readFromKeyboard()  {
    realMachine->changeSI(2);
}
void VirtualMachine::printNumber()  {
    realMachine->changeSI(3);
}
void VirtualMachine::printText()  {
    realMachine->changeSI(4);
}

void VirtualMachine::_halt() {
    realMachine->changeSI(1);
}
