

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

		if (my_util.InstructionType(opcode))
		{
			parsedInstrs.opcode = opcode;
			parsedInstrs.rs = strtoull(util.GetBits(binaryInstr, 6, 10).c_str(), nullptr, 2);
			parsedInstrs.rt = strtoull(util.GetBits(binaryInstr, 11, 15).c_str(), nullptr, 2);
			parsedInstrs.rd = strtoull(util.GetBits(binaryInstr, 16, 20).c_str(), nullptr, 2);
		}
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

int Pipeline::Execute(){
	return -1;
}

int Pipeline::Memory(){
	return -1;
}

int Pipeline::WriteBack(){
	return -1;
}
