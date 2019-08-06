// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "listingOutput.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include "opcode.h"
#include "convert.h"
#include "listingFileLine.h"
using namespace std;


void printLine(listingLine line) {
	cout << line.adress;
	cout << "   ";
	cout << line.label;
	for(int i=0;i< 6-line.label.length();i++)
		cout << " ";
	cout << "  ";
	if(line.b==1)
		cout << "+";
	else
		cout << " ";
	cout << line.instruction;
	for (int i = 0;i < 6 - line.instruction.length();i++)
		cout << " ";
	cout << "  ";
	if(line.i==1)
		cout << "#";
	if(line.n==1)
		cout << "@";
	if (0 == 1)//is it a literal
		cout << "=";
	else
		cout << " ";
	cout << line.symbol;
	for (int i = 0;i < 23 - line.symbol.length();i++)
		cout << " ";
	cout << "  ";
	cout << line.opcode;
	cout << endl;
}
void startDirective(string progName,string progStart) {
	cout << progStart;
	cout << "   ";
	cout << progName;
	for (int i = 0;i < 6 - progName.length();i++)
		cout << " ";
	cout << "   START    ";
	cout << progStart;
	cout << endl;
}
void endDirective(string progStartLabel) {
	cout << "                END      ";
	cout << progStartLabel;
	cout << endl;
}
void outputListingFile(string progName,string progStart, vector<listingLine> listingFileLines,string endingInstructionLabel) {
	cout << "This is the listing file for ";
	cout << progName;
	cout << " which starts at ";
	cout << progStart;
	cout << endl;
	startDirective(progName,progStart);
	for (int i;i < listingFileLines.size();i++) {
		printLine(listingFileLines.at(i));
	}
	endDirective(endingInstructionLabel);
}

