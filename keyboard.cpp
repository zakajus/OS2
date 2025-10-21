#include "keyboard.h"
#include <string>

uint8_t Keyboard::getKey(){

}

void Keyboard::getBytes(uint32_t* dest, uint32_t words){
    std::string input;
    getline(std::cin, input);
    size_t totalBytes = words * sizeof(uint32_t);  // words * 4
    while(input.length() < totalBytes){
        input += '\0';
    }
    memcpy(dest, input.c_str(), totalBytes); 
}
    
