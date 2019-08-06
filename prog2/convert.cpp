/*
 * Members: Colin Nugent, Carter Andrews, Patrick Perrine
 * Red ID's: 820151963, 820214623, and 820486635
 * Class Users: cssc0440, cssc0425, cssc0426
 * Class Information: CS530, Spring 2019
 * Assignment #2, SIC/XE Disassembler
 * Filename: convert.cpp
 * Purpose: Contains helper methods for data type conversions
 */

#include "convert.h"

// converts Hex in string to int
int stringHexToInt(string hex){
    int num = 0;
    int pow16 = 1;
	//cout << hex << endl;
    string alpha = "0123456789ABCDEF"; //bit value reference
    for(int i = hex.length() - 1; i >= 0; --i) //goes through each bit and converts it to its integer value multiplied by what 16s place it's in
    {
        num += alpha.find(toupper(hex[i])) * pow16; 
        pow16 *= 16; //increments power
    }
	//cout << integerToString(num) << endl;
    return num;
}

// Takes in an int and returns a binary string
string hexToStringBinary(unsigned long hex) { //IMPORTANT: all conversion methods only go through 4 bits at a time (i.e. 0000 in binary or 0 in hex, use these methods accordingly in for loops)
    if(hex == 0)
        return "0000"; 
    else if(hex == 1)
        return "0001";
    else if(hex == 2)
        return "0010";
    else if(hex == 3)
        return "0011";
    else if(hex == 4)
        return "0100";
    else if(hex == 5)
        return "0101";
    else if(hex == 6)
        return "0110";
    else if(hex == 7)
        return "0111";
    else if(hex == 8)
        return "1000";
    else if(hex == 9)
        return "1001";
    else if(hex == 0x0A) //here's the notation for A to F
        return "1010";
    else if(hex == 0x0B)
        return "1011";
    else if(hex == 0x0C)
        return "1100";
    else if(hex == 0x0D)
        return "1101";
    else if(hex == 0x0E)
        return "1110";
    else if (hex == 0x0F)
        return "1111";
    printf("Opcode Error!\n"); //simple exception handling
	return NULL;
}

//Take in a binary string and converts it into a hex string
string stringBinToHex(string s){ //same as HextoBin but backwards
    if(s.compare("0000")==0)
        return "0";
    else if(s.compare("0001")==0)
        return "1";
    else if(s.compare("0010")==0)
        return "2";
    else if(s.compare("0011")==0)
        return "3";
    else if(s.compare("0100")==0)
        return "4";
    else if(s.compare("0101")==0)
        return "5";
    else if(s.compare("0110")==0)
        return "6";
    else if(s.compare("0111")==0)
        return "7";
    else if(s.compare("1000")==0)
        return "8";
    else if(s.compare("1001")==0)
        return "9";
    else if(s.compare("1010")==0)
        return "A";
    else if(s.compare("1011")==0)
        return "B";
    else if(s.compare("1100")==0)
        return "C";
    else if(s.compare("1101")==0)
        return "D";
    else if(s.compare("1110")==0)
        return "E";
    else if(s.compare("1111")==0)
        return "F";
    printf("Invalided Input!\n"); //simple exception handling
	return NULL;
}

//Take in an int and returns it as a string
string integerToString(int number){
    string convertToString;

    stringstream ss;  // stream used for the conversion
    ss << number;      // insert the textual representation of 'Number' in the characters in the stream
    convertToString = ss.str();

    return convertToString;
}
