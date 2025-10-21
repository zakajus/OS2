#include <iostream>

using namespace std;
#include "realMachine.h"
#include "monitor.h"
#include "keyboard.h"
//#include "hardDisk.h"


int main() {
    //isvest kazka i ekrana (pasisveikinima:DD)
    //Paklaust ar vykdyt programa is kart ar zingsniniu rezimu

    //Zingsninis rezimas:
    //Spausdinti registru reiksmes
    //Spausdinti sekancia vykdoma komanda
    //isoriniu irenginiu busenos                    kas cia?
    //Vykdomos komandos VM puslapio reikšmės.
    Monitor monitor;
    Keyboard keyboard;
    RealMachine realMachine(monitor, keyboard);


    //realMachine.testas2();
    realMachine.testavimui();

    return 0;
}
