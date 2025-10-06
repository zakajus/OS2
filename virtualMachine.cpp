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
        uint32* rax;
        uint32* rbx;
        uint16* ds;
        uint16* cs;
        uint16* pc;
        StatusFlag* sf;
        RealMachine* realMachine;
    public:
        VirtualMachine(uint32 &rax, uint32 &rbx,  uint16 &ds, uint16 &cs, uint16 &pc, StatusFlag &sf, RealMachine &realMachine){
            this->rax = &rax;
            this->rbx = &rbx;
            this->ds = &ds;
            this->cs = &cs;
            this->pc = &pc;
            this->sf = &sf;  
            this->realMachine = &realMachine;
        }
        
        void add(uint8 x, uint8 y){
             int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
             uint32 valueToAdd = realMachine->getWordFromMemory(realAddress);
             *rax += valueToAdd;
            //patikrint ar nebuvo overflow
        }
        void substract(uint8 x, uint8 y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            uint32 valueToSubstract = realMachine->getWordFromMemory(realAddress);
            *rax -= valueToSubstract;
        }
        void multiply(uint8 x, uint8 y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            uint32 valueToMultiply = realMachine->getWordFromMemory(realAddress);
            *rax *= valueToMultiply;
            //patikrint ar nebuvo overflow
        }
        void divide(uint8 x, uint8 y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            uint32 valueToDivideFrom = realMachine->getWordFromMemory(realAddress);
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
            uint32 word = realMachine->getNextWord();
            *rbx = *rax & word;

            sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
            sf->cf = 0; // CF is typically cleared for logical operations
        }
        void or_(){
            uint32 word = realMachine->getNextWord();
            *rbx = *rax | word;  // Bitwise OR, store result in RBX
            
            sf->zf = (*rbx == 0) ? 1 : 0;  // Zero flag if result is 0
            sf->cf = 0;  // CF typically cleared for logical operations
        }
        void not_(){
            *rbx = ~(*rax);  // Bitwise NOT of RAX, store result in RBX
            
            sf->zf = (*rbx == 0) ? 1 : 0;
            sf->cf = 0;
        }
        void jumpZero(uint8 x, uint8 y){
            if(sf->zf){
                //hehe ka daryt
            }
        }
        void jumpNotCarry(uint8 x, uint8 y); //pagalvot ar nereiks pakeist i jump not zero?
        void jumpBelow(uint8 x, uint8 y){
            if(sf->cf){
                //hehe ka daryt
            }
        }
        void jumpAbove(uint8 x, uint8 y){
            if(!sf->zf && !sf->cf){
                //hehe ka daryt
            }
        }
        void moveToAX(uint8 x, uint8 y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            *rax = realMachine->getWordFromMemory(realAddress);
        }
        void moveToBX(uint8 x, uint8 y){
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            *rbx = realMachine->getWordFromMemory(realAddress);
        }
        void saveFromAX(uint8 x, uint8 y){ //patikrint
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            realMachine->saveWordToMemoryFromAx(realAddress);
        }
        void saveFromBX(uint8 x, uint8 y){ //patikritn
            int realAddress = realMachine->translateLocalAdrressToRealAddress(x, y);
            realMachine->saveWordToMemoryFromBx(realAddress);
        }
        void execute(uint8 x){
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
