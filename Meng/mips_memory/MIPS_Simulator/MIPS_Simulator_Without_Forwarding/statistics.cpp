#include "statistics.h"

void dump:: dump_all(statistics & my_dump, 
			  signed long * registers, 
			  entry * mem, 
			  int * changed_memory_track,
			  int changeCount){

	cout << "Look at all my dumps" << endl;
	cout << "----------------------" << endl;
	cout << "Total number of instructions: " << my_dump.total_instruct << endl;
	cout << "Arithmetic instructions: " << my_dump.arith_instruct << endl;
	cout << "Logical instructions: " << my_dump.logic_instruct << endl;
	cout << "Memory access instructions: " << my_dump.memory_instruct << endl;
	cout << "Control transfer instructions: " << my_dump.control_instruct << endl;

	cout << "\nProgram Counter: " << my_dump.final_pc << endl;
	
	cout << "\nFinal Register state" << endl;
	cout << "----------------------" << endl;
	for(int i = 0; i < 32; i++)
	{
		cout << "R" << i << ": " << registers[i] << endl;
	}

	cout << "\nFinal Memory State:" << endl;
	cout << "------------------------" << endl;

	/*if(changeCount <= 0)
	{
		cout << "Nothing changed in memory!" << endl;

	}else{
		for(int x = 0; x < changeCount; x++){

			if(changed_memory_track[x] == NULL) continue;

			cout << "Address: " << changed_memory_track[x] * 4 << "  Contents: " << mem[changed_memory_track[x]].word << endl;
		}
	}*/

	cout << "\nHazards" << endl;
	cout << "------------------------" << endl;

	cout << "Total Hazards: " << my_dump.total_data_hazard << endl;
	cout << "Total Stalls: " << my_dump.total_stalls << endl;
	cout << "Total Cycles: " << my_dump.total_cycles << endl;
	
};