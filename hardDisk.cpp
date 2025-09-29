#include <iostream>


using namespace std;

class HardDisk{
    private:
      uint32 memory[3200];
    public:
      HardDisk();
      ~HardDisk();

    uint32 getWord(int number);
    

};
