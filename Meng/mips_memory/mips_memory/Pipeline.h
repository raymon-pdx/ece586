
#pragma once
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cctype>
#include <bitset>
#include "memory.h"

using namespace std;


struct InstructionParts {
	int opcode;
	int rs;
	int rt;
	int rd;
	int imm;

	bool insr_type;
};

class Pipeline
{

public:
	int Fetch(int &, entry*, long &);
	int Decode(long long, InstructionParts &);
	int Execute(InstructionParts &, long *, long &);
	int Memory();
	int WriteBack();

};