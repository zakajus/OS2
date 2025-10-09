#ifndef monitor_h
#define monitor_h


#include <iostream>
#include <cstdint>

class Monitor {
public:
	void display(const uint8_t* src, uint32_t bytes);
};

#endif