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

using namespace std;

//GLOBALS
// ----------------

int PC = 0;

int CLK = 1;

int const REG_LENGTH = 32;

int const MEM_SIZE = 1024;

long registers[REG_LENGTH];

int changed_memory_track[MEM_SIZE];

statistics my_dump;

Utility my_util;

entry mem[MEM_SIZE];

struct instruction_circular_buffer{

	int stage;
	long hex_instr;
	InstructionParts parts;

	instruction_circular_buffer(){
		stage = 0;
	}

} instr_circular_buffer[5];

instruction_circular_buffer *stages[5];

int count_inside_circular_buffer(){
	int count = 0;

	for(int k = 0; k < 5; k++){

		if(instr_circular_buffer[k].stage > 0) count++;
	}

	return count;
}


int main()
{
	int err = -1;

	int instrCount = 0;

	bool first_five = true;

	err = my_util.Nullify(changed_memory_track);

	if(err < 0 ) return 0;

	instrCount = my_util.OpenTraceAndLoadMemory("Trace.txt", mem);

	if (instrCount < 0)
	{
		return 0;
	}

	instrCount = 1;
	
	//int baseRegAdress = instrCount + 1;

	// start pipeline

	Pipeline p;

	long result;

	while (CLK)
	{
		InstructionParts parsedInstr;
		
		int i = 0;

		int erase = -1;

		while(count_inside_circular_buffer() < instrCount || instrCount <= 0)
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
					err = p.Decode(instr_circular_buffer[i].hex_instr, parsedInstr);

					if(err < 0) return 0;

					break;
				// EXECUTE
				case 3:
					
					err = p.Execute(parsedInstr, registers, result, PC, my_dump);

					if(parsedInstr.opcode == 17) CLK = 0;

					if(err < 0){return 0;}

					if(parsedInstr.reset){

						for(int j = i + 1; j <= 5; j++)
						{
							instr_circular_buffer[i].stage = 0;
						}

						instrCount = 5 - i;

						parsedInstr.reset = false;

						//break; // break out of the for - inner loop
					}

					break;
				// MEMEORY
				case 4:
					if (parsedInstr.is_load){
						my_dump.memory_instruct += 1;
						parsedInstr.rd = mem[static_cast<int>(result)].word;
					}

					if (parsedInstr.is_store){
						my_dump.memory_instruct += 1;
						mem[static_cast<int>(result)].word = result;
					}

					break;
				// WRITE BACK
				case 5:
					registers[parsedInstr.rd] = result;
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

		if(count_inside_circular_buffer() < 5){
			instrCount++;
		}else{
			instrCount = 0;
		}

		if(erase >= 0 ) instr_circular_buffer[erase].stage = 0;

	}

	// DUMP
	dump dumper;

	dumper.dump_all(my_dump, registers,  mem, changed_memory_track);

	return 0;
}