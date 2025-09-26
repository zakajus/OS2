class ChannelDevice {
private:
    uint16 SB;
    uint16 DB;
    uint32 OFF;
    uint32 RNUM;
    uint32 NAME;
    uint8 ST;
    uint8 DT;

public:
    ChannelDevice(/* args */);
    ~ChannelDevice();

    void XCHG();
};