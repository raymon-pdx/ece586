// Author: Meng Lei
// MIPS-Lite ISA Memory Simulator
// memory.h: header file for memory implementation
#pragma once
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cctype>
#include <bitset>

// It's assumed that each line contains a 4 byte value (32 bits)
const int RegSize = 32;

// And the memory space used is 4KB, whichi is 1024 lines
const int LineSize = 1024;


// Each line entry in handed by a pointer in memory array
// One entry contains
struct entry
{
	long address; // memory line address
	long word;
};


class memory
{
public:
	memory(); // constructor
	~memory(); // destructor
	int load(long & Rt, long Rs, long Imm);
	int store(long Rt, long Rs, long Imm);
	int probe(long Rs, long Imm);
	int display(long Rs, long Imm);
	int display_all();
private:
	int flush_all(); // used in destructor
	int add(entry & to_add); // add new memory line
	int retrieve(long address, entry & retrieved); // retrieve a memory line
	int probe(long address);
	entry ** mips_memory;
};
