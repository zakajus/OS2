#include <iostream>
#include <cstdint>


using namespace std;

class HardDisk{
    private:
      uint32_t memory[3200];
    public:
      HardDisk();
      ~HardDisk();

    uint32_t getWord(int number);
    

};
