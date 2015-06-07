#include "statistics.h"

void dump:: dump_all(statistics & my_dump, 
			  long * registers, 
			  entry * mem, 
			  int * changed_memory_track){

	cout << "Look at all my dumps\n" << endl;
	cout << "Total number of instructions: " << my_dump.total_pc << endl;
	cout << "Arithmetic instructions: " << my_dump.arith_instruct << endl;
	cout << "Logical instructions: " << my_dump.logic_instruct << endl;
	cout << "Memory access instructions: " << my_dump.memory_instruct << endl;
	cout << "Control transfer instructions: " << my_dump.control_instruct << endl;


	cout << "Final Register state\n" << endl;

	for(int i = 0; i < 32; i++)
	{
		cout << "R" << i << ": " << registers[i] << endl;
	}

	cout << "Final Memory State:\n" << endl;

	for(int x = 0; x < sizeof(changed_memory_track)/sizeof(int); x++){

		if(changed_memory_track[x] == NULL) continue;

		cout << "Address: " << x << "Contents: " << mem[changed_memory_track[x]].word << endl;
	}
};