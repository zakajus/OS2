#ifndef keyboard_h
#define keyboard_h

#include <iostream>
#include <cstdint>
#include <vector>

class Keyboard {
private:
	std::vector<char> keysBuffer;
public:
	uint8_t getKey();
	void getBytes(uint8_t* dest, uint32_t rnum);
};

#endif