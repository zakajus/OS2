#ifndef HARDDISK_H
#define HARDDISK_H

#define HDD_SIZE 3200

#include <cstdint>
#include <filesystem>

using namespace std;

class HardDisk {
private:
    uint32_t memory[HDD_SIZE]{};
    static string fileName;
    filesystem::file_time_type lastModificationTime;

public:
    HardDisk();
    explicit HardDisk(const string &fileName);

    ~HardDisk();

    /* Nuskaito visa faila tik jei jis buvo pakeistas, tai galima kviesti daznai
     * cia aktualu, kad nereiketu perkrauti OS pakeitus hdd.txt */
    void refreshMemory();

    static filesystem::file_time_type getFileModificationTime() ;

    // Iskvietimas gali atnaujinti lastModificationTime, tai jei reikia keliems palyginimams, reiksme issaugoti i kintamaji
    bool fileWasModified();

    uint32_t getWord(int number) const; // TODO: implement
};

#endif // HARDDISK_H