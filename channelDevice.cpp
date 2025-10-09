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
    Monitor* monitor;
    Keyboard* keyboard;
    const size_t userMemSize = 1024; //16 blocks * 16 words * 4 bytes
    const size_t supervisorMemSize = 2048; //32 blocks * 16 words * 4 bytes
    const size_t BLOCK_SIZE = 64; // 16 words × 4 bytes

public:
    ChannelDevice(RealMachine* realMachine, uint8_t* userMemory, uint8_t* supervisorMemory, Monitor* monitor, Keyboard* keyboard) : sb(0), db(0), off(0), rnum(0), name(0), st(1), dt(1){
        this->realMachine = &realMachine;
        this->userMemory = &userMemory;
        this->supervisorMemory = &supervisorMemory;
        this->monitor = &monitor;
        this->keyboard = &keyboard;
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

    void copyFromUserMemory(uint8_t* dest, uint32_t offset);
    void copyFromSupervisorMemory(uint8_t* dest, uint32_t offset);
    void copyFromExternalMemory(uint8_t* dest);
    void copyFromInputStream(uint8_t* dest);

    void copyToUserMemory(uint32_t offset, const uint8_t* src);
    void copyToSupervisorMemory(uint32_t offset, const uint8_t* src);
    void copyToOutputStream(const uint8_t* src);

    void xchg(){
        if (rnum == 0) {
            throw runtime_error("RNUM cannot be zero");
        }

        std::vector<uint8_t> buffer(rnum);
        switch (st) {
            case 1: // User memory
                copyFromUserMemory(buffer.data(), sb * BLOCK_SIZE + off);
                break;
            case 2: // Supervisor memory
                copyFromSupervisorMemory(buffer.data(), sb * BLOCK_SIZE + off);
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
                copyToUserMemory(db * BLOCK_SIZE + off, buffer.data());
                break;
            case 2: // Supervisor memory
                copyToSupervisorMemory(db * BLOCK_SIZE + off, buffer.data());
                break;
            case 3: // Output stream
                copyToOutputStream(buffer.data());
                break;
            default:
                realMachine->changePI(3);
        }
        
    }
};

void ChannelDevice::copyFromUserMemory(uint8_t* dest, uint32_t offset) {
    if (rnum + size > userMemSize) {
        realMachine->changePI(1);
    }
    memcpy(dest, userMemory + offset);
}

void ChannelDevice::copyFromSupervisorMemory(uint8_t* dest, uint32_t offset) {
    if (rnum + size > supervisorMemSize) {
        realMachine->changePI(1);
    }
    memcpy(dest, supervisorMemory + offset);
}

void ChannelDevice::copyFromExternalMemory(uint8_t* dest) {
    ifstream file("disk.txt", ios::binary);
    
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl; //Koks cia pertraukimas turetu but
        return 1;
    }

    uint8_t[rnum] buffer;
    uint8_t[4] temp;
    const uint8_t target[4] = {'*', '*', '*', '*'};


    while(1){
        file.read(temp, 4);
        if(file.gcount() != 4) {
            //kazkoks pertraukimas?
            cerr << "Error: Could not find target sequence" << endl;
            file.close();
            return;
        }
        if(memcmp(temp, target, 4) == 0) {
            file.read(temp, 4);
            if(file.gcount() != 4) {
                //kazkoks pertraukimas?
                cerr << "Error: Could not find target sequence" << endl;
                file.close();
                return;
            }
            if(memcmp(temp, &name, 4) == 0) {
                break;
            }
        }
    }
    //
    file.read((char*)buffer, rnum); // Cast to char*
    streamsize bytesRead = file.gcount();
    
    if(bytesRead != rnum) {
        cerr << "Warning: Only read " << bytesRead << " bytes out of " << rnum << endl;
    }
    memcpy(dest, buffer, bytesRead); 
    //cout << "Bytes read: " << bytesRead << endl;

    file.close();
}


void ChannelDevice::copyToSupervisorMemory(uint32_t offset, const uint8_t* src) {
    if (offset + rnum > supervisorMemSize) {
        realMachine->changePI(1);
    }
    memcpy(supervisorMemory + offset, src);
}

void ChannelDevice::copyToUserMemory(uint32_t offset, const uint8_t* src) {
    if (offset + rnum > userMemSize) {
        realMachine->changePI(1);
    }
    memcpy(userMemory + offset, src);
}


void ChannelDevice::copyFromInputStream(uint8_t* dest) {    
    keybord->getBytes(dest, rnum);
}

void ChannelDevice::copyToOutputStream(const uint8_t* src) {
    monitor->display(src, rnum);
}
