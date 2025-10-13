#ifndef monitor_h
#define monitor_h


#include <iostream>
#include <cstdint>

class Monitor {
public:
	void display(const uint32_t* src, uint32_t words, bool isNumber);
	void displayNumber(const uint32_t* src, uint32_t words);
	void displayText(const uint32_t* src, uint32_t words);
};

#endif