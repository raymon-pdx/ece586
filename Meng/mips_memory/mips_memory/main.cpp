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

using namespace std;

//GLOBALS
// ----------------

int PC = 0;

int CLK = 1;

long registers[32];

int states[5];

struct instruction_circular_buffer{

	int stage = 0;
	long hex_instr;
	InstructionParts parts;

} instr_ring_buffer[5];

int main()
{

	Utility my_util;

	entry mem[1024];

	int instrCount = 0;

	instrCount = my_util.OpenTraceAndLoadMemory("Trace.txt", mem);

	if (instrCount < 0)
	{
		return 0;
	}

	int baseRegAdress = instrCount + 1;

	// start pipeline

	Pipeline p;

	int err = -1;

	long result;

	while (CLK)
	{
		InstructionParts parsedInstr;

		for (int i = 0; i < 5; i++)
		{
			int next_stage = instr_ring_buffer[i].stage + 1;

			instr_ring_buffer[i].stage = next_stage;

			switch (next_stage){
				// FETCH
				case 1:
					instr_ring_buffer[i].hex_instr = mem[PC].word;
					PC++;
					break;
				// DECODE
				case 2:
					err = p.Decode(instr_ring_buffer[i].hex_instr, parsedInstr);
					break;
				// EXECUTE
				case 3:
					err = p.Execute(parsedInstr, registers, result);
					break;
				// MEMEORY
				case 4:
					break;
				// WRITE BACK
				case 5:
					break;
				// ALL STAGE DONE
				default:
					break;
			}
		}
	}

	return 0;
}