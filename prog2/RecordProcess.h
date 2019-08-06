/*
 * Members: Colin Nugent, Carter Andrews, Patrick Perrine
 * Red ID's: 820151963, 820214623, and 820486635
 * Class Users: cssc0440, cssc0425, cssc0426
 * Class Information: CS530, Spring 2019
 * Assignment #2, SIC/XE Disassembler
 * Filename: RecordProcess.h
 * Purpose: Header for the main processing of the .obj file into the .sic 
 */

#include <fstream>
#include <vector>
#include <map>

class RecordProcess{

public:
    void readFile(std::string rawName); //used with my file reading version
    void run(FILE* outFile,FILE* lisFile);
    char *fileName; //used with my file reading version
	//std::string findSymbol(int directAddr);
	std::vector<std::string> printFormat(int lineNumber, int start, int tAddr, int baseAddr);
	std::vector<std::string> objContent;
	std::vector<std::string> symContent;
	std::map<int, std::string> litElement, symElement, directive;
	std::map<int, std::string>::iterator it;
private:
    
};

