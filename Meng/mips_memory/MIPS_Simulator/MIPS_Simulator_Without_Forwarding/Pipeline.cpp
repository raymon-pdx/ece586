/*
	Implementation of the Pipeline functions
	
*/

#include "Pipeline.h"
#include "memory.h"
#include "Utility.h"
#include "statistics.h"


// Fetches the instructions from the memory
int Pipeline::Fetch(int & PC, entry* mem, signed long & instr){

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

// Decodes the instructions from trace file and assigns the parts decodes into the struct parsedInstrs
int Pipeline::Decode(signed long instr, InstructionParts & parsedInstrs){
	try
	{
		Utility util;

		bitset<32> b(instr);

		// transform bitset to string
		string binaryInstr = b.to_string<char, std::string::traits_type, std::string::allocator_type>();

		// get opcode

		signed long opcode = strtoul(util.GetBits(binaryInstr, 0, 5).c_str(), nullptr, 2);

		// get the instr type

		Utility my_util;

		
		//true - J-type
		//false - I-type
		bool instrType = my_util.InstructionType(opcode);


		// J -Type Instruction
		if (instrType)
		{
			parsedInstrs.opcode = opcode;
			parsedInstrs.insr_type = instrType;
			parsedInstrs.rs = strtoul(util.GetBits(binaryInstr, 6, 10).c_str(), nullptr, 2);
			parsedInstrs.rt = strtoul(util.GetBits(binaryInstr, 11, 15).c_str(), nullptr, 2);
			parsedInstrs.rd = strtoul(util.GetBits(binaryInstr, 16, 20).c_str(), nullptr, 2);
		}
		//I - Type Instruction
		else
		{
			parsedInstrs.opcode = opcode;
			parsedInstrs.insr_type = instrType;
			parsedInstrs.rs = strtoul(util.GetBits(binaryInstr, 6, 10).c_str(), nullptr, 2);
			parsedInstrs.rt = strtoul(util.GetBits(binaryInstr, 11, 15).c_str(), nullptr, 2);

			string val = util.GetBits(binaryInstr, 16, 31);

			if(util.GetBits(binaryInstr, 16, 16) == "1"){
				val = util.FlipBits(val);
				string value = to_string((stol(val, nullptr, 2) + 1));
				string binary;
				util.HexToBinary(value,  binary, false);
				value = "-" + binary;
				parsedInstrs.imm = stoi(value, nullptr, 2);
			}else{
				parsedInstrs.imm = stol(val, nullptr, 2);
			}
		}

		return 0;
	}
	catch (exception ex)
	{
		return -1;
	}
}

//  
int Pipeline::Execute(InstructionParts & parsedInstr, 
					  signed long * registers, 
					  int & PC,
					  statistics & my_dump)
{

	signed long rs = 0;
	signed long rt = 0;
	signed long immediate = 0;

	if (parsedInstr.insr_type)  // J- type
	{
		rs = registers[parsedInstr.rs];
		rt = registers[parsedInstr.rt];

		switch (parsedInstr.opcode)
		{
		case 0:		// ADD
			my_dump.arith_instruct += 1;
			parsedInstr.result = rs + rt;
			break;
		case 2:		// SUB
			my_dump.arith_instruct += 1;
			parsedInstr.result = rs - rt;
			break;
		case 4:		// MUL
			my_dump.arith_instruct += 1;
			parsedInstr.result = rs * rt;
			break;
		case 6:		// OR
			my_dump.logic_instruct += 1;
			parsedInstr.result = rs | rt;
			break;
		case 8:		// AND  
			my_dump.logic_instruct += 1;
			parsedInstr.result = rs & rt;
			break;
		case 10:	// XOR
			my_dump.logic_instruct += 1;
			parsedInstr.result = rs ^ rt;
			break;
		default:	// HALT
			cout << "Cannot process this opcode : " << parsedInstr.opcode << "hex: " << parsedInstr.insr_type <<endl;
			break;
		}
	}
	else // I-TYPE
	{
		rs = registers[parsedInstr.rs];
		rt = registers[parsedInstr.rt];
		immediate = parsedInstr.imm;

		switch (parsedInstr.opcode)
		{
		case 1:		// ADDI
			my_dump.arith_instruct += 1;
			parsedInstr.result = rs + immediate;
			break;
		case 3:		// SUBI
			my_dump.arith_instruct += 1;
			parsedInstr.result = rs - immediate;
			break;
		case 5:		// MULI
			my_dump.arith_instruct += 1;
			parsedInstr.result = rs * immediate;
			break;
		case 7:		// ORI
			my_dump.logic_instruct += 1;
			parsedInstr.result = rs | immediate;
			break;
		case 9:		// ANDI
			my_dump.logic_instruct += 1;
			parsedInstr.result = rs & immediate;
			break;
		case 11:	// XORI
			my_dump.logic_instruct += 1;
			parsedInstr.result = rs ^ immediate;
			break;
		case 12:	// LDW
			// goto memory
			my_dump.memory_instruct += 1;
			parsedInstr.result = (rs + immediate)/4;
			parsedInstr.is_load = true;
			parsedInstr.is_store = false;

			break;
		case 13:	// STW
			my_dump.memory_instruct += 1;
			parsedInstr.result = (rs + immediate)/4;
			parsedInstr.is_load = false;
			parsedInstr.is_store = true;

			break;
		case 14:	// BZ

			my_dump.control_instruct += 1;

			if (rs == 0)
			{
				PC = parsedInstr.this_pc;

				PC += immediate;

				parsedInstr.reset = true;
			}else{
				parsedInstr.reset = false;
			}

			break;
		case 15:	// BEQ

			my_dump.control_instruct += 1;

			if (rs == rt)
			{
				PC = parsedInstr.this_pc;

				PC += immediate;

				parsedInstr.reset = true;
			}else{
				parsedInstr.reset = false;
			}

			break;
		case 16:	// JR

			my_dump.control_instruct += 1;

			PC = rs/4;

			parsedInstr.reset = true;

			break;
		case 17:	// HALT
			my_dump.control_instruct += 1;
			break;
		default:	
			cout << "Cannot process this opcode : " << parsedInstr.opcode << endl;
		}
	}

	return 0;
}

// Not necessary because mem array is global in the main 
int Pipeline::Memory(){
	return -1;
}

// Not necessary because register array is global in the main 
int Pipeline::WriteBack(){
	return -1;
}
