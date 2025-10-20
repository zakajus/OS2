#include "channelDevice.h"
#include "realMachine.h"
#include "keyboard.h"
#include "monitor.h"
//#include "hardDisk.h"

using namespace std;

void ChannelDevice::xchg(){
    if (rnum == 0) {
        throw runtime_error("RNUM cannot be zero");
    }

    std::vector<uint32_t> buffer(rnum);
    switch (st) {
        case 1: {
             // User memory
            copyFromUserMemory(buffer.data(), sb * BLOCK_SIZE + off);
            break;
        }
       
        case 2: {
            // Supervisor memory
            copyFromSupervisorMemory(buffer.data(), sb * BLOCK_SIZE + off);
            break;
        }
        
        case 3:{
            // External memory
            copyFromExternalMemory(buffer.data());
            break;
        }
         
        case 4: {
            // Input stream
            copyFromInputStream(buffer.data());
            break;
        }
        
        case 5:{
            copyFromRbx(buffer.data());
            cout << "Ar nukopijavo is rbx: " << buffer.data() << endl;
            break;
        }
            
        default:{
            realMachine->changePI(3);
            break;
        }
            
    }

    // Step 2: Write to destination based on DT
    switch (dt) {
        case 1: {// User memory
            copyToUserMemory(db * BLOCK_SIZE + off, buffer.data());
            break;
        }
            
        case 2: {
            // Supervisor memory
            copyToSupervisorMemory(db * BLOCK_SIZE + off, buffer.data());
            break;
        }
            
        case 3: {
            // Output stream
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

void printAsASCII1(uint32_t* memory, int size) {
    uint8_t* bytes = (uint8_t*)memory;
    int totalBytes = size * 4; // 4 bytes per uint32_t
    
    for (int i = 0; i < totalBytes; i++) {
        if (bytes[i] >= 32 && bytes[i] <= 126) { // Printable ASCII
            cout << (char)bytes[i];
        } else if (bytes[i] == 10) { // Newline
            cout << endl;
        } else {
            cout << '.'; // Non-printable characters
        }
    }
    cout << endl;
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
        cout << "Supervizorinej meta PI" << endl;
    }
    memcpy(dest, supervisorMemory + offset, rnum*4);
    cout << "Supervisor memory: " << endl;
    printAsASCII1(dest, rnum);
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
        realMachine->changePI(1);
    }
    memcpy(supervisorMemory + offset, src, rnum*4); 
}

void ChannelDevice::copyToUserMemory(uint32_t offset, const uint32_t* src) {
    if (offset + rnum > userMemSize) {
        realMachine->changePI(1);
        cout << "Userio meta Pi"  << endl;
    }
    memcpy(userMemory + offset, src, rnum*4);
    cout << "User memory: " << endl;
    printAsASCII1(userMemory + offset, rnum);
}


//KVIECIANT NURODYT BAITUS BBZ KODEL NEVEIKIA
//GALLL nuskaito atvirkstiai TODO: sutvarkyti?
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
