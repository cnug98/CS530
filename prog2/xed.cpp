/*
 * Members: Colin Nugent, Carter Andrews, Patrick Perrine
 * Red ID's: 820151963, 820214623, and 820486635
 * Class Users: cssc0440, cssc0425, cssc0426
 * Class Information: CS530, Spring 2019
 * Assignment #2, SIC/XE Disassembler
 * Filename: xed.cpp
 * Purpose: The driver for the processing of the files 
 */

#include "xed.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include "opcode.h"

#include "listingFileLine.h"
#include "convert.h"
#include "RecordProcess.h"
using namespace std;

string progName;
int progStart;
int progSize;
vector<pair<int,int> > modRecords;
int firstInstruction;

vector<listingLine> listingFileLines;

// Reads the header of the .obj file
void readHeader(char* file) {
	string name(file + 1, 6);
	name.erase(remove(name.begin(), name.end(), ' '), name.end());
	progName = name;
	string start(file + 7, 6);
	progStart = stringHexToInt(start);
	string size(file + 13, 6);
	progSize = stringHexToInt(size);
}

// Reads the text records of the .obj file
int readTextRecord(char* file,int location) {
	int textRecordStart = location;
	string start(file + location+1, 6);
	int startingAdress= stringHexToInt(start);
	string size(file + location + 7, 2);
	location += 9;
	int recordLength = stringHexToInt(size);
	
	return location +recordLength*2+10;
	
}

// Reads modification records of the .obj file
int readModRecord(char* file, int location) {
	string start(file + location + 1, 6);
	int startingAdress = stringHexToInt(start);
	string size(file + location + 7, 2);
	int recordLength = stringHexToInt(size);
	modRecords.push_back(pair<int, int>(startingAdress, recordLength));
	return location + 10;
}

// Reads the end record of the .obj file
void readEndRecord(char* file, int location) {
	string start(file + location + 1, 6);
	firstInstruction = stringHexToInt(start);
}

// Performs the processing doe the .obj file
void processObjFile(ifstream&f) {
	f.seekg(0, f.end);
	size_t length = f.tellg();
	f.seekg(0, f.beg);
	char fileContents[100000];

	f.read(fileContents, length);

	readHeader(fileContents);
	int counter = 20;
	while (true) {
		if(fileContents[counter] == 'T')
			counter=readTextRecord(fileContents, counter);
		if (fileContents[counter] == 'M')
			counter = readModRecord(fileContents, counter);
		if (fileContents[counter] == 'E') {
			readEndRecord(fileContents,counter);
		}
		else
			break;
	}
	cout << endl;
}

// Main function, calls methods required for execution
int main(int argc, char **argv)
{
	string rawName(argv[1]);
	RecordProcess* rec = new RecordProcess();
	rec->readFile("sample");
}
	

	


