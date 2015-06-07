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

int CLK = 1;

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

int count_inside_circular_buffer(){
	int count = 0;

	for (int k = 0; k < CIRCULAR_BUFFER_SIZE; k++){

		if(instr_circular_buffer[k].stage > 0) count++;
	}

	return count;
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

	long result = 0;
	int i = 0;
	int erase = -1;
	int first_five_count = 1;

	while (CLK)
	{

		i = 0;

		erase = -1;

		while(count_inside_circular_buffer() < first_five_count || first_five_count <= 0)
		{
			int next_stage = instr_circular_buffer[i].stage + 1;

			instr_circular_buffer[i].stage = next_stage;

			switch (next_stage){

				// FETCH
				case 1:
					instr_circular_buffer[i].hex_instr = mem[PC].word;
					PC++;
					my_dump.total_pc += 1;

					break;

				// DECODE
				case 2:
					err = p.Decode(instr_circular_buffer[i].hex_instr, instr_circular_buffer[i].parts);

					if(err < 0) return 0;

					break;
				// EXECUTE
				case 3:
					
					err = p.Execute(instr_circular_buffer[i].parts, registers, result, PC, my_dump);

					if (instr_circular_buffer[i].parts.opcode == 17) CLK = 0;

					if(err < 0){return 0;}

					if (instr_circular_buffer[i].parts.reset){

						for (int j = i + 1; j <= CIRCULAR_BUFFER_SIZE; j++)
						{
							instr_circular_buffer[i].stage = 0;
						}

						first_five_count = 5 - i;

						instr_circular_buffer[i].parts.reset = false;

						//break; // break out of the for - inner loop
					}

					break;
				// MEMEORY
				case 4:
					if (instr_circular_buffer[i].parts.is_load){
						my_dump.memory_instruct += 1;
						instr_circular_buffer[i].parts.rd = mem[static_cast<int>(result)].word;
					}

					if (instr_circular_buffer[i].parts.is_store){
						my_dump.memory_instruct += 1;
						mem[static_cast<int>(result)].word = result;
					}

					break;
				// WRITE BACK
				case 5:
					registers[instr_circular_buffer[i].parts.rd] = result;
					erase = i;
					break;

				// Out of the pipeline..... somethign is wrong
				default:
					return 0;
			}

			i++;

			if(i > 4) 
			{
				i = 0; 
				if(erase >= 0 ) instr_circular_buffer[erase].stage = 0;
			}

		}

		if (count_inside_circular_buffer() < CIRCULAR_BUFFER_SIZE){
			first_five_count++;
		}else{
			first_five_count = 0;
		}

		if (erase >= 0) {
			instr_circular_buffer[erase].stage = 0;
			
			instr_circular_buffer[erase].parts.is_load = false;
			instr_circular_buffer[erase].parts.is_store = false;
		}
	}

	// DUMP
	dump dumper;

	dumper.dump_all(my_dump, registers,  mem, changed_memory_track);

	return 0;
}