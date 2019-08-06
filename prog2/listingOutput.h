// CMakeProject1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "listingFileLine.h"
void outputListingFile(std::string progName, std::string progStart, std::vector<listingLine> listingFileLines, std::string endingInstructionLabel);
// TODO: Reference additional headers your program requires here.
