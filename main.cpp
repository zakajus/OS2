#include <iostream>

using namespace std;
#include "realMachine.h"
#include "monitor.h"
#include "keyboard.h"


#ifdef _WIN32
    #include <conio.h>
    inline char getKeyPress() {
        return _getch();
    }
#else
    #include <termios.h>
    #include <unistd.h>
    inline char getKeyPress() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
            perror("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror("tcsetattr ~ICANON");
        return buf;
    }
#endif


int main() {
    //Zingsninis rezimas:
    //isoriniu irenginiu busenos                    kas cia?
    Monitor monitor;
    Keyboard keyboard;
    RealMachine realMachine(monitor, keyboard);
    uint32_t name = 0x315A5650;

    int choice = 0;
    char c;

    
    cout << "Do you want to enter program name? \n[0] - no \n[1] - yes\n";
    cin >> choice;
    cin.ignore();

    if(choice){
        cout << "Enter program name:\n";
        string nameStr;
        cin >> nameStr;
        cin.ignore();

        if(nameStr.length() == 4){
            name = (uint32_t(nameStr[0]) << 24) |
           (uint32_t(nameStr[1]) << 16) |
           (uint32_t(nameStr[2]) << 8)  |
           uint32_t(nameStr[3]);
           realMachine.reverseBytesInWord(name);
        } else {
            cout << "Invalid name! Using default.\n";
            name = 0x315A5650;
        }
    }

    cout << "Choose mode:\n [1] - stepping \n [2] - normal\n";
    cin >> choice;
    cin.ignore();

    if(choice == 1){
        realMachine.setEverythingForSteppingMode(name);
        cout << "Stepping mode: Press SPACE to continue, or 'q' to quit\n";
        while(1){
            if(realMachine.runNextCommand() != 0){
                break;
            }
            
            //realMachine.printAllRegisterValues();
            //realMachine.printCurrentPage();
            uint32_t nextCommand = realMachine.getNextWord();
            //cout << "\nNext command: ";
            //cout << "0x" << hex << setw(8) << setfill('0') << nextCommand << dec << "\n" ;
            cout.flush();
            c = getKeyPress();  // Changed from getch()
            
            
            if(c == ' '){
                continue;
            }
            else if(c == 'q' || c == 'Q'){
                break;
            }
        }
    }
    if(choice == 2){
        realMachine.rm_run(name);
    }

    

    return 0;
}
