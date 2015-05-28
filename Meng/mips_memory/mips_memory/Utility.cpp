
#include "Utility.h"

int Utility:: HexToBinary(char* hexadecimal, char* binary, bool inverse)
{
	int error = -1;

	// Binary to HEX
	if (inverse)
	{
		char* temp;

		int count = (int)strlen(binary) / 4;

		for (int i = 0; i <= count; i++){
			int br = i * 4;

			for (int j = 0; j < 4; j++){
				temp[j] = binary[br];
				br++;
			}
			hexadecimal[i] = GetHexadecimalFromBinary(temp);
		}

		error = 0;

		return error;
	}

	// HEX to Binary

	for (int i = 0; i <= (int)strlen(hexadecimal); i++){
		string b = GetBinaryFromHexadecimal(hexadecimal[i]);

		char *cstr = new char[b.length() + 1];
		strcpy(cstr, b.c_str());
		int jr = 0;
		for (int j = (i * 4); j < (i * 4) + 4; j++){
			
			binary[j] = cstr[jr];
			jr++;
		}
		

		delete[] cstr;
		error = 0;
	}
	return error;
}

int Utility::BinaryToInt(char* binary, int integer, bool inverse)
{
	int error = -1;

	if (inverse)
	{
		return error;
	}

	return error;
}

char Utility::GetHexadecimalFromBinary(char* binary){

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
}