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

void VirtualMachine::add(uint8_t x, uint8_t y){ // suziuret ar cia viskas logiska
        int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
        uint32_t valueToAdd = realMachine->getWordFromMemory(realAddress);

    uint32_t originalRax = *rax;
    *rax += valueToAdd;
    sf->cf = (*rax < originalRax) ? 1 : 0; // Check for unsigned overflow (carry flag)// If there was overflow, the result will be smaller than original value
    sf->zf = (*rax == 0) ? 1 : 0;  // Check zero flag
    sf->af = (((originalRax & 0x0F) + (valueToAdd & 0x0F)) > 0x0F) ? 1 : 0; // Check auxiliary flag (carry from bit 3 to bit 4)

    //jei buvo overflow i RBX padet 1
}
void VirtualMachine::substract(uint8_t x, uint8_t y){
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    uint32_t valueToSubstract = realMachine->getWordFromMemory(realAddress);
    *rax -= valueToSubstract;
    if(*rax == 0){
        sf->zf = 0;
    }
}
void VirtualMachine::multiply(uint8_t x, uint8_t y){ //istestuot paziuret kur tas musu atsakymas galu gale gaunasi
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
void VirtualMachine::divide(uint8_t x, uint8_t y){
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
void VirtualMachine::compare(){
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
void VirtualMachine::and_(){
    uint32_t word = realMachine->getNextWord();
    *rbx = *rax & word;

    sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
    sf->cf = 0; // CF is typically cleared for logical operations
}
void VirtualMachine::or_(){
    uint32_t word = realMachine->getNextWord();
    *rbx = *rax | word;  // Bitwise OR, store result in RBX
    
    sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
    sf->cf = 0;  // CF typically cleared for logical operations
}
void VirtualMachine::not_(){
    *rbx = ~(*rax);  // Bitwise NOT of RAX, store result in RBX
    
    sf->zf = (*rbx == 0) ? 1 : 0;
    sf->cf = 0;
}
void VirtualMachine::jumpZero(uint8_t x, uint8_t y){
    if(sf->zf){
        //hehe ka daryt
    }
}
void VirtualMachine::jumpNotCarry(uint8_t x, uint8_t y){

} //pagalvot ar nereiks pakeist i jump not zero?
void VirtualMachine::jumpBelow(uint8_t x, uint8_t y){
    if(sf->cf){
        //hehe ka daryt
    }
}
void VirtualMachine::jumpAbove(uint8_t x, uint8_t y){
    if(!sf->zf && !sf->cf){
        //hehe ka daryt
    }
}
void VirtualMachine::moveToAX(uint8_t x, uint8_t y){
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    *rax = realMachine->getWordFromMemory(realAddress);
}
void VirtualMachine::moveToBX(uint8_t x, uint8_t y){
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    *rbx = realMachine->getWordFromMemory(realAddress);
}
void VirtualMachine::saveFromAX(uint8_t x, uint8_t y){ //patikrint
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    realMachine->saveWordToMemoryFromAx(realAddress);
}
void VirtualMachine::saveFromBX(uint8_t x, uint8_t y){ //patikrint
    int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
    realMachine->saveWordToMemoryFromBx(realAddress);
}
void VirtualMachine::execute(uint8_t x){
    //Paleidžiama nauja programa, kurios failo pavadinimas yra nurodomas RBX registre.
    //Yra galimybe paduoti bent vieną bloką duomenu programai kaip parametrą. Baitas x nurodo parametru˛ bloką.
    realMachine->changeSI(5);
}
void VirtualMachine::runNextCommand(){
    
}


void VirtualMachine::readFromKeyboard(){
    realMachine->changeSI(2);
}
void VirtualMachine::printNumber(){
    realMachine->changeSI(3);
    //cia turetu pagal ideja nieko daugiau nereikt ir RBX registre bus tai ka spausdinam ar kaip
}
void VirtualMachine::printText(){
    realMachine->changeSI(4);
    //cia turetu pagal ideja nieko daugiau nereikt ir RBX registre bus tai ka spausdinam ar kaip
}

void VirtualMachine::_halt(){
    realMachine->changeSI(1);
}