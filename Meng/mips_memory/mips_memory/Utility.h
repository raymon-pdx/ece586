# pragma once

#include <string>
#include <stdlib.h>
#include <fstream>
#include "memory.h"

using namespace std;


class Utility{

public:

	int HexToBinary(string &, string &, bool);

	int BinaryToInt(string, int, bool );

	char GetHexadecimalFromBinary(string );

	string GetBinaryFromHexadecimal(char);

	string ToLittleEndian(string);

	int OpenTraceAndLoadMemory(string, entry* );

	bool InstructionType(int);

	string GetBits(string, int, int);

	string BitsReverse(string, int, int);

	int Nullify(int *);

	int Flush();
};

