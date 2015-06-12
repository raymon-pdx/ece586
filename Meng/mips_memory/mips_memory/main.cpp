// Author: Meng Lei
// MIPS-Lite ISA Memory Simulator
// main.cpp: main functions verification and testing

#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <bitset>
#include "memory.h"
#include "Utility.h"
#include "Pipeline.h"
#include "statistics.h"

#define REG_LENGTH  32
#define MEM_SIZE  1024
#define PIPELINE_SIZE 5
#define CIRCULAR_BUFFER_SIZE 5

using namespace std;

//GLOBALS
// ----------------

int PC = 0;

bool HALT = false;

long registers[REG_LENGTH];

int changed_memory_track[MEM_SIZE];

statistics my_dump;

Utility my_util;

entry mem[MEM_SIZE];

struct instruction_circular_buffer{

	int stage;
	long hex_instr;
	InstructionParts parts;

	instruction_circular_buffer() : 
		stage(0),
		hex_instr(0){}

} instr_circular_buffer[CIRCULAR_BUFFER_SIZE];

instruction_circular_buffer *stages[PIPELINE_SIZE];

int count_inside_circular_buffer()
{
	int count = 0;

	for (int k = 0; k < CIRCULAR_BUFFER_SIZE; k++){

		if(instr_circular_buffer[k].stage > 0) count++;
	}

	return count;
}

void reset_buffer(instruction_circular_buffer * buffer, int index)
{
	instr_circular_buffer[index].stage = 0;
	instr_circular_buffer[index].hex_instr = 0;
	instr_circular_buffer[index].parts.is_load = false;
	instr_circular_buffer[index].parts.is_store = false;
	instr_circular_buffer[index].parts.reset = false;
	instr_circular_buffer[index].parts.imm = 0;
	instr_circular_buffer[index].parts.opcode = 0;
	instr_circular_buffer[index].parts.rd = 0;
	instr_circular_buffer[index].parts.rs = 0;
	instr_circular_buffer[index].parts.rt = 0;
	instr_circular_buffer[index].parts.result = 0;
}


int main()
{
	int err = -1;

	err = my_util.Nullify(changed_memory_track);

	if(err < 0 ) return 0;

	err = my_util.fill_with_zeroes(registers);

	if (err < 0) return 0;

	err = my_util.OpenTraceAndLoadMemory("Trace.txt", mem);

	if (err < 0) return 0;


	// start pipeline

	Pipeline p;

	bool abort;
	int k = 0;
	int i = 0;
	int start = 0;
	int erase = -1;
	int first_five_count = 1;

	while (!HALT)
	{
		
		k = count_inside_circular_buffer();

		if(k < 5)
		{
			k++;
		}

		abort = false;
		erase = -1;
		i = 0;

		while(i < k && !abort)
		{
			int next_stage = instr_circular_buffer[i].stage + 1;

			instr_circular_buffer[i].stage = next_stage;

			switch (next_stage)
			{

				// FETCH
				case 1:
					instr_circular_buffer[i].hex_instr = mem[PC].word;
					PC++;
					my_dump.total_instruct += 1;

					break;

				// DECODE
				case 2:
					err = p.Decode(instr_circular_buffer[i].hex_instr, instr_circular_buffer[i].parts);

					if(err < 0) return 0;

					break;
				// EXECUTE
				case 3:
					
					err = p.Execute(instr_circular_buffer[i].parts, registers, PC, my_dump);
					
					if(err < 0){return 0;}
					
					if (instr_circular_buffer[i].parts.opcode == 17) {
						HALT = true;
					}

					if (instr_circular_buffer[i].parts.reset){

						for (int j = i; j < CIRCULAR_BUFFER_SIZE; j++)
						{
							reset_buffer(instr_circular_buffer, j);							
						}

						instr_circular_buffer[i].parts.reset = false;

						abort = true;

					}

					break;
				// MEMEORY
				case 4:
					if (instr_circular_buffer[i].parts.is_load){
						my_dump.memory_instruct += 1;

						registers[instr_circular_buffer[i].parts.rt] = mem[instr_circular_buffer[i].parts.result].word;
					}

					if (instr_circular_buffer[i].parts.is_store){
						my_dump.memory_instruct += 1;
						mem[instr_circular_buffer[i].parts.result].word = registers[instr_circular_buffer[i].parts.rt];
					}

					reset_buffer(instr_circular_buffer, i);

					break;
				// WRITE BACK
				case 5:
					if(!instr_circular_buffer[i].parts.is_load 
						|| !instr_circular_buffer[i].parts.is_store)
					{
						if(instr_circular_buffer[i].parts.insr_type){  //  J-type
							registers[instr_circular_buffer[i].parts.rd] = instr_circular_buffer[i].parts.result;
						}else{  // I-type
							registers[instr_circular_buffer[i].parts.rt] = instr_circular_buffer[i].parts.result;
						}
					}

					reset_buffer(instr_circular_buffer, i);

					break;

				// Out of the pipeline..... somethign is wrong
				default:
					break;
					//return 0;

			} // end switch - case

			i++; // Go to next buffer

		} // End of Inner While


		// Erase the last instruction that was completed from MEM and WB stage
		//if (erase >= 0) {
			//reset_buffer(instr_circular_buffer, erase);
		//}

	} // End of Outer While

	// DUMP
	dump dumper;

	dumper.dump_all(my_dump, registers,  mem, changed_memory_track);

	return 0;
}