#include "hardDisk.h"
#include <iostream>
#include <fstream>

using namespace std;

HardDisk::HardDisk() {
    HardDisk::fileName = "hdd.txt";
}

HardDisk::HardDisk(const string &fileName) {
    HardDisk::fileName = fileName;
}

HardDisk::~HardDisk() = default;

void HardDisk::refreshMemory() {
    if (fileWasModified() == false) {
        return;
    }

    ifstream file(fileName, ios::in | ios::binary);
    if (!file) {
        cerr << "Error opening hard disk file" << endl;
        return;
    }
    file.read(reinterpret_cast<char*>(memory), sizeof(memory));
    if (!file) {
        cerr << "Error reading from hard disk file" << endl;
    }
    file.close();
}

filesystem::file_time_type HardDisk::getFileModificationTime() {
    return filesystem::last_write_time(fileName);
}

bool HardDisk::fileWasModified() {
    if (const auto currentModificationTime = getFileModificationTime(); currentModificationTime != lastModificationTime) {
        this->lastModificationTime = currentModificationTime;
        return true;
    }
    return false;
}

uint32_t HardDisk::getWord(const int number) const {
    return memory[number];
}


vector<uint8_t> HardDisk::readBlock(const int fromWord, const int wordCount) const {
    if (fromWord + wordCount >= HDD_SIZE) {
        realMachine->changePI(1);
        break;
    }

    vector<uint8_t> block;
    block.reserve(byteCount);

    for (int i = 0; i < wordCount; ++i) {
        block.push_back(HardDisk::getWord(fromWord + i));
    }
    
    return block;

} // TODO: implement 16 zodziu po 4 baitus

uint32_t* HardDisk::getMemoryPointer()  {
    return this->memory;
}