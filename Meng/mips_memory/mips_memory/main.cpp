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

	char* binary_buff = new char[32];

	char* hex_buff = new char[8];

	int err = 0;

	err = my_util.HexToBinary("00000011", binary_buff, false);

	err = my_util.HexToBinary(hex_buff, "00000000000000000000000000010001", true);

/*
	test.STW(1,0,0);
	test.display(0,0);
	test.STW(2,0,1);
	test.display(0,1);

	test.display(0,3);

	long temp = 0;

	test.LDW(temp, 0, 1);
	cout << "Temp value: " << temp << endl;

	test.display_all();
*/
	cin.get();
}