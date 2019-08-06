/*
 * Members: Colin Nugent, Carter Andrews, Patrick Perrine
 * Red ID's: 820151963, 820214623, and 820486635
 * Class Users: cssc0440, cssc0425, cssc0426
 * Class Information: CS530, Spring 2019
 * Assignment #2, SIC/XE Disassembler
 * Filename: opcode.h
 * Purpose: Contains method headers for opcode processing
 */

#include "convert.h"

struct op_code { //here's all the variables needed for the opcode
	string name;
	string opcode;
	string format;
};
struct SIC_Reg { //here's the register table as a bonus
	string name;
	string code;
};
const struct op_code opcodeTable[] = { //Aside from format 0 this should be all Appendix A instructions. Cross-reference if needed
	{ "ADD", "18", "3/4" },       { "ADDR", "90", "2" },      { "ADDF", "58", "3/4" },
	{ "AND", "40", "3/4" },       { "CLEAR", "B4", "2" },     { "COMP", "28", "3/4" },
	{ "COMPF", "88", "3/4" },     { "COMPR", "A0", "2" },     { "DIV", "24", "3/4" },
	{ "DIVR", "9C", "2" },        { "DIVF", "64", "3/4" },    { "FIX", "C4", "1" },
	{ "FLOAT", "C0", "1" },       { "HIO", "F4", "1" },       { "J", "3C", "3/4" },
	{ "JEQ", "30", "3/4" },       { "JGT", "34", "3/4" },     { "JLT", "38", "3/4" },
	{ "JSUB", "48", "3/4" },      { "LDA", "00", "3/4" },     { "LDB", "68", "3/4" },
	{ "LDCH", "50", "3/4" },      { "LDL", "08", "3/4" },     { "LDF", "70", "3/4" },
	{ "LDS", "6C", "3/4" },       { "LDT", "74", "3/4" },     { "LDX", "04", "3/4" },
	{ "LPS", "D0", "3/4" },       { "MUL", "20", "3/4" },     { "MULR", "98", "2" },
	{ "MULF", "60", "3/4" },      { "NORM", "C8", "1" },      { "OR", "44", "3/4" },
	{ "RD", "D8", "3/4" },        { "RMO", "AC", "2" },       { "RSUB", "4C", "3/4" },
	{ "SHIFTL", "A4", "2" },      { "SHIFTR", "A8", "2" },    { "SIO", "F0", "1" },
	{ "SSK", "EC", "3/4" },       { "STA", "0C", "3/4" },     { "STF", "80", "3/4" },
	{ "STB", "78", "3/4" },       { "STCH", "54", "3/4" },    { "STI", "D4", "3/4" },
	{ "STL", "14", "3/4" },       { "STS", "7C", "3/4" },     { "STSW", "E8", "3/4" },
	{ "STT", "84", "3/4" },       { "STX", "10", "3/4" },     { "SUB", "1C", "3/4" },
	{ "SUBR", "94", "2" },        { "SUBF", "5C", "3/4" },    { "SVC", "B0", "2" },
	{ "TD", "E0", "3/4" },        { "TIO", "F8", "1" },       { "TIX", "2C", "3/4" },
	{ "TIXR", "B8", "2" },        { "WD", "DC", "3/4" }
};
const struct SIC_Reg SIC_Regs[] = { //register table
	{ "A", "0" },
	{ "X", "1" },
	{ "L", "2" },
	{ "PC", "8" },
	{ "SW", "9" },
	{ "B", "3" },
	{ "S", "4" },
	{ "T", "5" },
	{ "F", "6" }
};
int validateOpcode(string opcode);
string getInstruction(int optabIndex);
string getFormat(int optabIndex);
string getOpcode(string opcode);
string getRegister(string register_code);

