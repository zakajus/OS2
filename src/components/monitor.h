#ifndef monitor_h
#define monitor_h


#include <iostream>
#include <cstdint>

class Monitor {
public:
	void display( uint32_t* src, uint32_t words, bool isNumber);
	void displayNumber( uint32_t* src, uint32_t words);
	void displayText( uint32_t* src, uint32_t words);
};

#endif