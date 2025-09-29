class ChannelDevice {
private:
    uint16 sb;
    uint16 db;
    uint32 off;
    uint32 rnum;
    uint32 name;
    uint8 st;
    uint8 dt;

public:
    ChannelDevice(/* args */);
    ~ChannelDevice();

    void xchg();
};
