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

using namespace std;

int main()
{
	memory test; // instatiation

	test.STW(1,0,0);
	test.display(0,0);
	test.STW(2,0,1);
	test.display(0,1);

	test.display(0,3);

	long temp = 0;

	test.LDW(temp, 0, 1);
	cout << "Temp value: " << temp << endl;

	test.display_all();

	cin.get();
}