# pragma once

#include <string>
#include <stdlib.h>
#include <fstream>
#include "memory.h"

using namespace std;

class Utility{

public:
	int HexToBinary(string & hexadecimal, string & binary, bool inverse);

	int BinaryToInt(string binary, int integer, bool inverse);

	char GetHexadecimalFromBinary(string binary);

	string GetBinaryFromHexadecimal(char hexadecimal);

	int OpenTraceAndLoadMemory(string filename, entry* mem);

};

