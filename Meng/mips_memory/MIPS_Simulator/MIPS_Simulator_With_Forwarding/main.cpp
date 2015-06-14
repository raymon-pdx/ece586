/*---------------------------------------------------------------------------------------------------
Name: Sabin Maharjan
	  Chad Thueson
	  Meng Lei 

Class: ECE 486, Spring 2015

Instructor: Zeshan A. Chishti

Project: 5 Stage MIPS Pipeline Simulator

Description:
-------------
Model of the simplified version of a 5 stage pipeline. The simulator will 
take the provided memory image as its input. It will implement two key features 
(i) A function al simulator which simulates the MIPS-lite ISA and captures the impact of instruction execution on machine state
(ii) A timing simulator which models the timing details for the 5-stage pipeline. The ouput of the simulator will include 
	(a) The breakdown of instruction frequencies in the instruction types 
	(b) Final machine state (register values, memory contents etc.) 
	(c) Execution time (in cycles) of the instruction trace on the 5-stage pipeline.

Header Files:
--------------
- memory.h
- Pipeline.h
- statistics.h
- Utility.h

-------------------------------------------------------------------------------------------*/

// Header Files and libraries
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <bitset>
#include "memory.h"
#include "Utility.h"
#include "Pipeline.h"
#include "statistics.h"

// Constants
#define REG_LENGTH  32
#define MEM_SIZE  1024
#define PIPELINE_SIZE 5
#define CIRCULAR_BUFFER_SIZE 5

using namespace std;

// GLOBALS

int PC = 0;

int changeCount = 0;
/*
	Creates a 5 Array Circular buffer for the instructions	

	Properties:
	stage = current stage of the instruction
	hex_instr = hex value instruction from trace file
	parts = instruction decoded into several parts (see pipeline.h for details)
*/
struct instruction_circular_buffer{
	
	int stage;
	signed long hex_instr;
	bool no_op;

	InstructionParts parts;

	instruction_circular_buffer() : 
		stage(0),
		hex_instr(0),
		no_op(false){}

} instr_circular_buffer[CIRCULAR_BUFFER_SIZE];

signed long registers[REG_LENGTH]; // Contains the register values for storage

signed long forward_registers[REG_LENGTH] = {NULL};	// Registers for keeping record of forwarded register values

statistics my_dump;  // Contains statistical information of the simulator (see statistics.h)

Utility my_util;  // Utilities for different tasks (see Utility.h)

int changed_memory_track[MEM_SIZE];  // records only those that is stored/changes in the memory

entry mem[MEM_SIZE];	// Memory array of 1024 address to store the instructions 

/*
	parameter: none
	return value: int
	summary: counts the number of instructions inside the circular buffer
*/
int count_inside_circular_buffer()
{
	int count = 0;

	for (int k = 0; k < CIRCULAR_BUFFER_SIZE; k++){

		if(instr_circular_buffer[k].stage > 0) count++;
	}

	return count;
}

/*
	parameter: buffer refrence
	return value: none
	summaryu: resets the properties of circular buffer to initial values
*/

void reset_buffer(instruction_circular_buffer & buffer)
{
	buffer.stage = 0;
	buffer.hex_instr = 0;
	buffer.parts.is_load = false;
	buffer.parts.is_store = false;
	buffer.parts.reset = false;
	buffer.parts.hazard = false;
	buffer.parts.this_pc = 0;
	buffer.parts.imm = 0;
	buffer.parts.opcode = 0;
	buffer.parts.rd = 0;
	buffer.parts.rs = 0;
	buffer.parts.rt = 0;
	buffer.parts.result = 0;
	buffer.no_op = false;
	buffer.parts.forward_rs_value = NULL;
	buffer.parts.forward_rt_value = NULL;
}

/*-------------
PROGRAM ENTERS HERE
----------------*/

