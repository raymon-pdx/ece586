
#include "Utility.h"

int Utility:: HexToBinary(string & hexadecimal, string & binary, bool inverse)
{
	int error = -1;

	// Binary to HEX
	if (inverse)
	{
		string temp;

		int count = binary.length() / 4;

		for (int i = 0; i < count; i++){
			int br = i * 4;

			for (int j = 0; j < 4; j++){
				temp += binary[br];
				br++;
			}
			
			hexadecimal += GetHexadecimalFromBinary(temp);

			temp = "";
		}

		error = 0;

		return error;
	}

	// HEX to Binary

	for (int i = 0; i < hexadecimal.length(); i++){

		string b = GetBinaryFromHexadecimal(hexadecimal[i]);

		binary += b;

		/*char *cstr = new char[b.length() + 1];
		strcpy(cstr, b.c_str());
		int jr = 0;
		for (int j = (i * 4); j < (i * 4) + 4; j++){
			
			binary[j] = cstr[jr];
			jr++;
		}
		

		delete[] cstr;*/
		error = 0;
	}
	return error;
}

int Utility::OpenTraceAndLoadMemory(string filename, entry* mem)
{
	int err = -1;

	ifstream file(filename);

	string hex;

	int index = 0;

	try
	{
		while (file >> hex)
		{
			string binary;

			HexToBinary(hex, binary, false);

			// changes binary string to long. 
			// changes the format of binary string.
			long long b = stoll(binary, nullptr, 2);

			// store b to the memory

			mem[index].address = index;
			mem[index].word = b;

			index++;
		}
		err = index;
		return err;
	}
	catch (exception ex){
		return err;
	}
}

bool Utility::InstructionType(int opcode)
{
	bool returnValue = false;
	
	//All opcode without the Immediate value
	if (opcode == 0
		|| opcode == 2
		|| opcode == 4
		|| opcode == 6
		|| opcode == 8
		|| opcode == 10)
	{
		returnValue = true;
	}
		return returnValue;
}


int Utility::BinaryToInt(string binary, int integer, bool inverse)
{
	int error = -1;

	if (inverse)
	{
		return error;
	}

	return error;
}

string Utility::ToLittleEndian(string binary)
{
	string temp = "";

	temp += GetBits(binary, 24, 31);
	temp += GetBits(binary, 16, 23);
	temp += GetBits(binary, 8, 15);
	temp += GetBits(binary, 0, 7);

	return temp;
}

string Utility::GetBits(string instr, int start, int end){

	string temp;

	for (int i = start; i <= end; i++)
	{
		temp += instr[i];
	}

	return temp;

}

char Utility::GetHexadecimalFromBinary(string binary){

	if (binary == "0000"){ return '0'; }
	if (binary == "0001"){ return '1'; }
	if (binary == "0010"){ return '2'; }
	if (binary == "0011"){ return '3'; }
	if (binary == "0100"){ return '4'; }
	if (binary == "0101"){ return '5'; }
	if (binary == "0110"){ return '6'; }
	if (binary == "0111"){ return '7'; }
	if (binary == "1000"){ return '8'; }
	if (binary == "1001"){ return '9'; }
	if (binary == "1010"){ return 'A'; }
	if (binary == "1011"){ return 'B'; }
	if (binary == "1100"){ return 'C'; }
	if (binary == "1101"){ return 'D'; }
	if (binary == "1110"){ return 'E'; }
	if (binary == "1111"){ return 'F'; }
	return '-1';
}

string Utility::GetBinaryFromHexadecimal(char hexadecimal){

	if (hexadecimal == '0') { return "0000"; }
	if (hexadecimal == '1') { return "0001"; }
	if (hexadecimal == '2') { return "0010"; }
	if (hexadecimal == '3') { return "0011"; }
	if (hexadecimal == '4') { return "0100"; }
	if (hexadecimal == '5') { return "0101"; }
	if (hexadecimal == '6') { return "0110"; }
	if (hexadecimal == '7') { return "0111"; }
	if (hexadecimal == '8') { return "1000"; }
	if (hexadecimal == '9') { return "1001"; }
	if (hexadecimal == 'A') { return "1010"; }
	if (hexadecimal == 'B') { return "1011"; }
	if (hexadecimal == 'C') { return "1100"; }
	if (hexadecimal == 'D') { return "1101"; }
	if (hexadecimal == 'E') { return "1110"; }
	if (hexadecimal == 'F') { return "1111"; }
	
	return "";
}