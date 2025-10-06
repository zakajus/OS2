#include <vector>

class Keyboard {
private:
	std::vector<char> keysBuffer;
public:
	char getKey();
};