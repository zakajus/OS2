#include <cstdint>

class ChannelDevice {
private:
    uint16_t sb;
    uint16_t db;
    uint32_t off;
    uint32_t rnum;
    uint32_t name;
    uint8_t st;
    uint8_t dt;

public:
    ChannelDevice(/* args */);
    ~ChannelDevice();

    void xchg();
};
