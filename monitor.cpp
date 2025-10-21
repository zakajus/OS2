#include "monitor.h"

using namespace std;

void Monitor::displayNumber( uint32_t* src, uint32_t words){
    for (uint32_t i = 0; i < words; i++) {
        uint32_t word = src[i];
        printf("%u\n", word);
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

    // for(uint32_t i = 0; i < words; i++){
    //     uint8_t byte0 = (src[i] >> 24) & 0xFF;
    //     uint8_t byte1 = (src[i] >> 16) & 0xFF;
    //     uint8_t byte2 = (src[i] >> 8) & 0xFF;
    //     uint8_t byte3 = src[i] & 0xFF;
        
    //     printf("%c", (byte0 >= 32 && byte0 <= 126) ? byte0 : '.');
    //     printf("%c", (byte1 >= 32 && byte1 <= 126) ? byte1 : '.');
    //     printf("%c", (byte2 >= 32 && byte2 <= 126) ? byte2 : '.');
    //     printf("%c", (byte3 >= 32 && byte3 <= 126) ? byte3 : '.');
    // }
    printf("\n");
}



void Monitor::display(uint32_t* src, uint32_t words, bool isNumber){
    if(isNumber){
        displayNumber(src, words);
    }
    else{
        displayText(src, words);
    }
}