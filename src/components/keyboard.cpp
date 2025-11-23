#include "keyboard.h"
#include <string>

uint8_t Keyboard::getKey(){
    return 0;
}

void Keyboard::getBytes(uint32_t* dest, uint32_t words){
    std::string input;
    getline(std::cin, input);
    size_t totalBytes = words * sizeof(uint32_t);  // words * 4
    
    int check = 0;
    for(int i = 0; i < input.length(); ++i){
        if(input[i] >= 48 && input[i] <= 57){
            ++check;
        }
    }

    if(check == input.length()){ //visi simboliai buvo skaiciai
        if(input.length() <= 4){
            uint32_t numberValue = 0;
            for(int i = 0; i < input.length(); ++i){
                numberValue = numberValue * 10 + (input[i] - '0');
            }
            dest[0] = numberValue;
            for(uint32_t i = 1; i < words; ++i){
                dest[i] = 0;
            }
        }
        
    }
    else{
        while(input.length() < totalBytes){
            input += '\0';
        }
        memcpy(dest, input.c_str(), totalBytes); 
    }
}
    
