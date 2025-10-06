#include <iostream>

struct StatusFlag{
    // one-bit unsigned field, allowed values are 0-1
    unsigned int cf : 1; //carry flag
    unsigned int pr : 1; //parity flag
    unsigned int af : 1; //auxiliary flag
    unsigned int zf : 1; //zero flag
};

class VirtualMachine
{
    private:
        uint8_t* rax;
        uint8_t* rbx;
        uint16_t* ds;
        uint16_t* cs;
        uint16_t* pc;
        StatusFlag* sf;
        RealMachine* realMachine;
    public:
        VirtualMachine(uint8_t &rax, uint8_t &rbx,  uint16_t &ds, uint16_t &cs, uint16_t &pc, StatusFlag &sf, RealMachine &realMachine){
            this->rax = &rax;
            this->rbx = &rbx;
            this->ds = &ds;
            this->cs = &cs;
            this->pc = &pc;
            this->sf = &sf;  
            this->realMachine = &realMachine;
        }

        void add(uint8_t x, uint8_t y){
             int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
             uint8_t valueToAdd = realMachine->getWordFromMemory(realAddress);
             *rax += valueToAdd;
            //patikrint ar nebuvo overflow
        }
        void substract(uint8_t x, uint8_t y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            uint8_t valueToSubstract = realMachine->getWordFromMemory(realAddress);
            *rax -= valueToSubstract;
        }
        void multiply(uint8_t x, uint8_t y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            uint8_t valueToMultiply = realMachine->getWordFromMemory(realAddress);
            *rax *= valueToMultiply;
            //patikrint ar nebuvo overflow
        }
        void divide(uint8_t x, uint8_t y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            uint8_t valueToDivideFrom = realMachine->getWordFromMemory(realAddress);
            *rax /= valueToDivideFrom;
        }
        void compare(){
            if(*rax > *rbx){//CF = 0, ZF = 0
                sf->cf = 0;
                sf->zf = 0;
            }
            else if(*rax < *rbx){  //CF = 1
                sf->cf = 1;
                sf->zf = 0;
            }
            else{ //rax = rbx //ZF = 0
                sf->zf = 0;
            }
        }
        void and_(){
            uint8_t word = realMachine->getNextWord();
            *rbx = *rax & word;

            sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
            sf->cf = 0; // CF is typically cleared for logical operations
        }
        void or_(){
            uint8_t word = realMachine->getNextWord();
            *rbx = *rax | word;  // Bitwise OR, store result in RBX
            
            sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
            sf->cf = 0;  // CF typically cleared for logical operations
        }
        void not_(){
            *rbx = ~(*rax);  // Bitwise NOT of RAX, store result in RBX
            
            sf->zf = (*rbx == 0) ? 1 : 0;
            sf->cf = 0;
        }
        void jumpZero(uint8_t x, uint8_t y){
            if(sf->zf){
                //hehe ka daryt
            }
        }
        void jumpNotCarry(uint8_t x, uint8_t y); //pagalvot ar nereiks pakeist i jump not zero?
        void jumpBelow(uint8_t x, uint8_t y){
            if(sf->cf){
                //hehe ka daryt
            }
        }
        void jumpAbove(uint8_t x, uint8_t y){
            if(!sf->zf && !sf->cf){
                //hehe ka daryt
            }
        }
        void moveToAX(uint8_t x, uint8_t y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            *rax = realMachine->getWordFromMemory(realAddress);
        }
        void moveToBX(uint8_t x, uint8_t y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            *rbx = realMachine->getWordFromMemory(realAddress);
        }
        void saveFromAX(uint8_t x, uint8_t y){ //patikrint
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            realMachine->saveWordToMemoryFromAx(realAddress);
        }
        void saveFromBX(uint8_t x, uint8_t y){ //patikrint
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            realMachine->saveWordToMemoryFromBx(realAddress);
        }
        void execute(uint8_t x){
            //Paleidžiama nauja programa, kurios failo pavadinimas yra nurodomas RBX registre.
            //Yra galimybe paduoti bent vieną bloką duomenu programai kaip parametrą. Baitas x nurodo parametru˛ bloką.
            realMachine->changeSI(5);
        }
        void runNextCommand();


        void readFromKeyboard(){
             realMachine->changeSI(2);
        }
        void printNumber(){
            realMachine->changeSI(3);
            //cia turetu pagal ideja nieko daugiau nereikt ir RBX registre bus tai ka spausdinam ar kaip
        }
        void printText(){
            realMachine->changeSI(4);
            //cia turetu pagal ideja nieko daugiau nereikt ir RBX registre bus tai ka spausdinam ar kaip
        }

        void _halt(){
            realMachine->changeSI(1);
        }



};
