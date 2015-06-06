

#include "Pipeline.h"
#include "memory.h"
#include "Utility.h"

int Pipeline::Fetch(int & PC, entry* mem, long & instr){

	try
	{
		instr = mem[PC].word;

		PC++;

		return 0;
	}
	catch (exception ex)
	{
		return -1;
	}

}

int Pipeline::Decode(long long instr, InstructionParts & parsedInstrs){
	try
	{
		Utility util;

		bitset<32> b(instr);

		string binaryInstr = b.to_string<char, std::string::traits_type, std::string::allocator_type>();

		// get opcode

		int opcode = strtoull(util.GetBits(binaryInstr, 0, 5).c_str(), nullptr, 2);

		// get the instr type

		Utility my_util;

		// I - Type Instruction
		if (my_util.InstructionType(opcode))
		{
			parsedInstrs.opcode = opcode;
			parsedInstrs.rs = strtoull(util.GetBits(binaryInstr, 6, 10).c_str(), nullptr, 2);
			parsedInstrs.rt = strtoull(util.GetBits(binaryInstr, 11, 15).c_str(), nullptr, 2);
			parsedInstrs.rd = strtoull(util.GetBits(binaryInstr, 16, 20).c_str(), nullptr, 2);
		}
		// J -Type Instruction
		else
		{
			parsedInstrs.opcode = opcode;
			parsedInstrs.rs = strtoull(util.GetBits(binaryInstr, 6, 10).c_str(), nullptr, 2);
			parsedInstrs.rt = strtoull(util.GetBits(binaryInstr, 11, 15).c_str(), nullptr, 2);
			parsedInstrs.imm = strtoull(util.GetBits(binaryInstr, 16, 31).c_str(), nullptr, 2);
		}

		return 0;
	}
	catch (exception ex)
	{
		return -1;
	}
}

int Pipeline::Execute(InstructionParts & parsedInstr, long * registers, long & result, int & PC)
{

	long source1 = 0;
	long source2 = 0;
	long destination = 0;

	if (parsedInstr.insr_type) // I-TYPE
	{
		source1 = registers[parsedInstr.rs];
		source2 = registers[parsedInstr.rt];

		switch (parsedInstr.opcode)
		{
		case 0:		// ADD
			result = source1 + source2;
			break;
		case 2:		// SUB
			result = source1 - source2;
			break;
		case 4:		// MUL
			result = source1 * source2;
			break;
		case 6:		// OR
			result = source1 | source2;
			break;
		case 8:		// AND  
			result = source1 & source2;
			break;
		case 10:	// XOR
			result = source1 ^ source2;
			break;
		default:	// HALT
			break;
		}
	}
	else  // J - Type
	{
		source1 = registers[parsedInstr.rs];
		source2 = parsedInstr.imm;

		switch (parsedInstr.opcode)
		{
		case 1:		// ADDI
			result = source1 + source2;
			break;
		case 3:		// SUBI
			result = source1 - source2;
			break;
		case 5:		// MULI
			result = source1 * source2;
			break;
		case 7:		// ORI
			result = source1 | source2;
			break;
		case 9:		// ANDI
			result = source1 & source2;
			break;
		case 11:	// XORI
			result = source1 ^ source2;
			break;
		case 12:	// LDW
			// goto memory
			// Eff Address = source1 + source2;
			parsedInstr.is_load = true;
			parsedInstr.is_store = false;
			result = source1 + source2;
			break;
		case 13:	// STW
			// Eff address = source1 + source2;
			parsedInstr.is_load = false;
			parsedInstr.is_store = true;
			result = source1 + source2;
			break;
		case 14:	// BZ
			if (parsedInstr.rs == 0)
			{
				PC += source2;

				parsedInstr.reset = true;
			}

			parsedInstr.reset = false;

			break;
		case 15:	// BEQ
			break;
		case 16:	// JR
			break;
		default:	// HALT
			break;
		}

	}

	return -1;
}

int Pipeline::Memory(){
	return -1;
}

int Pipeline::WriteBack(){
	return -1;
}
