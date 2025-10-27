#include "channelDevice.h"
#include "realMachine.h"
#include "keyboard.h"
#include "monitor.h"
using namespace std;

void ChannelDevice::xchg(){
    if (rnum == 0) {
        throw runtime_error("RNUM cannot be zero");
    }

    std::vector<uint32_t> buffer(rnum);
    switch (st) {
        case 1: {
            copyFromUserMemory(buffer.data(), sb * BLOCK_SIZE + off);
            break;
        }
        case 2: {
            copyFromSupervisorMemory(buffer.data(), sb * BLOCK_SIZE + off);
            break;
        }
        case 3:{
            copyFromExternalMemory(buffer.data());
            break;
        }   
        case 4: {
            copyFromInputStream(buffer.data());
            break;
        }
        case 5:{
            copyFromRbx(buffer.data());
            break;
        }
        default:{
            realMachine->changePI(3);
            break;
        }
            
    }
    switch (dt) {
        case 1: {
            copyToUserMemory(db * BLOCK_SIZE + off, buffer.data());
            break;
        }
        case 2: {
            copyToSupervisorMemory(db * BLOCK_SIZE + off, buffer.data());
            break;
        }    
        case 3: {
            copyToOutputStream(buffer.data());
            break;
        } 
        case 4:{
            copyToRbx(buffer.data());
            break;
        }
        default:{
            realMachine->changePI(3);
            break;
        }   
    }
}

void ChannelDevice::copyFromUserMemory(uint32_t* dest, uint32_t offset) {
    if (rnum + offset > userMemSize) {
        realMachine->changePI(1);
    }
    memcpy(dest, userMemory + offset, rnum*4);
}

void ChannelDevice::copyFromSupervisorMemory(uint32_t* dest, uint32_t offset) {
    if (rnum + offset > supervisorMemSize) {
        realMachine->changePI(1);
    }
    memcpy(dest, supervisorMemory + offset, rnum*4);
}

void ChannelDevice::copyFromExternalMemory(uint32_t* dest) {
    uint32_t word;
    ifstream file("test.dat", ios::binary);
    
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl; //Koks cia pertraukimas turetu but
        return;
    }

    uint8_t buffer[rnum*4];
    uint8_t temp[4];
    uint8_t target[4] = {'*', '*', '*', '*'};

    while(1){
        file.read((char*)temp, 4);
        if(file.gcount() != 4) {
            //kazkoks pertraukimas?
            cout << file.gcount() << endl;
            break;
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
            uint32_t fileNameValue = *((uint32_t*)temp);
            if(fileNameValue == name) {
                break;
            }
        }
    }
    file.read((char*)buffer, rnum*4);
    streamsize bytesRead = file.gcount();
    
    if(bytesRead != rnum*4) {
        cerr << "Warning: Only read " << bytesRead << " bytes out of " << rnum*4 << endl;
    }
    memcpy(dest, buffer, bytesRead); 

    file.close();
}


void ChannelDevice::copyToSupervisorMemory(uint32_t offset, const uint32_t* src) {
    if (offset + rnum > supervisorMemSize) {
        cout << "Ar ccia tas interuptsa" << endl;
        realMachine->changePI(1);
    }
    memcpy(supervisorMemory + offset, src, rnum*4); 
}

void ChannelDevice::copyToUserMemory(uint32_t offset, const uint32_t* src) {
    if (offset + rnum > userMemSize) {
        realMachine->changePI(1);
    }
    memcpy(userMemory + offset, src, rnum*4);
}

void ChannelDevice::copyFromInputStream(uint32_t* dest) {    
    keyboard->getBytes(dest, rnum);
}

void ChannelDevice::copyToOutputStream( uint32_t* src) {
    monitor->display(src, rnum, isNumber);
}

void ChannelDevice::copyFromRbx(uint32_t* dest){
    rnum = 1;
    if (rnum > 1) {
        realMachine->changePI(3); 
        return;
    }   
    
    memcpy(dest, &reg, rnum*4);
}

void ChannelDevice::copyToRbx(const uint32_t* src){
    if (rnum > 1) {
        realMachine->changePI(3); 
        return;
    }
     memcpy(&reg, src, rnum*4);
}
