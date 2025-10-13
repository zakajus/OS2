#include "keyboard.h"


uint8_t Keyboard::getKey(){

}

void Keyboard::getBytes(uint32_t* dest, uint32_t rnum){
    std::cin.read(reinterpret_cast<char*>(dest), rnum*4);
    //for (uint32_t i = 0; i < rnum; ++i) {
        //scanf("%c", (char*)&dest[i]);
    //}
}