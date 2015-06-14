

#pragma once
#include "memory.h"

using namespace std;

struct statistics {
	int final_pc;
	int total_instruct;
	int arith_instruct;
	int logic_instruct;
	int memory_instruct;
	int control_instruct;

	int total_data_hazard;
	int total_stalls;
	int total_cycles;

	statistics() : 
		final_pc(0),
		total_instruct(0),
		arith_instruct(0),
		logic_instruct(0),
		memory_instruct(0),
		control_instruct(0),
		total_data_hazard(0),
		total_stalls(0),
		total_cycles(0){}

};

class dump
{
public:
	void dump_all(statistics &, 
			  long *, 
			  entry *, 
			  int *,
			  int);
};