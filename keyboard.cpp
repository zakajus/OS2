#include "keyboard.h"


uint8_t Keyboard::getKey(){

}

void Keyboard::getBytes(uint32_t* dest, uint32_t words){
    char buffer[4];
    for(uint32_t i = 0; i < words; i++){
        size_t bytesRead = fread(buffer, 1, 4, stdin);
        uint32_t word = 0;
        for(size_t j = 0; j < 4; j++){
            if(j < bytesRead){
                word |= ((uint32_t)(buffer[j] & 0xFF)) << (j * 8);
            }
        }
        dest[i] = word;
    }
}