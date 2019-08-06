#include "RecordProcess.h"
#include "opcode.h"
#include "convert.h"
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
using namespace std;
void RecordProcess::readFile(){
	int MAX_INPUT = 10000;
    char *input = new char[MAX_INPUT]();
    char *objName = new char[MAX_INPUT]();
    char *symtabName = new char[MAX_INPUT]();
	char *sicFileName = new char[MAX_INPUT]();
    fileName =  new char[MAX_INPUT]();
    string line;
    // Read file name
    if (fgets(input, MAX_INPUT, stdin) != NULL)
        sscanf(input, "%s", fileName);
    
    // Assign extension to file name
    strcat(symtabName, fileName);
    strcat(symtabName, ".sym"); // read in file.sym
    strcat(objName, fileName);
    strcat(objName, ".obj");    // read in file.obj
	strcat(sicFileName, fileName);
	strcat(sicFileName, ".sic");
    // Open obj file and assign each line to vector objContent.
	FILE* outFile= fopen(sicFileName, "w+");
    ifstream objfile(objName);
    if (objfile.is_open()) {
        while (objfile.good()) {
            getline(objfile, line);
            objContent.push_back(line);
        }
        objfile.close();
        
        // Open symtab file and assign each line to vector symContent.
        ifstream symfile(symtabName);
        if (symfile.is_open()) {
            while (symfile.good()) {
                getline(symfile, line);     // read each line
                symContent.push_back(line); // store each line in a vector
            }
            symfile.close();
        } else printf("Unable to process sym file \n");
        
        RecordProcess::run(outFile);
    }
    else printf("Unable to proccess obj file.\n");
}

