#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

class ChannelDevice {
private:
    uint16_t sb;
    uint16_t db;
    uint32_t off;
    uint32_t rnum;
    uint32_t name;
    uint8_t st;
    uint8_t dt;

    uint8_t* userMemory;
    uint8_t* supervisorMemory;
    RealMachine* realMachine;
    const size_t userMemSize = 1024; //16 blocks * 16 words * 4 bytes
    const size_t supervisorMemSize = 2048; //32 blocks * 16 words * 4 bytes
    const size_t BLOCK_SIZE = 64; // 16 words × 4 bytes

public:
    ChannelDevice(RealMachine* realMachine, uint8_t* userMemory, uint8_t* supervisorMemory) : sb(0), db(0), off(0), rnum(0), name(0), st(1), dt(1){
        this->realMachine = &realMachine;
        this->userMemory = &userMemory;
        this->supervisorMemory = &supervisorMemory;
    }
    ~ChannelDevice(){}

    void setSB(uint16_t value) { sb = value; }
    void setDB(uint16_t value) { db = value; }
    void setOFF(uint32_t value) { off = value; }
    void setRNUM(uint32_t value) { rnum = value; }
    void setNAME(uint32_t value) { name = value; }
    void setST(uint8_t value) { 
        if (value < 1 || value > 4) 
            realMachine.changePI(3);
        st = value; 
    }
    void setDT(uint8_t value) { 
        if (value < 1 || value > 3) 
            realMachine.changePI(3);
        dt = value; 
    }

    void copyFromUserMemory(uint8_t* dest, uint32_t offset, uint32_t size);
    void copyFromSupervisorMemory(uint8_t* dest, uint32_t offset, uint32_t size);
    void copyFromExternalMemory(uint8_t* dest);
    void copyFromInputStream(uint8_t* dest);

    void copyToUserMemory(uint32_t offset, const uint8_t* src, uint32_t size);
    void copyToSupervisorMemory(uint32_t offset, const uint8_t* src, uint32_t size);
    void copyToOutputStream(const uint8_t* src);

    void xchg(){
        if (rnum == 0) {
            throw runtime_error("RNUM cannot be zero");
        }

        std::vector<uint8_t> buffer(rnum);
        switch (st) {
            case 1: // User memory
                copyFromUserMemory(buffer.data(), sb * BLOCK_SIZE + off, rnum);
                break;
            case 2: // Supervisor memory
                copyFromSupervisorMemory(buffer.data(), sb * BLOCK_SIZE + off, rnum);
                break;
            case 3: // External memory
                copyFromExternalMemory(buffer.data());
                break;
            case 4: // Input stream
                copyFromInputStream(buffer.data());
                break;
            default:
                realMachine->changePI(3);
        }

        // Step 2: Write to destination based on DT
        switch (dt) {
            case 1: // User memory
                copyToUserMemory(db * BLOCK_SIZE + off, buffer.data(), rnum);
                break;
            case 2: // Supervisor memory
                copyToSupervisorMemory(db * BLOCK_SIZE + off, buffer.data(), rnum);
                break;
            case 3: // Output stream
                copyToOutputStream(buffer.data());
                break;
            default:
                realMachine->changePI(3);
        }
        
    }
};

void ChannelDevice::copyFromUserMemory(uint8_t* dest, uint32_t offset, uint32_t size) {
    if (offset + size > userMemSize) {
        realMachine->changePI(1);
    }
    memcpy(dest, userMemory + offset, size);
}

void ChannelDevice::copyFromSupervisorMemory(uint8_t* dest, uint32_t offset, uint32_t size) {
    if (offset + size > supervisorMemSize) {
        realMachine->changePI(1);
    }
    memcpy(dest, supervisorMemory + offset, size);
}

void ChannelDevice::copyFromExternalMemory(uint8_t* dest) {
    //nueit i ta txt faila kur turim 
    //susirast failo pradzia
    //ziuret ar pavadiniams sutampa
    //jei ne einam prie kito failo
    //jei taip tai skaitom tiesiog
    //jei pabaiga griztam
}


void ChannelDevice::copyToSupervisorMemory(uint32_t offset, const uint8_t* src, uint32_t size) {
    if (offset + size > supervisorMemSize) {
        realMachine->changePI(1);
    }
    memcpy(supervisorMemory + offset, src, size);
}

void ChannelDevice::copyToUserMemory(uint32_t offset, const uint8_t* src, uint32_t size) {
    if (offset + size > userMemSize) {
        realMachine->changePI(1);
    }
    memcpy(userMemory + offset, src, size);
}


void ChannelDevice::copyFromInputStream(uint8_t* dest) {    
    //iskviest klaviaturos klase
    //isidet i dest tai ka nuskaitom
}

void ChannelDevice::copyToOutputStream(const uint8_t* src) {
    //Iskviest monitoriaus klase
    //isspausdint tai ka turim src
}
