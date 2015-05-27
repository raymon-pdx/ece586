
#include <string>
#include <stdlib.h>

using namespace std;

class Utility{

public:
	int HexToBinary(char* hexadecimal, char* binary, bool inverse);

	int BinaryToInt(char* binary, int integer, bool inverse);

	char GetHexadecimalFromBinary(char* binary);

	string GetBinaryFromHexadecimal(char hexadecimal);

};

