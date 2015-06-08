
#pragma once
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cctype>
#include <bitset>
#include "memory.h"
#include "statistics.h"

using namespace std;


struct InstructionParts {
	int opcode;
	int rs;
	int rt;
	int rd;
	int imm;

	long result;

	bool insr_type;

	bool is_load;
	bool is_store;

	bool reset;

	InstructionParts() : is_load(false),
		is_store(false), 
		reset(false),
		opcode(0),
		rs(0),
		rt(0),
		rd(0),
		imm(0),
		result(0){}
};

class Pipeline
{

public:
	int Fetch(int &, entry*, long &);
	int Decode(long, InstructionParts &);
	int Execute(InstructionParts &, long *, int &, statistics &);
	int Memory();
	int WriteBack();

};