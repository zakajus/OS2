#include <iostream>

using namespace std;
#include "realMachine.h"
#include "monitor.h"
#include "keyboard.h"
#include "hardDisk.h"


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
    HardDisk hardisk;
    RealMachine realMachine(monitor, keyboard, hardisk);
    

    int choice = 0;
    cout << "Choose mode: (1) - run everything at once; (2) - stepping mode" << endl;
    cin >> choice;

    if(choice = 1){

    }
    if(choice = 2){
        cout << "Press space to make a step. Press q to exit." << endl;
        while(1){
            char key;
            cin.get(key);
        
            if(key == ' '){
                realMachine.nextStep();
            }
            else if(key == 'q' || key == 'Q'){
                break;
            }
        }
        
    }

    while(1){
        
        int x = realMachine.test_();
        if(x == -1){
            break;
        }
        if (x == 1){
            break; //arba paleist kita programa uzkrovus
        }
        break;
    }

    return 0;
}
