

#include "Pipeline.h"
#include "memory.h"
#include "Utility.h"
#include "statistics.h"

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

int Pipeline::Decode(long instr, InstructionParts & parsedInstrs){
	try
	{
		Utility util;

		bitset<32> b(instr);

		string binaryInstr = b.to_string<char, std::string::traits_type, std::string::allocator_type>();

		// get opcode

		int opcode = strtoul(util.GetBits(binaryInstr, 0, 5).c_str(), nullptr, 2);

		// get the instr type

		Utility my_util;

		
		//true - J-type
		//false - I-type
		// J -Type Instruction
		if (my_util.InstructionType(opcode))
		{
			parsedInstrs.opcode = opcode;
			parsedInstrs.rs = strtoul(util.GetBits(binaryInstr, 6, 10).c_str(), nullptr, 2);
			parsedInstrs.rt = strtoul(util.GetBits(binaryInstr, 11, 15).c_str(), nullptr, 2);
			parsedInstrs.rd = strtoul(util.GetBits(binaryInstr, 16, 20).c_str(), nullptr, 2);
		}
		//I - Type Instruction
		else
		{
			parsedInstrs.opcode = opcode;
			parsedInstrs.rs = strtoul(util.GetBits(binaryInstr, 6, 10).c_str(), nullptr, 2);
			parsedInstrs.rt = strtoul(util.GetBits(binaryInstr, 11, 15).c_str(), nullptr, 2);
			parsedInstrs.imm = strtoul(util.GetBits(binaryInstr, 16, 31).c_str(), nullptr, 2);
		}

		return 0;
	}
	catch (exception ex)
	{
		return -1;
	}
}

int Pipeline::Execute(InstructionParts & parsedInstr, 
					  long * registers, 
					  long & result, 
					  int & PC,
					  statistics & my_dump)
{

	long source1 = 0;
	long source2 = 0;
	long destination = 0;

	if (parsedInstr.insr_type)  // I- type
	{
		source1 = registers[parsedInstr.rs];
		source2 = registers[parsedInstr.rt];

		switch (parsedInstr.opcode)
		{
		case 0:		// ADD
			my_dump.arith_instruct += 1;
			result = source1 + source2;
			break;
		case 2:		// SUB
			my_dump.arith_instruct += 1;
			result = source1 - source2;
			break;
		case 4:		// MUL
			my_dump.arith_instruct += 1;
			result = source1 * source2;
			break;
		case 6:		// OR
			my_dump.logic_instruct += 1;
			result = source1 | source2;
			break;
		case 8:		// AND  
			my_dump.logic_instruct += 1;
			result = source1 & source2;
			break;
		case 10:	// XOR
			my_dump.logic_instruct += 1;
			result = source1 ^ source2;
			break;
		default:	// HALT
			cout << "Cannot process this opcode : " << parsedInstr.opcode << endl;
			break;
		}
	}
	else // J-TYPE
	{
		source1 = registers[parsedInstr.rs];
		source2 = parsedInstr.imm;

		switch (parsedInstr.opcode)
		{
		case 1:		// ADDI
			my_dump.arith_instruct += 1;
			result = source1 + source2;
			break;
		case 3:		// SUBI
			my_dump.arith_instruct += 1;
			result = source1 - source2;
			break;
		case 5:		// MULI
			my_dump.arith_instruct += 1;
			result = source1 * source2;
			break;
		case 7:		// ORI
			my_dump.logic_instruct += 1;
			result = source1 | source2;
			break;
		case 9:		// ANDI
			my_dump.logic_instruct += 1;
			result = source1 & source2;
			break;
		case 11:	// XORI
			my_dump.logic_instruct += 1;
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

			if (source1 == 0)
			{
				my_dump.control_instruct += 1;

				PC += source2;

				parsedInstr.reset = true;
			}

			parsedInstr.reset = false;

			break;
		case 15:	// BEQ

			source2 = registers[parsedInstr.rt];

			if (source1 == source2)
			{
				my_dump.control_instruct += 1;

				PC += parsedInstr.imm;

				parsedInstr.reset = true;
			}

			parsedInstr.reset = false;
			break;
		case 16:	// JR

			my_dump.control_instruct += 1;

			PC += source1;

			parsedInstr.reset = true;

			break;
		default:	// HALT
			cout << "Cannot process this opcode : " << parsedInstr.opcode << endl;
		}

	}

	return 0;
}

int Pipeline::Memory(){
	return -1;
}

int Pipeline::WriteBack(){
	return -1;
}
