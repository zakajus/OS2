#include "channelDevice.h"
#include "realMachine.h"
#include "keyboard.h"
#include "monitor.h"

using namespace std;

void ChannelDevice::xchg(){
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
        case 5:
            copyFromRbx(buffer.data());
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
        case 4:
            copyToRbx(buffer.data());
        default:
            realMachine->changePI(3);
    }
}


void ChannelDevice::copyFromUserMemory(uint8_t* dest, uint32_t offset) {
    if (rnum + offset > userMemSize) {
        realMachine->changePI(1);
    }
    memcpy(dest, userMemory + offset, rnum);
}

void ChannelDevice::copyFromSupervisorMemory(uint8_t* dest, uint32_t offset) {
    if (rnum + offset > supervisorMemSize) {
        realMachine->changePI(1);
    }
    memcpy(dest, supervisorMemory + offset, rnum);
}

void ChannelDevice::copyFromExternalMemory(uint8_t* dest) {
    ifstream file("disk.txt", ios::binary);
    
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl; //Koks cia pertraukimas turetu but
        return;
    }

    uint8_t buffer[rnum];
    uint8_t temp[4];
    const uint8_t target[4] = {'*', '*', '*', '*'};


    while(1){
        file.read((char*)temp, 4);
        if(file.gcount() != 4) {
            //kazkoks pertraukimas?
            cerr << "Error: Could not find target sequence" << endl;
            file.close();
            return;
        }
        if(memcmp(temp, target, 4) == 0) {
            file.read((char*)temp, 4);
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
    memcpy(supervisorMemory + offset, src, rnum);
}

void ChannelDevice::copyToUserMemory(uint32_t offset, const uint8_t* src) {
    if (offset + rnum > userMemSize) {
        realMachine->changePI(1);
    }
    memcpy(userMemory + offset, src, rnum);
}


void ChannelDevice::copyFromInputStream(uint8_t* dest) {    
    keyboard->getBytes(dest, rnum);
}

void ChannelDevice::copyToOutputStream(const uint8_t* src) {
    monitor->display(src, rnum, isNumber);
}

void ChannelDevice::copyFromRbx(uint8_t* dest){
    if (rnum > 4) {
        realMachine->changePI(3); 
        return;
    }
    
    uint32_t rbxValue = realMachine->getRBX(); 
    memcpy(dest, &rbxValue, rnum);
}

void ChannelDevice::copyToRbx(const uint8_t* src){
    if (rnum > 4) {
        realMachine->changePI(3); 
        return;
    }
    
    uint32_t rbxValue = 0;
    memcpy(&rbxValue, src, rnum);
    realMachine->setRBX(rbxValue);
}
