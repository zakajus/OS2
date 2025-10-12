#ifndef HARDDISK_H
#define HARDDISK_H

#include <cstdint>

class HardDisk {
private:
    uint32_t memory[3200];

public:
    HardDisk();
    ~HardDisk();

    uint32_t getWord(uint16_t number) const;
};

#endif // HARDDISK_H