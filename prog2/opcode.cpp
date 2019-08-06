/*
 * Members: Colin Nugent, Carter Andrews, Patrick Perrine
 * Red ID's: 820151963, 820214623, and 820486635
 * Class Users: cssc0440, cssc0425, cssc0426
 * Class Information: CS530, Spring 2019
 * Assignment #2, SIC/XE Disassembler
 * Filename: opcode.cpp
 * Purpose: Analyzes the given opcode for use in RecordProcess.cpp
 */

#include "opcode.h"
#include <stdio.h>
#include <string.h>
#include <map>

// Returns the register used in the object code
string getRegister(string register_code){ 
    for (int i = 0; i < 9; i++) {
        if (SIC_Regs[i].code == register_code) {
            return SIC_Regs[i].name;
        }
    }
    return "";
}

// Returns a string given the first byte in the object code as a string
string getOpcode(string opcode) { 
    string firstdigitS;
    string lastdigitS;
    int lastdigitI;
    string lastdigitB;
    string lastdigitF;
    string clippedstr;
    string lastdigitH;
    string fullopcode;

    firstdigitS = opcode.substr(0,1);   // grab the first digit 
    lastdigitS = opcode.substr(1,1);    // grab the second digit 

    lastdigitI = stringHexToInt(lastdigitS);             // conversion process of last opcode digit to binary from the string given
    lastdigitB = hexToStringBinary(lastdigitI);       

    clippedstr = (char *)lastdigitB.substr(0,2).c_str();   // takes the second hex digit without last two bits
    lastdigitF = strcat((char *)clippedstr.c_str(), "00"); // appends 00 to second digit, formatting it correctly
    lastdigitH = stringBinToHex(lastdigitF);// convert the parsed opcode into hex

    fullopcode = strcat((char *)firstdigitS.c_str(),(char *)lastdigitH.c_str()); // combime both opcode digits after conversion

    return fullopcode;
}

// Verifies if the instruction is in the optable
int validateOpcode(string opcode){ 
    for (int i = 0; i <= 56; i++) { //56 instructions in table, increase the number of passes if there's more added
        if (opcodeTable[i].opcode.compare(opcode) == 0) {
            return i;
        }
    }
    return -1;
}

// Returns instruction name based on opcode
string getInstruction(int optabIndex){ 
    string instruction;
    instruction = opcodeTable[optabIndex].name;
    return instruction;
}

// Returns format based on opcode (will return both 3 and 4 formats at the same time, use additional methods to verify which is actually used)
string getFormat(int optabIndex){ 
    string format;
    format = opcodeTable[optabIndex].format;
    return format;
}