void RecordProcess::run(FILE* outFile){ //you can add another parameter for listing file
    //symContent and objContent are vectors, while litElement, symElement and directive are all map<int,string> with an iterator (refer to header)
    if (symContent.size() > 0) { 
        for (int i = 0; i < symContent.size()-1; i++) {
            if (symContent[i].length() > 1 && symContent[i][19] != 'r'&& symContent[i][0] != '-') {
                string symbol = symContent[i].substr(0,6); 
                
                int symAddr = stringHexToInt(symContent[i].substr(16,4));
                
                if (symbol[0] == '=')  // encounter literal
                    litElement[symAddr] = symbol;   // store literals in a separate map to reference later
                else {
                    symElement[symAddr] = symbol; //symbol elements stored here
                    directive[symAddr] = symbol; //symbols restored for declaration purposes
                }
            }
        } 
    }
	int lastAddr;
    for (int i = 1; i < objContent.size(); i++) {
        if (objContent[i][0] == 'T') { //start text record processing

            int textAddr = stringHexToInt(objContent[i].substr(1,6));   // text record address
            int textLength = stringHexToInt(objContent[i].substr(7,2)); // text record length
            lastAddr = textAddr + textLength; //final address of text record
            int start = 9; //location of first text record instruction
            int textSkip = 0; //where to jump to the next instruction
            int baseAddr = 0; //storage for base address
            
            while (textAddr < lastAddr) {
                
                /*Portions of data vector
                 data[0] = instruction
                 data[1] = skipBits (total bits to skip to get next instruction)
                 data[2] = originalSym (original symbol used)
                 data[3] = addressed symbol
                 data[4] = format
                 */
                
                vector<string> data = fileFormat(i,start,textAddr, baseAddr); //data can contain everything the file printing system needs at once, vectors are extremely useful
                
                string instruction = data[0];
                string skipBits = data[1];
                textSkip = stringHexToInt(skipBits);
                string option = data[3];
                int format = stringHexToInt(data[4]);
                
                // Check if address in symtab and littab
                string hasSym = ""; 
                if (symElement.find(textAddr) != symElement.end()) {
                    hasSym = symElement.find(textAddr)->second; //find symbol name
                }
                
                string hasLit = "";
                if (litElement.find(textAddr) != litElement.end()) {
                    hasLit = litElement.find(textAddr)->second; //if there's a literal, find it as well
                }
                
                // IMPORTANT: All of the printf functions work and are commented out, but are only formatted for the .sic file. Add addresses and object code and adjust spacing when needed.
                if (hasSym.length() > 0) { //symbol found
                    //prints instruction
					if (format == 4) //extended situation
						//fprintf(outFile,"%-9s %-11s %-5s %s %s \n", hasSym.c_str(), "","+", instruction.c_str(), option.c_str()); 
						;
					else //otherwise just print instruction with no additional symbols
						//fprintf(outFile, "%-9s %-11s %s \n", hasSym.c_str(), instruction.c_str(), option.c_str());
						;
                    
                    // Delete used symbol in directive map
                    directive.erase(textAddr);
                    
                    // jump to next instruction
                    start = start + textSkip;
                    
                    // increment pc
                    textAddr = textAddr + format;
                }
                
                else if ((hasLit.length() > 0) && (litElement.begin()->second == hasLit)) { //literal found

                    //fprintf(outFile, "%-9s %-11s \n", "", "LTORG");
                    textSkip = 2;
                    
                    // jump to next instruction
                    start = start + textSkip;
                    
                    // increment pc
                    textAddr = textAddr + 1;
                }
                
               
                else{ //if neither is found
					if (format == 4) //extended
						//fprintf(outFile, "%-8s %s%-11s %s \n", "","+", instruction.c_str(), option.c_str());
						;
					else
						//fprintf(outFile, "%-9s %-11s %s \n", "", instruction.c_str(), option.c_str());
						;
                    
                    //If LDB is used
                    if(instruction == "LDB"){
                        //fprintf(outFile, "%-9s %-11s %s \n", "", "BASE", data[2].c_str());
                    
                        if (format == 4) { //extended base storage
                            string sBaseAddr = objContent[i].substr(start + 4, 4); //takes the whole target address to use as base address
                            baseAddr = stringHexToInt(sBaseAddr);
                        }
                        if (format == 3) { 
                            string sDisp = objContent[i].substr(start + 3, 3); //takes displacement of object code to calculate base address
                            int disp = stringHexToInt(sDisp);
                            baseAddr = disp + textAddr; 
                        }
                    }
                    
                    // jump to next instruction
                    start = start + textSkip;
                    
                    // increment pc
                    textAddr = textAddr + format;
                }
            } 
        } 
    } 
    
    // This is for variable declaration, found a way to implement Leonard's proposal for resb and resw in class
    unsigned long size = directive.size(); //unsigned longs are better for character-related operations
    for (int i = 0; i < size; i++) {
        int address = directive.begin()-> first;
        string label = directive.begin()-> second;
        directive.erase(address);
        
        int nextAddr = 0;
        int storage = 0;
        
        if (directive.size() == 0) //if directive size is 0, find storage based on the last address of text record
            storage = lastAddr - address;
        else {
            nextAddr = directive.begin()->first; //otherwise calculate storage based on directive address
            storage = nextAddr - address;
        }
        
        if (storage % 3 == 0) { //modulus check for storage size
            storage = storage / 3;
            fprintf(outFile, "%-9s %-11s %d \n", label.c_str(), "RESW", storage);
        } else
            fprintf(outFile, "%-9s %-11s %d \n", label.c_str(), "RESB", storage);
    }
    
    //end record processing
    for (int i = 1; i < objContent.size(); i++) {
        if (objContent[i][0] == 'E') {
            int loadAddr = stringHexToInt(objContent[i].substr(1, 6)); //takes start address
            string symbol = symElement.find(loadAddr)->second; //finds original symbol
            //fprintf(outFile, "%-9s %-11s %s \n", "", "END", symbol.c_str());
        }
    }
	fclose(outFile); // close file.sic
    //add closing for listing file as well
}

