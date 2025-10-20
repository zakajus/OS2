#include "keyboard.h"
#include <string>

uint8_t Keyboard::getKey(){

}

void Keyboard::getBytes(uint32_t* dest, uint32_t words){
    // Read entire input line
    std::string input;
    getline(std::cin, input);
    //std::cout << input << std::endl;
    size_t totalBytes = words * sizeof(uint32_t);  // words * 4
    while(input.length() < totalBytes){
        input += '\0';
    }
    //std::cout << input.c_str() << std::endl;
    memcpy(dest, input.c_str(), totalBytes); 
}

// void Keyboard::getBytes(uint32_t* dest, uint32_t words){
//     char buffer[4];

//     for(int i = 0; i < words * 4; ++i){

//     }

//     for(uint32_t i = 0; i < words; i++){
//         size_t bytesRead = fread(buffer, 1, 4, stdin);
//         uint32_t word = 0;
//         for(size_t j = 0; j < 4; j++){
//             if(j < bytesRead){
//                 word |= ((uint32_t)(buffer[j] & 0xFF)) << (j * 8);
//             }
//         }
//         dest[i] = word;
//     }
// }

// void Keyboard::getBytes(uint32_t* dest, uint32_t words){
//     // Read entire input line
//     std::string input;
//     getline(std::cin, input);
//     //Convert to words
//     for(uint32_t i = 0; i < words; i++){
//         uint8_t word[4] = {0};
//         for(size_t j = 0; j < 4; j++){
//             size_t charIndex = i * 4 + j;
//             if(charIndex < input.length()){
//                 word[j] = input[charIndex];
//                 std::cout << word[j] << std::endl;
//             }
//         }
//         dest[i] = *((uint32_t*)word);
//     }
// }

// void Keyboard::getBytes(uint32_t* dest, uint32_t words){
//     uint32_t bytesToRead = words * 4;
//     uint8_t buffer[bytesToRead];
    
//     // Read entire input line
//     std::string input;
//     getline(std::cin, input);
    
//     // Check if we got enough data
//     size_t bytesRead = input.length();
//     if(bytesRead != bytesToRead) {
//         std::cerr << "Warning: Only read " << bytesRead << " bytes out of " << bytesToRead << std::endl;
//     }
    
//     // Copy input to buffer (up to bytesRead)
//     memcpy(buffer, input.c_str(), bytesRead);
    
//     // If we read less than expected, zero out the rest
//     if(bytesRead < bytesToRead) {
//         memset(buffer + bytesRead, 0, bytesToRead - bytesRead);
//     }
    
//     // Copy buffer to dest
//     memcpy(dest, buffer, bytesToRead);
// }

    