

#pragma once
#include "memory.h"

using namespace std;

struct statistics {
	int total_pc;
	int total_instruct;
	int arith_instruct;
	int logic_instruct;
	int memory_instruct;
	int control_instruct;
};

class dump
{
public:
	void dump_all(statistics &, 
			  long *, 
			  entry *, 
			  int *);
};