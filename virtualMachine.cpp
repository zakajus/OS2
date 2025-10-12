#include "virtualMachine.h"
#include "realMachine.h"

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

void VirtualMachine::add(const uint8_t x, const uint8_t y) const { // suziuret ar cia viskas logiska
        int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
        uint32_t valueToAdd = realMachine->getWordFromMemory(realAddress);

    uint32_t originalRax = *rax;
    *rax += valueToAdd;
    sf->cf = (*rax < originalRax) ? 1 : 0; // Check for unsigned overflow (carry flag)// If there was overflow, the result will be smaller than original value
    sf->zf = (*rax == 0) ? 1 : 0;  // Check zero flag
    sf->af = (((originalRax & 0x0F) + (valueToAdd & 0x0F)) > 0x0F) ? 1 : 0; // Check auxiliary flag (carry from bit 3 to bit 4)

    //jei buvo overflow i RBX padet 1
}
void VirtualMachine::substract(const uint8_t x, const uint8_t y) const {
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    uint32_t valueToSubstract = realMachine->getWordFromMemory(realAddress);
    *rax -= valueToSubstract;
    if(*rax == 0){
        sf->zf = 0;
    }
}
void VirtualMachine::multiply(const uint8_t x, const uint8_t y) const { //istestuot paziuret kur tas musu atsakymas galu gale gaunasi
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
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
void VirtualMachine::divide(const uint8_t x, const uint8_t y) const {
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
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
void VirtualMachine::compare() const {
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
void VirtualMachine::and_() const {
    uint32_t word = realMachine->getNextWord();
    *rbx = *rax & word;

    sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
    sf->cf = 0; // CF is typically cleared for logical operations
}
void VirtualMachine::or_() const {
    uint32_t word = realMachine->getNextWord();
    *rbx = *rax | word;  // Bitwise OR, store result in RBX
    
    sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
    sf->cf = 0;  // CF typically cleared for logical operations
}
void VirtualMachine::not_() const {
    *rbx = ~(*rax);  // Bitwise NOT of RAX, store result in RBX
    
    sf->zf = (*rbx == 0) ? 1 : 0;
    sf->cf = 0;
}

// VISIEMS JUMPAMS: tiesiog pakeisti program counteri
void VirtualMachine::jump(const uint8_t x, const uint8_t y) const {
    int const realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    *pc = realMachine->getWordFromMemory(realAddress);
}

void VirtualMachine::jumpZero(const uint8_t x, const uint8_t y) const {
    if (sf->zf) {
        jump(x, y);
    }
}
void VirtualMachine::jumpNotCarry(const uint8_t x, const uint8_t y) const {
    if (!sf->cf) {
        jump(x, y);
    }

} //pagalvot ar nereiks pakeist i jump not zero?
void VirtualMachine::jumpBelow(const uint8_t x, const uint8_t y) const {
    if(sf->cf){
        jump(x, y);
    }
}
void VirtualMachine::jumpAbove(const uint8_t x, const uint8_t y) const {
    if(!sf->zf && !sf->cf){
        jump(x, y);
    }
}
void VirtualMachine::moveToAX(const uint8_t x, const uint8_t y) const {
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    *rax = realMachine->getWordFromMemory(realAddress);
}
void VirtualMachine::moveToBX(const uint8_t x, const uint8_t y) const {
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    *rbx = realMachine->getWordFromMemory(realAddress);
}
void VirtualMachine::saveFromAX(const uint8_t x, const uint8_t y) const { //patikrint
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    realMachine->saveWordToMemoryFromAx(realAddress);
}
void VirtualMachine::saveFromBX(const uint8_t x, const uint8_t y) const { //patikrint
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    realMachine->saveWordToMemoryFromBx(realAddress);
}
void VirtualMachine::execute(const uint8_t x) const {
    //Paleidžiama nauja programa, kurios failo pavadinimas yra nurodomas RBX registre.
    //Yra galimybe paduoti bent vieną bloką duomenu programai kaip parametrą. Baitas x nurodo parametru˛ bloką.
    realMachine->changeSI(5);
}
void VirtualMachine::runNextCommand(const uint32_t cmd) const {
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
            printText();
            return;
    }

    // 2 baitu opcodes (yra xy dalis)
    const uint16_t opcode = (cmd >> 16) & 0xFFFF; // & xFF(FF) bitmaskas pravalo prasiftintus bitus
    const uint8_t x = (cmd >> 8) & 0xFF;
    const uint8_t y = cmd & 0xFF;

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
        case 0x4558: // EXEx - kazkodel turim viena 3 baitu opkoda xd? ziurim tik i EX
            execute(x);
            break;

        default:
            realMachine->changePI(2); // neteisingas operacijos kodas
    }
}


void VirtualMachine::readFromKeyboard() const {
    realMachine->changeSI(2);
}
void VirtualMachine::printNumber() const {
    realMachine->changeSI(3);
    //cia turetu pagal ideja nieko daugiau nereikt ir RBX registre bus tai ka spausdinam ar kaip
}
void VirtualMachine::printText() const {
    realMachine->changeSI(4);
    //cia turetu pagal ideja nieko daugiau nereikt ir RBX registre bus tai ka spausdinam ar kaip
}

void VirtualMachine::_halt() const{
    realMachine->changeSI(1);
}