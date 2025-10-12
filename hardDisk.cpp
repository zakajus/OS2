#include "hardDisk.h"
#include <iostream>

using namespace std;

HardDisk::HardDisk() {
}

HardDisk::~HardDisk() {
}

uint32_t HardDisk::getWord(const uint16_t number) const {
    return memory[number];
}