vector<string> RecordProcess::fileFormat(int lineNumber, int start, int textAddr, int baseAddr){
    // start is instruction location
    // textAddr is current text record address, whenever we want to use the program counter, textAddr + given format allows us to calculate with displacement correctly
    
    vector<string> data(5);
     // purpose of each index of data
     //data[0] = instruction;
     //data[1] = skipBits;
     //data[2] = originalSym;
     //data[3] = addressed symbol;
     //data[4] = format
    
    op_code *check = new op_code();
    
    // Take the first 2 digits to put to getopcode
    string firstbits = objContent[lineNumber].substr(start,2);
    
    // get full opcode and validate it
    string op = getOpcode(firstbits);
    int optabIndex = validateOpcode(op);
    
    // Check Format and store its data
    if (optabIndex != -1) {
        string format = getFormat(optabIndex);
        // Format "3/4"
        if (format.length() > 1) {

            // Get NIXBPE
            string stringNixbpe1 = objContent[lineNumber].substr(start+1,1); //similar idea to snipping the last two bits in getOpcode, but to isolate the nixbpe bits
            int intNixbpe1 = stringHexToInt(stringNixbpe1);
            string binNixbpe1 = hexToStringBinary(intNixbpe1);
            
            string stringNixbpe2 = objContent[lineNumber].substr(start+2,1); //second bit of nixbpe
            int intNixbpe2 = stringHexToInt(stringNixbpe2);
            string binNixbpe2 = hexToStringBinary(intNixbpe2);
            
            string nixbpe = binNixbpe1.substr(2,2); //remove first two bits from the first hex digit
            nixbpe = strcat((char *)nixbpe.c_str(), (char *)binNixbpe2.c_str()); //rebuild full nixbpe string
            
            // Store Format 4 data
            if (nixbpe[5] == '1') {
                data[0] = getInstruction(optabIndex);
                data[1] = "8"; //length of format 4
                
                string sDirectAddr = objContent[lineNumber].substr(start+4, 4); //retrieve full address from instruction
                int directAddr = stringHexToInt(sDirectAddr);
                string symbol = symElement.find(directAddr)->second; //find symbol from address
                
                if(nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '1') //if indexed
                    symbol = symbol + ",X";
                else if(nixbpe[0] == '1' && nixbpe[1] == '0') //if indirect
                    symbol = "@" + symbol; 
                else if (nixbpe[0] == '0' && nixbpe[1] == '1' ) { //if immediate
                    // Check if the address is in Symbol Table:
                    if (symElement.find(directAddr) != symElement.end())
                        symbol = symElement.find(directAddr)->second;
                    else
                        symbol = integerToString(directAddr);
                    
                    symbol = "#" + symbol;
                }
                string originalSym = symElement.find(directAddr)->second;
                data[2] = originalSym; //original symbol used
                data[3] = symbol; //new formatted symbol
                data[4] = "4"; //format
            } 

            else {
                
                // Store Format 3 data
                data[0] = getInstruction(optabIndex);
                data[1] = "6"; //length of format 3
                
                string sDisp = objContent[lineNumber].substr(start+3, 3); //isolate displacement
                if (sDisp[0] == '8' || sDisp[0] == '9' || sDisp[0] == 'A' || sDisp[0] == 'B' || sDisp[0] == 'C' || sDisp[0] == 'D' || sDisp[0] == 'E' || sDisp[0] == 'F') { 
                    sDisp = "0x" + sDisp;
                }

                int disp = stringHexToInt(sDisp); //
                string symbol;
                string originalSym;
                int targetAddr = 0;

                //I hope you like if statements
                
                // run-of-the-mill pc relative
                if ((nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '0') || (nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '1')) {
                    
                    targetAddr = disp + textAddr + 3;
                    
                    // Check if targetAddr in symtab or littab
                    if (symElement.find(targetAddr) != symElement.end()) {
                        symbol = symElement.find(targetAddr)->second; //symbol is reformatted based on addressing modes, in this case pc-relative yields no changes
                        originalSym = symbol;
                    }
                    
                    if (litElement.find(targetAddr) != litElement.end()) {
                        symbol = litElement.find(targetAddr)->second;
                        originalSym = symbol;
                    }
                    
                    if (data[0].compare("RSUB") == 0) { //if RSUB is used, no symbol is used
                        symbol = "";
                        originalSym = "";
                    }
                }
                
                // Base relative
                if (nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '1' && nixbpe[4] == '0'){
                    targetAddr = disp + baseAddr;
                    symbol = symElement.find(targetAddr)->second;
                    originalSym = symbol;
                }
                
                // Indexed addressing
                if ((nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '1' && nixbpe[3] == '0' && nixbpe[4] == '0') || (nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '1' && nixbpe[3] == '0' && nixbpe[4] == '1')) {
                    if ((-2048 <= disp) && (disp <= 2047)) { // PC relative mode
                        targetAddr = disp + tAddr + 3;
                        symbol = symElement.find(targetAddr)->second;
                        originalSym = symbol;
                        symbol = symbol + ",X";
                    }
                    else { // base relative mode
                        targetAddr = disp + baseAddr;
                        symbol = symElement.find(targetAddr)->second;
                        originalSym = symbol;
                        symbol = symbol + ",X";
                    }
                }

                // Indexed with base addressing
                if ((nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '1' && nixbpe[3] == '1' && nixbpe[4] == '0')) {
                    targetAddr = disp + baseAddr;
                    symbol = symElement.find(targetAddr)->second;
                    originalSym = symbol;
                    symbol = symbol + ",X";
                }
                
                // Indirect addressing
                if ((nixbpe[0] == '1' && nixbpe[1] == '0' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '0') || (nixbpe[0] == '1' && nixbpe[1] == '0' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '1')) {
                    if ((-2048 <= disp) && (disp <= 2047)) { //PC relative mode
                        targetAddr = disp + tAddr + 3;
                        symbol = symElement.find(targetAddr)->second;
                        originalSym = symbol;
                        symbol = "@" + symbol;
                    }
                    else { // base relative mode
                        targetAddr = disp + baseAddr;
                        symbol = symElement.find(targetAddr)->second;
                        originalSym = symbol;
                        symbol = "@" + symbol;
                    }
                }
                // Indirect with base addressing
                if ((nixbpe[0] == '1' && nixbpe[1] == '0' && nixbpe[2] == '0' && nixbpe[3] == '1' && nixbpe[4] == '0')) {
                    targetAddr = disp + baseAddr;
                    symbol = symElement.find(targetAddr)->second;
                    originalSym = symbol;
                    symbol = "@" + symbol;
                }
                
                // Immediate addressing
                if ((nixbpe[0] == '0' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '0') || (nixbpe[0] == '0' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '1')) {
                    if ((-2048 <= disp) && (disp <= 2047)) { // PC relative mode
                        targetAddr = disp + tAddr + 3;
                        if (symElement.find(targetAddr) != symElement.end()) {
                            symbol = symElement.find(targetAddr)->second;
                            originalSym = symbol;
                        } else {
                            symbol = integerToString(disp);
                            originalSym = symbol;
                        }
                        symbol = "#" + symbol;
                    }
                    else { // base relative mode
                        targetAddr = disp + baseAddr;
                        if (symElement.find(targetAddr) != symElement.end()) {
                            symbol = symElement.find(targetAddr)->second;
                            originalSym = symbol;
                        } else {
                            symbol = integerToString(disp);
                            originalSym = symbol;
                        }
                        symbol = "#" + symbol;
                    }
                }
                // Immediate with base addressing
                if ((nixbpe[0] == '0' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '1' && nixbpe[4] == '0')) {
                    targetAddr = disp + baseAddr;
                    if (symElement.find(targetAddr) != symElement.end()) {
                        symbol = symElement.find(targetAddr)->second;
                        originalSym = symbol;
                    } else {
                        symbol = integerToString(disp);
                        originalSym = symbol;
                    }
                    symbol = "#" + symbol;
                }
                data[2] = originalSym;
                data[3] = symbol;
                data[4] = "3";
            }
        }
        else {
            // Format 1
            if (format[0] == '1') {
                data[0] = getInstruction(optabIndex);
                data[1] = "2"; //length of format 1
                data[4] = "1";
                
                // Format 2
            } else if (format[0] == '2'){
                data[0] = getInstruction(optabIndex);
                data[1] = "4"; //length of format 1
                data[2] = "2"; // 2 registers
                data[4] = "2";
            }
        }
    }
    return data;
}
