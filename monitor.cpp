#include "monitor.h"

void Monitor::displayNumber(const uint8_t* src, uint32_t bytes){
    for(uint32_t i = 0; i < bytes; i+=4){
        uint32_t word = (src[i] << 24) | (src[i+1] << 16) | (src[i+2] << 8) | src[i+3];
        printf("0x%08X\n", word);
    }
}

void Monitor::displayText(const uint8_t* src, uint32_t bytes){
    for(uint32_t i = 0; i < bytes; i++){
        if(src[i] >= 32 && src[i] <= 126){
            printf("%c", src[i]); 
        } else {
            printf(".");  
        }
    }
    printf("\n");
}

void Monitor::display(const uint8_t* src, uint32_t bytes, bool isNumber){
    if(isNumber){
        displayNumber(src, bytes);
    }
    else{
        displayText(src, bytes);
    }
}