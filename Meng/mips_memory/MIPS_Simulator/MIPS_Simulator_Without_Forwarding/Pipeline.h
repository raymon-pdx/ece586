/*
	STRUCTS:
	- InstructionParts
		.properties
			- opcode (long)
			- rs (long)
			- rt (long)
			- rd (long)
			- imm (long)
			- result (long)
			- this_pc (int)
			- insr_type (bool)
			- is_load (bool)
			- is_store (bool)
			- reset (bool)
			- hazard (bool)

	CLASS:
		- Pipeline
			.members
				- Fetch()
				- Decode()
				- Execute()
				- Memory()
				- WriteBack()
*/


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
	signed long opcode;
	signed long rs;
	signed long rt;
	signed long rd;
	int imm;

	signed long result;

	int this_pc;

	bool insr_type;

	bool is_load;
	bool is_store;

	bool reset;

	bool hazard;

	InstructionParts() : is_load(false),
		is_store(false), 
		reset(false),
		opcode(0),
		rs(0),
		rt(0),
		rd(0),
		imm(0),
		result(0),
		this_pc(0),
		hazard(0){}
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