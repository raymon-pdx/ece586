#include "statistics.h"

void dump:: dump_all(statistics & my_dump, 
			  long * registers, 
			  entry * mem, 
			  int * changed_memory_track){

	cout << "Look at all my dumps" << endl;
	cout << "----------------------" << endl;
	cout << "Total number of instructions: " << my_dump.total_instruct << endl;
	cout << "Arithmetic instructions: " << my_dump.arith_instruct << endl;
	cout << "Logical instructions: " << my_dump.logic_instruct << endl;
	cout << "Memory access instructions: " << my_dump.memory_instruct << endl;
	cout << "Control transfer instructions: " << my_dump.control_instruct << endl;

	
	cout << "\nFinal Register state" << endl;
	cout << "----------------------" << endl;
	for(int i = 0; i < 32; i++)
	{
		cout << "R" << i << ": " << registers[i] << endl;
	}

	cout << "\nFinal Memory State:\n" << endl;
	cout << "------------------------" << endl;

	int count_memory_changed = (int) sizeof(changed_memory_track)/sizeof(changed_memory_track[0]);

	if(count_memory_changed <= 0)
	{
		cout << "Nothing changed in memory!" << endl;

	}else{
		for(int x = 0; x < count_memory_changed; x++){

			if(changed_memory_track[x] == NULL) continue;

			cout << "Address: " << x << "Contents: " << mem[changed_memory_track[x]].word << endl;
		}
	}
	
};