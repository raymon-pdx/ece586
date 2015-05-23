#include <cstring>
#include <cctype>
#include <cmath>
#include <bitset>
#include "memory.h"

using namespace std;

/* =========================== */
/* public functions start here */
/* =========================== */

memory::memory()
{
	mips_memory = new entry *[LineSize];

	for (int i = 0; i < LineSize; i++)
	{
		mips_memory[i] = NULL;
	}
}

memory::~memory()
{
	if (flush_all())
		cout << "MIPS-Lite memory module destroyed" << endl;
}

int memory::STW(long Rt, long Rs, long Imm)
{
	if (Rt >= (LineSize))
	{
		long temp_mask = 0xFFFFFFFF;
		Rt &= temp_mask;
	}

	entry to_add;
	to_add.address = Rs + Imm;
	to_add.word = Rt;

	if (add(to_add))
		return 1; // Store succeeded
	else return -1; // Store failed
}

int memory::LDW(long & Rt, long Rs, long Imm)
{
	if (!probe(Rs, Imm))
	{
		cout << "Address line does not exist!\n";
		return -1; // Load failed
	}

	entry temp;
	if (retrieve((Rs + Imm), temp))
	{
		Rt = temp.word;
		return 1; // Load succeeded
	}
	else return -1; // Load failed
}

int memory::probe(long Rs, long Imm)
{
	if (probe((Rs + Imm)))
		return 1; // Adress exists, return true;
	else return 0; // Address does not exist, return false;
}

int memory::display(long Rs, long Imm)
{
	if (!probe((Rs + Imm)))
	{
		cout << "Adress does not exist!\n";
		return -1; // Display failed
	}

	cout << "Address: " << mips_memory[(Rs + Imm)]->address << endl;
	cout << "Data Word: " << mips_memory[(Rs + Imm)]->word << endl;

	return 1; // Display succeeded
}

int memory::display_all()
{
	for (int i = 0; i<LineSize; i++)
	{
		if (probe(i)) // We encountered valid data
		{
			cout << "Address: " << mips_memory[i]->address << "\t";
			cout << "Data Word: " << mips_memory[i]->word << endl;
		}
	}

	return 1; // display_all succeeded
}

/* ========================== */
/* public functions ends here */
/* ========================== */

/* ============================ */
/* private functions start here */
/* ============================ */

int memory::flush_all()
{
	for (int i = 0; i<LineSize; i++)
	{
		delete mips_memory[i];
		mips_memory[i] = NULL;
	}

	delete mips_memory;
	mips_memory = NULL;

	return 1; // flush_all succeeded
}

int memory::add(entry & to_add)
{
	mips_memory[to_add.address] = new entry;
	mips_memory[to_add.address]->address = to_add.address;
	mips_memory[to_add.address]->word = to_add.word;

	return 1; // add succeeded
}

int memory::retrieve(long address, entry & retrieved)
{
	retrieved.address = mips_memory[address]->address;
	retrieved.word = mips_memory[address]->word;

	return 1; // retrieve succeeded
}

int memory::probe(long address)
{
	if (mips_memory[address] == NULL)
		return 0; // memory does not exist
	else return 1; // memory exists
}



