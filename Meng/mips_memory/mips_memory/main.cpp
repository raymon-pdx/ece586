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

using namespace std;

int main()
{
	memory test; // instatiation

	Utility my_util;

	string binary_buff;

	string hex_buff;

	entry mem[1024];

	string hex_test = "00000011";
	string bin_test = "00000000000000000000000000010001";

	int err = 0;

	err = my_util.HexToBinary(hex_test, binary_buff, false);

	err = my_util.HexToBinary(hex_buff, bin_test, true);

	err = my_util.OpenTraceAndLoadMemory("Trace.txt", mem);

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