#include <iostream>

using namespace std;

int main() {
    //isvest kazka i ekrana (pasisveikinima:DD)
    //Paklaust ar vykdyt programa is kart ar zingsniniu rezimu

    //Zingsninis rezimas:
    //Spausdinti registru reiksmes
    //Spausdinti sekancia vykdoma komanda
    //isoriniu irenginiu busenos                    kas cia?
    //Vykdomos komandos VM puslapio reikšmės.

    VirtualMachine virtualMachine1;
    RealMachine realMachine1;

    while(1){
        virtualMachine1.runNextCommand();
        realMachine1.test_();
        
    }
    //po vykdomu komandu tikrinti SI, PI, TI reiksmes

    //cia ir sukurt virtualias masinas

    
    return 0;
}
