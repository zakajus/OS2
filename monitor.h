#ifndef monitor_h
#define monitor_h


#include <iostream>
#include <cstdint>

class Monitor {
public:
	void display(const uint32_t* src, uint32_t bytes);
	void display(const uint32_t* src, uint32_t bytes, bool isNumber);
	void displayNumber(const uint32_t* src, uint32_t bytes);
	void displayText(const uint32_t* src, uint32_t bytes);
};

#endif