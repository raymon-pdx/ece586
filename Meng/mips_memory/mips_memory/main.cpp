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

	while (PC <= instrCount)
	{
		
		InstructionParts parsedInstr;

		long long instr;

		instr = mem[PC].word; // FETCH

		err = p.Decode(instr, parsedInstr);
		
	}
	


	//test.STW(1,0,0);
	//test.display(0,0);
	//test.STW(2,0,1);
	//test.display(0,1);

	//test.display(0,3);

	//long temp = 0;

	//test.LDW(temp, 0, 1);
	//cout << "Temp value: " << temp << endl;

	//test.display_all();

	cin.get();
}