int main()
{
	bool HALT = false;		// halts the entire simulation

	int err = -1;			// error values variable for checking error in operations

	err = my_util.Nullify(changed_memory_track);	// set the inital values of changed_memory_track to NULL

	if(err < 0 ) return 0;

	err = my_util.fill_with_zeroes(registers);		// Set the initial values of register array to zeros

	if (err < 0) return 0;

	//err = my_util.OpenTraceAndLoadMemory("Trace.txt", mem);		// load the instructions from trace file into mem array
	err = my_util.OpenTraceAndLoadMemory("FinalTrace.txt", mem);

	if (err < 0) return 0;
	
	Pipeline p;		// initialize pipeline class

	/*InstructionParts parts[MEM_SIZE];
	for(int k = 0; k < MEM_SIZE; k++){
		p.Decode(mem[k].word, parts[k]);
	}

	return 0;*/


	bool abort = false;		// ends the pipeline
	bool hazard = false;	// bool check for RAW hazards
	bool found = false;		//	temp value for hazard check
	
	int current_circular_buff_count = 0;	// stores the current number of instructions in the circular buffer
	int circular_buffer_pos = 0;			// current positon of the circular buffer
	
	int tempCounter = 0;		// temporary counter for hazard checks
	int tempsource1 = 0;		// temporary source variable for hazard checks
	int tempsource2 = 0;		// temporary source variable for hazard checks
	int temprd = 0;				// temporary source vairable for hazard checks

	int stall = 0;				// variable to store the number of stall cycles

	// Start and run the Simulator until HALT is true
	while (!HALT)
	{
		abort = false; // reset abort

		bool isBranch = false;	// checks if instruction is branch

		circular_buffer_pos = 0;	// reset circular buffer position

		current_circular_buff_count = count_inside_circular_buffer();	//	get the current instructon count of circular buffer 

		// add one to the existing count because if the pipeline has instructions less than 5
		// its adds one to the existing count to get one more instructions for fetch
		if(current_circular_buff_count < 5){current_circular_buff_count++;}	

		// decrement stall and check if stall is equals to zero and if true set hazard to false
		/*stall--;
		if(stall > 0){
			my_dump.total_stalls += 1;
		}

		if(stall == 0) {
			hazard = false;
		}*/

		// temporary circular buffer 
		// This is the alignment of the instruction inside the circular buffer
		// Basically, it takes the instructions scattered inside the ciruclar buffer and
		// sorts linearly. This is done because our algorithm to get one fetch instruction
		// on one clock cycle was failing if not aligned.
		instruction_circular_buffer tempBuff[CIRCULAR_BUFFER_SIZE];
		int foo = 0;
		for(int k = 0; k < CIRCULAR_BUFFER_SIZE; k++)
		{
			if(instr_circular_buffer[k].stage > 0 ){
				tempBuff[foo] = instr_circular_buffer[k];
				foo++;
				reset_buffer(instr_circular_buffer[k]);
			}
		}

		// recopies back to the circular buffer
		for(int j = 0; j < CIRCULAR_BUFFER_SIZE; j++){
			instr_circular_buffer[j] = tempBuff[j];
		}

		// increment the stages of the pipeline 
		// if there is hazard do not increment the stages for 1,2 and 0
		// this is to prevent RAW hazards.
		for(int x =0; x < current_circular_buff_count; x++){

			instr_circular_buffer[x].stage++;

			/*if(!hazard){
				instr_circular_buffer[x].stage++;
			}else{
				if((instr_circular_buffer[x].stage != 1 
					&& instr_circular_buffer[x].stage != 2)
					&& instr_circular_buffer[x].stage != 0)
				{
					
					instr_circular_buffer[x].stage++;
				}
			}*/
		}

		// Start of the loop to go through each instructions in the pipeline
		// don't stop until it went through all the instruction or abort command is set
		while(circular_buffer_pos < current_circular_buff_count && !abort)
		{
			// get current stage
			int stage = instr_circular_buffer[circular_buffer_pos].stage;

			switch (stage)
			{
				// FETCH
				case 1:
					// Gets the current instruction from the memory that is populated from the trace file
					// Increments the Program Counter - PC

					if(!instr_circular_buffer[circular_buffer_pos].no_op){
						instr_circular_buffer[circular_buffer_pos].hex_instr = mem[PC].word;
						instr_circular_buffer[circular_buffer_pos].parts.this_pc = PC;
						PC++;
					}
					break;

				// DECODE
				case 2: 
					// The instruction is decoded that was fetched from the memory
					// This stage also checks where there are any RAW data hazards in 
					// instructions inside circular buffer

					if(!instr_circular_buffer[circular_buffer_pos].no_op)
					{
						// decodes the instruction fetched from the memory
						err = p.Decode(instr_circular_buffer[circular_buffer_pos].hex_instr, 
										instr_circular_buffer[circular_buffer_pos].parts);
						if(err < 0) {
							return 0;
						}

					}
					
					break;
				// EXECUTE
				case 3:
					// executes the instructions that was decoded in ID stage
					// The execute stage matches which operation code is trying to function
					// It gets the contents of the source operands from the register and depending on the opcode
					// it operates on those source oeprands and stores in the result properties of the buffer

					if(!instr_circular_buffer[circular_buffer_pos].no_op)
					{
						
						my_dump.total_instruct += 1;

						err = p.Execute(instr_circular_buffer[circular_buffer_pos].parts, registers, PC, my_dump);
					
						if(err < 0){
							return 0;
						}

						tempCounter=0;
						tempsource1 = 0;
						tempsource2 = 0;
						temprd = 0;

						// Check if there are any instructions currently in the execution and memory stage
						do{
							if(instr_circular_buffer[tempCounter].stage == 2){
								found = true;
							}
						}while(!found && tempCounter++ < CIRCULAR_BUFFER_SIZE);

						// if found any instructions in mem and ex stage then
						// get the instruction and read the destination operand

						if(found){
							// true - J type
							// false - I type
		
							tempsource1 =  instr_circular_buffer[tempCounter].parts.rs;
							tempsource2 =  instr_circular_buffer[tempCounter].parts.rt;					
						
							// Check if the destination register matches to the source 
							// operands of the current instruction
							// if matches, there is a hazard
							// if not, then hazard is not there
							if(instr_circular_buffer[circular_buffer_pos].stage > 0){
								if( instr_circular_buffer[circular_buffer_pos].parts.insr_type){ // J type
									temprd = instr_circular_buffer[circular_buffer_pos].parts.rd;

									hazard =  (temprd == tempsource1 || temprd == tempsource2);

									if(hazard){
										if(temprd == tempsource1){
											instr_circular_buffer[tempCounter].parts.forward_rs_value = instr_circular_buffer[circular_buffer_pos].parts.result;
										}
										if(temprd == tempsource2){
											instr_circular_buffer[tempCounter].parts.forward_rt_value = instr_circular_buffer[circular_buffer_pos].parts.result;
										}
									}

								}else{ // I type

									temprd = instr_circular_buffer[circular_buffer_pos].parts.rt;

									hazard =  (temprd == tempsource1);
									
									if(hazard){
										instr_circular_buffer[tempCounter].parts.forward_rs_value = instr_circular_buffer[circular_buffer_pos].parts.result;

										if( instr_circular_buffer[tempCounter].parts.opcode == 15){
											instr_circular_buffer[tempCounter].parts.forward_rt_value = instr_circular_buffer[circular_buffer_pos].parts.result;
										}
									}
								}
							}
						}

						found = false; // reset the value of found
						hazard = false;


						// instruction halt is detected and tells the program to abort and halt
						if (instr_circular_buffer[circular_buffer_pos].parts.opcode == 17) {
							abort = true;
							HALT = true;
						}

						// If there is a branch and it matches the condition of the branch,
						// or jumps to different instructions -
						// we have to flush out the latest instructions that was fetched 
						if (instr_circular_buffer[circular_buffer_pos].parts.reset){

							for (int j = 0; j < CIRCULAR_BUFFER_SIZE; j++)
							{
								if(instr_circular_buffer[j].stage == 1 
									|| instr_circular_buffer[j].stage == 2){
									
										instr_circular_buffer[j].no_op = true;
								}
												
							}

							instr_circular_buffer[circular_buffer_pos].parts.reset = false;

							abort = true;

						}
					}
					

					break;
				// MEMEORY
				case 4:
					if(!instr_circular_buffer[circular_buffer_pos].no_op)
					{
						// If the instruction opcode is load, then the result of the operation from the execution stage 
						// is loaded into the registor from the memory to the destination register 
						if (instr_circular_buffer[circular_buffer_pos].parts.is_load){
							
							registers[instr_circular_buffer[circular_buffer_pos].parts.rt] = mem[instr_circular_buffer[circular_buffer_pos].parts.result].word;

							tempCounter=0;
							tempsource1 = 0;
							tempsource2 = 0;
							temprd = 0;

							// Check if there are any instructions currently in the execution and memory stage
							do{
								if(instr_circular_buffer[tempCounter].stage == 2){
									found = true;
								}
							}while(!found && tempCounter++ < CIRCULAR_BUFFER_SIZE);

							// if found any instructions in mem and ex stage then
							// get the instruction and read the destination operand

							if(found){
								// true - J type
								// false - I type

								tempsource1 =  instr_circular_buffer[tempCounter].parts.rs;
								tempsource2 =  instr_circular_buffer[tempCounter].parts.rt;

								// Check if the destination register matches to the source 
								// operands of the current instruction
								// if matches, there is a hazard
								// if not, then hazard is not there
								if(instr_circular_buffer[circular_buffer_pos].stage > 0){
									if( instr_circular_buffer[circular_buffer_pos].parts.insr_type){ // J type
										temprd = instr_circular_buffer[circular_buffer_pos].parts.rd;

										hazard =  (temprd == tempsource1 || temprd == tempsource2);

										if(hazard){
											if(temprd == tempsource1){
												instr_circular_buffer[tempCounter].parts.forward_rs_value = mem[instr_circular_buffer[circular_buffer_pos].parts.result].word;
											}
											if(temprd == tempsource2){
												instr_circular_buffer[tempCounter].parts.forward_rt_value = mem[instr_circular_buffer[circular_buffer_pos].parts.result].word;
											}
										}

									}else{ // I type

										temprd = instr_circular_buffer[circular_buffer_pos].parts.rt;

										hazard =  (temprd == tempsource1);
									
										if(hazard){
											instr_circular_buffer[tempCounter].parts.forward_rs_value = mem[instr_circular_buffer[circular_buffer_pos].parts.result].word;
											
											if( instr_circular_buffer[tempCounter].parts.opcode == 15){
												instr_circular_buffer[tempCounter].parts.forward_rt_value = instr_circular_buffer[circular_buffer_pos].parts.result;
											}
										}
									}
								}
							}

							found = false; // reset the value of found
							hazard = false;
						}

						// If the instruction opcode is store, the register source operand value 
						// is loaded into the register to the address that was calculated from execution stage into the memory location
						if (instr_circular_buffer[circular_buffer_pos].parts.is_store){

							mem[instr_circular_buffer[circular_buffer_pos].parts.result].word = registers[instr_circular_buffer[circular_buffer_pos].parts.rt];

							changed_memory_track[changeCount] = instr_circular_buffer[circular_buffer_pos].parts.result;
							changeCount++;
						}
					}
					break;
				// WRITE BACK
				case 5:
					if(!instr_circular_buffer[circular_buffer_pos].no_op)
					{
						// check if the opcode is branch
						isBranch = instr_circular_buffer[circular_buffer_pos].parts.opcode == 14 
									|| instr_circular_buffer[circular_buffer_pos].parts.opcode == 15
									|| instr_circular_buffer[circular_buffer_pos].parts.opcode == 17;

						// Do not write back to the destination register if 
						// load, store and branch op-code
						if(!instr_circular_buffer[circular_buffer_pos].parts.is_load 
							&& !instr_circular_buffer[circular_buffer_pos].parts.is_store
							&& !isBranch)
						{
							if(instr_circular_buffer[circular_buffer_pos].parts.insr_type){  //  J-type
								registers[instr_circular_buffer[circular_buffer_pos].parts.rd] = instr_circular_buffer[circular_buffer_pos].parts.result;
							}else{  // I-type
								registers[instr_circular_buffer[circular_buffer_pos].parts.rt] = instr_circular_buffer[circular_buffer_pos].parts.result;
							}
						}
					}
					reset_buffer(instr_circular_buffer[circular_buffer_pos]);
					break;

				// Out of the pipeline..... somethign is wrong
				default:
					break;

			} // end switch - case

			circular_buffer_pos++; // Go to next buffer

		} // End of Inner While

		my_dump.total_cycles++;
	} // End of Outer While

	my_dump.final_pc = PC;

	// DUMP
	dump dumper;

	// show the statistics of the simulation
	dumper.dump_all(my_dump, registers,  mem, changed_memory_track, changeCount);

	return 0;
}