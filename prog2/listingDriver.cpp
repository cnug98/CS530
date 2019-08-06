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
#include "listingFileLine.h"

#include "convert.h"
#include "listingOutput.h"
using namespace std;



int main(int argc, char **argv)
{
	vector<listingLine> testLines;
	listingLine line = { "LDA","123456","3","symbol","0000", "123456","FIRST" };
	testLines.push_back(line);
	outputListingFile("sample", "0000", testLines,"FIRST");
	
}

