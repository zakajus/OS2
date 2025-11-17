#include "monitor.h"

using namespace std;

void Monitor::displayNumber( uint32_t* src, uint32_t words){
    for (uint32_t i = 0; i < words; i++) {
        uint32_t word = src[i];
        printf(" %u ", word);
    }
}

void Monitor::displayText(uint32_t* src, uint32_t words){
    uint8_t* bytes = (uint8_t*)src;
    int totalBytes = words * 4;
    for(int i = 0; i < totalBytes; ++i){
        if (bytes[i] >= 32 && bytes[i] <= 126) { // Printable ASCII
            cout << (char)bytes[i];
        } else if (bytes[i] == 10) { // Newline
            cout << endl;
        } else {
            cout << '.'; // Non-printable characters
        }
    }
}

void Monitor::display(uint32_t* src, uint32_t words, bool isNumber){
    if(isNumber){
        displayNumber(src, words);
    }
    else{
        displayText(src, words);
    }
}
