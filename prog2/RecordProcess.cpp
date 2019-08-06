/*
 * Members: Colin Nugent, Carter Andrews, Patrick Perrine
 * Red ID's: 820151963, 820214623, and 820486635
 * Class Users: cssc0440, cssc0425, cssc0426
 * Class Information: CS530, Spring 2019
 * Assignment #2, SIC/XE Disassembler
 * Filename: RecordProcess.cpp
 * Purpose: Performs the main processing of the .obj file into the .sic 
 */

#include "RecordProcess.h"
#include "opcode.h"
#include "convert.h"
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm> 

using namespace std;
//I used my own method to read in the files, feel free to either use this new method or repurpose run's parameters to use with xed.cpp
void RecordProcess::readFile(string fileName){
	int MAX_INPUT = 10000;
    char *input = new char[MAX_INPUT]();
    char *objName = new char[MAX_INPUT]();
    char *symtabName = new char[MAX_INPUT]();
	char *sicFileName = new char[MAX_INPUT]();
	char *lisFileName = new char[MAX_INPUT]();
    //fileName =  new char[MAX_INPUT]();
    string line;
    // Read file name
    
    // Assign extension to file name
    strcat(symtabName, fileName.c_str());
    strcat(symtabName, ".sym"); // read in file.sym
    strcat(objName, fileName.c_str());
    strcat(objName, ".obj");    // read in file.obj
	strcat(sicFileName, fileName.c_str());
	strcat(sicFileName, ".sic");
	strcat(lisFileName, fileName.c_str());
	strcat(lisFileName, ".lis");
    // Open obj file and assign each line to vector objContent.
	FILE* outFile= fopen(sicFileName, "w+");
	FILE* lisFile = fopen(lisFileName, "w+");
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
				//cout << line << endl;
            }
            symfile.close();
        } else printf("Unable to process sym file \n");
        
        RecordProcess::run(outFile,lisFile);
    }
    else printf("Unable to proccess obj file.\n");
}
string findSymbol(int address, map<int, string> table) {
	string symbol = integerToString(address);
	if (table.find(address) != table.end())
		symbol = table.find(address)->second;
	return symbol;
}
string padLeftZeroes(int value) {
	stringstream stream;
	stream << hex << value;
	string result(stream.str());
	string starter = result;
	string out = starter;
	//cout << out << endl;
	if (out.length() < 4)
		for (int i = 0;i < 4 - starter.length();i++) {
			out = "0" + out;
			//cout << "there" << endl;
		}
	else
		out = out.substr(0, 4);
	transform(out.begin(), out.end(), out.begin(), ::toupper);
	out.append("  ");
	return out;
}

// 
void RecordProcess::run(FILE* outFile,FILE* lisFile){ 
    //symContent and objContent are vectors, while litElement, symElement and directive are all map<int,string> with an iterator (refer to header)
	string progName= objContent[0].substr(1, 6);
	string progStart = objContent[0].substr(7, 6);
	string progSize = objContent[0].substr(13, 6);
	int progEnd = stringHexToInt(progStart) + stringHexToInt(progSize);
	//cout << progName << endl;
	//cout << progStart << endl;
	//cout << progSize << endl;
	char outputText[50];
	sprintf(outputText, "%s %s %s ", progName.c_str(), "   START      ",progStart.c_str());
	fprintf(outFile, "%s \n", outputText);
	progStart = progStart.substr(2, 4);
	progStart = progStart.append("  ");
	fprintf(lisFile, "%s %-9s \n",progStart.c_str(), outputText);
	symContent.erase(symContent.begin(), symContent.begin() + 2);
	while (symContent[0].length()>3) {
		//cout << symContent[0] << endl;
		string symbol = symContent[0].substr(0, 6);
		int symAddr = stringHexToInt(symContent[0].substr(8, 6));
		//cout << integerToString(symAddr) << "   " << symbol << endl;
		symElement[symAddr] = symbol;
		directive[symAddr] = symbol;
		symContent.erase(symContent.begin());
	}
	symContent.erase(symContent.begin(), symContent.begin() + 3);
	while (symContent[0].length() > 3) {
		string literal = symContent[0].substr(8,6 );
		
		int litAddr = stringHexToInt(symContent[0].substr(24,6));
		litElement[litAddr] = literal;
		symContent.erase(symContent.begin());
	}
	int lastAddr;
    for (int i = 1; i < objContent.size(); i++) {
        if (objContent[i][0] == 'T') {

            int tAddr = stringHexToInt(objContent[i].substr(1,6));   // text record address
            int tLength = stringHexToInt(objContent[i].substr(7,2)); // text record length
            lastAddr = tAddr + tLength;
            int start = 9; //location of first text record instruction
            int jump = 0; //where to jump to the next instruction
            int baseAddr = 0; //storage for base address
            
            while (tAddr < lastAddr) {
                
                /*Portions of data vector
                 data[0] = instruction
                 data[1] = skipDigits (total bits to skip to get next instruction)
                 data[2] = originalSym (original symbol used)
                 data[3] = option (symbol with given addressing modes)
                 data[4] = format
                 */
                
                vector<string> data = printFormat(i,start,tAddr, baseAddr); //data can contain everything the file printing system needs at once
                
                string instruction = data[0];
                string skipDigits = data[1];
                jump = stringHexToInt(skipDigits); //formats skipDigits for jump to move to next instruction
                string option = data[3];
                int format = stringHexToInt(data[4]);
                
                // Check if address in symtab and littab
                string hasSym = ""; 
                if (symElement.find(tAddr) != symElement.end()) {
                    hasSym = symElement.find(tAddr)->second; //find symbol name
                }
                
                string hasLit = "";
                if (litElement.find(tAddr) != litElement.end()) {
                    hasLit = litElement.find(tAddr)->second; //if there's a literal, find it as well
                }
                if (hasSym.length() > 0) { //symbol found
                    //prints instruction
					if (format == 4) { //extended situation
						char outputText[50];
						sprintf(outputText,"%-9s %-11s %-5s %s %s", hasSym.c_str(), "","+", instruction.c_str(), option.c_str()); 
						fprintf(outFile, "%s \n",outputText);
						while (strlen(outputText) < 42)
							strcat(outputText, " ");
						string opcodeText=objContent[i].substr(start, 8);
						const char* adressText =  padLeftZeroes(tAddr).c_str();
						fprintf(lisFile, "%s %-9s %s  \n",adressText, outputText,opcodeText.c_str()); 

						;
					}
                                        else if (format == 1) { //original format 1 & 2 possibilities, never implemented correctly
                                            sprintf(outputText, "%-9s %s ", instruction.c_str());
                                        }
                                        else if (format == 2) {
                                            string fullReg = objContent[i].substr(start+2,2);
                                            string reg1 = fullReg.substr(0,1);
                                            string reg2 = fullReg.substr(1,1);
                                            
                                            
                                            reg1 = getRegister(reg1);
                                            if(!data[0].compare("SHIFTR") || !data[0].compare("SHIFTL")) //treat register 2 as the n operator
                                                reg2 = reg2;
                                            
                                            else{
                                                reg2 = getRegister(reg2);
                                            }
                                            
                                            if(!data[0].compare("CLEAR") || !data[0].compare("TIXR")) //format 2 instructions that only use register 1
                                                    sprintf(outputText, "%-9s %-11s %s ", instruction.c_str(), reg1.c_str());
                                                    
                                            string trueReg = reg1 + ',' + reg2;
                                            sprintf(outputText, "%-9s %-11s %s ", instruction.c_str(), trueReg.c_str());
                                            
                                        }
					else { //otherwise just print instruction with no additional symbols
						//WE NEED TO ADD FORMAT 1 and 2 formatting here
						char outputText[50];
						sprintf(outputText, "%-9s %-11s %s ", hasSym.c_str(), instruction.c_str(), option.c_str());
						fprintf(outFile, "%s \n", outputText);
						while (strlen(outputText) < 42)
							strcat(outputText, " ");
						string opcodeText = objContent[i].substr(start, 6);
						const char* adressText = padLeftZeroes(tAddr).c_str();
						fprintf(lisFile, "%s %-9s %s \n", adressText, outputText, opcodeText.c_str());
						
					}
                    // Delete used symbol
                    directive.erase(tAddr);
                    
                    // jump to next instruction
                    start = start + jump;
                    
                    // increment pc
                    tAddr = tAddr + format;
                }
                
                else if ((hasLit.length() > 0) && (litElement.begin()->second == hasLit)) { //literal found

					if (format == 4) { //extended situation
						char outputText[50];
						sprintf(outputText, "%-8s %s%-11s %s ", "", "+", instruction.c_str(), hasLit.c_str());
						fprintf(outFile, "%s \n", outputText);
						while (strlen(outputText) < 42)
							strcat(outputText, " ");
						string opcodeText = objContent[i].substr(start, 8);
						const char* adressText = padLeftZeroes(tAddr).c_str();
						fprintf(lisFile, "%s %-9s %s \n", adressText, outputText, opcodeText.c_str());

						;
					}
					else { //otherwise just print instruction with no additional symbols

						char outputText[50];
						sprintf(outputText, "%-9s %-11s %s ", "", instruction.c_str(), hasLit.c_str());
						fprintf(outFile, "%s \n", outputText);
						while (strlen(outputText) < 42)
							strcat(outputText, " ");
						string opcodeText = objContent[i].substr(start, 6);
						const char* adressText = padLeftZeroes(tAddr).c_str();
						fprintf(lisFile, "%s %-9s %s \n", adressText, outputText, opcodeText.c_str());

					}
					char outputText[50];
					sprintf(outputText, "%-9s %-11s ", "", "LTORG");
					fprintf(outFile, "%s \n", outputText);
					while (strlen(outputText)<42)
						strcat(outputText, " ");
					string opcodeText = objContent[i].substr(start, 6);
					const char* adressText = padLeftZeroes(tAddr).c_str();
					fprintf(lisFile, "%s %-9s \n", adressText, outputText);
                    jump = 8;
                    
                    // jump to next instruction
                    start = start + jump;
                    
                    // increment pc
                    tAddr = tAddr + 4;
                }
                
               
                else{ //if neither is found
					if (format == 4) { //extended
						char outputText[50];
						sprintf(outputText, "%-8s %s%-11s %s ", "", "+", instruction.c_str(), option.c_str());
						fprintf(outFile, "%s \n", outputText);
						while (strlen(outputText) < 42)
							strcat(outputText, " ");
						string opcodeText = objContent[i].substr(start, 8);
						const char* adressText = padLeftZeroes(tAddr).c_str();
						fprintf(lisFile, "%s %-9s %s \n", adressText, outputText, opcodeText.c_str());
					}
					else {
						//WE NEED TO ADD FORMAT 1 and 2 formatting here
						char outputText[50];
						sprintf(outputText, "%-9s %-11s %s ", "", instruction.c_str(), option.c_str());
						fprintf(outFile, "%s \n", outputText);
						while (strlen(outputText) < 42)
							strcat(outputText, " ");
						string obcodeText = objContent[i].substr(start, 6);
						const char* adressText = padLeftZeroes(tAddr).c_str();
						fprintf(lisFile, "%s %-9s %s \n", adressText, outputText, obcodeText.c_str());
					}
                    //If LDB is used
                    if(instruction == "LDB"){
						char outputText[50];
						sprintf(outputText, "%-9s %-11s %s ", "", "BASE", data[2].c_str());
						fprintf(outFile, "%s \n", outputText);
						const char* adressText = padLeftZeroes(tAddr).c_str();
						fprintf(lisFile, "%s %-9s   \n", adressText, outputText);
                        if (format == 4) { //extended base storage
                            string sBaseAddr = objContent[i].substr(start + 4, 4); //takes the whole target address to use as base address
                            baseAddr = stringHexToInt(sBaseAddr);
                        }
                        if (format == 3) { 
                            string sDisp = objContent[i].substr(start + 3, 3); //takes displacement of object code to calculate base address
                            int disp = stringHexToInt(sDisp);
                            baseAddr = disp + tAddr; 
                        }
                    }
                    
                    // jump to next instruction
                    start = start + jump;
                    
                    // increment pc
                    tAddr = tAddr + format;
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
        
		if (directive.size() == 0) { //if directive size is 0, find storage based on the last address of text record
			storage = progEnd - address;
			//cout << integerToString(lastAddr) << endl;
			//cout << integerToString(address) << endl;
		}
        else {
            nextAddr = directive.begin()->first; //otherwise calculate storage based on directive address
            storage = nextAddr - address;
        }
		
        if (storage % 3 == 0) { //modulus check for storage size
            storage = storage / 3;
			
			char outputText[50];
			sprintf(outputText, "%-9s %-11s %d ", label.c_str(), "RESW", storage);
			fprintf(outFile, "%s \n", outputText);
			const char* adressText = padLeftZeroes(address).c_str();
			fprintf(lisFile, "%s %-9s \n", adressText, outputText);
		}
		else {
			char outputText[50];
			sprintf(outputText, "%-9s %-11s %d ", label.c_str(), "RESB", storage);
			fprintf(outFile, "%s \n", outputText);
			const char* adressText = padLeftZeroes(address).c_str();
			fprintf(lisFile, "%s %-9s \n", adressText, outputText);
		}
	
    }
    
    //end record processing
    for (int i = 1; i < objContent.size(); i++) {
        if (objContent[i][0] == 'E') {
            int loadAddr = stringHexToInt(objContent[i].substr(1, 6)); //takes start address
			string symbol = findSymbol(loadAddr,symElement); //finds original symbol
        
			char outputText[50];
			sprintf(outputText, "%-9s %-11s %s ", "", "END", symbol.c_str());
			fprintf(outFile, "%s \n", outputText);
			fprintf(lisFile, " %s %-9s \n","     ", outputText);
        }
    }
	fclose(outFile); // close file.sic
	fclose(lisFile); //add closing for listing file as well

}

vector<string> RecordProcess::printFormat(int lineNumber, int start, int tAddr, int baseAddr){
    // start is instruction location
    // tAddr is current text record address, whenever we want to use the program counter, tAddr + given format allows us to simulate the program counter
    vector<string> data(5);
     // purpose of each index of data
     //data[0] = instruction;
     //data[1] = skipDigits;
     //data[2] = originalSym;
     //data[3] = symbol for option field;
     //data[4] = format
    
    op_code *check = new op_code();
    
    // Take the first 2 digits to put to getopcode
    string first2 = objContent[lineNumber].substr(start,2);
	//cout << first2 << endl;
    // get full opcode and validate it
    string op = getOpcode(first2);
	//cout << op << endl;
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
            
            string nixbpe = binNixbpe1.substr(2,2); //remove first two bits
            nixbpe = strcat((char *)nixbpe.c_str(), (char *)binNixbpe2.c_str()); //rebuild full nixbpe string
			//cout << nixbpe << endl;
            // Store Format 4 data
            if (nixbpe[5] == '1') {
                data[0] = getInstruction(optabIndex);
                data[1] = "8"; //length of format 4
                
                string sDirectAddr = objContent[lineNumber].substr(start+4, 4); //retrieve full address from instruction
                int directAddr = stringHexToInt(sDirectAddr);
				string symbol = findSymbol(directAddr,symElement);
                                for(int i = 0; i < symbol.length(); i++){
                                    if(symbol[i] == ' ')
                                        symbol = symbol.substr(0, i);
                                }
                
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
				string originalSym = integerToString(directAddr);
				if (symElement.count(directAddr) != 0)
					originalSym = symElement.find(directAddr)->second; //find symbol from address
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

                int disp = stringHexToInt(sDisp);
                string symbol;
                string originalSym;
                int targetAddr = 0;

                //I hope you like if statements
                
                // run-of-the-mill pc relative
                if ((nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '0') || (nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '1')) {
                    
                    targetAddr = disp + tAddr + 3;
                    
                    // Check if targetAddr in symtab or littab
                    if (symElement.find(targetAddr) != symElement.end()) {
						symbol = findSymbol(targetAddr,symElement);
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
					symbol = findSymbol(targetAddr,symElement);
                    originalSym = symbol;
                }
                
                // Indexed addressing
                if ((nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '1' && nixbpe[3] == '0' && nixbpe[4] == '0') || (nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '1' && nixbpe[3] == '0' && nixbpe[4] == '1')) {
                    if ((-2048 <= disp) && (disp <= 2047)) { // PC relative mode
                        targetAddr = disp + tAddr + 3;
                        symbol = findSymbol(targetAddr, symElement);
                        for(int i = 0; i < symbol.length(); i++){
                                    if(symbol[i] == ' ')
                                        symbol = symbol.substr(0, i);
                        }
                        originalSym = symbol;
                        symbol = symbol + ",X";
                    }
                    else { // base relative mode
                        targetAddr = disp + baseAddr;
                        symbol = findSymbol(targetAddr, symElement);
                        for(int i = 0; i < symbol.length(); i++){
                                    if(symbol[i] == ' ')
                                        symbol = symbol.substr(0, i);
                        }
                        originalSym = symbol;
                        symbol = symbol + ",X";
                    }
                }

                // Indexed with base addressing
                if ((nixbpe[0] == '1' && nixbpe[1] == '1' && nixbpe[2] == '1' && nixbpe[3] == '1' && nixbpe[4] == '0')) {
                    targetAddr = disp + baseAddr;
                    symbol = findSymbol(targetAddr, symElement);
                    for(int i = 0; i < symbol.length(); i++){
                                    if(symbol[i] == ' ')
                                        symbol = symbol.substr(0, i);
                    }
                    originalSym = symbol;
                    symbol = symbol + ",X";
                }
                
                // Indirect addressing
                if ((nixbpe[0] == '1' && nixbpe[1] == '0' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '0') || (nixbpe[0] == '1' && nixbpe[1] == '0' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '1')) {
                    if ((-2048 <= disp) && (disp <= 2047)) { //PC relative mode
                        targetAddr = disp + tAddr + 3;
                        symbol = findSymbol(targetAddr, symElement);
                        originalSym = symbol;
                        symbol = "@" + symbol;
                    }
                    else { // base relative mode
                        targetAddr = disp + baseAddr;
                        symbol = findSymbol(targetAddr, symElement);
                        originalSym = symbol;
                        symbol = "@" + symbol;
                    }
                }
                // Indirect with base addressing
                if ((nixbpe[0] == '1' && nixbpe[1] == '0' && nixbpe[2] == '0' && nixbpe[3] == '1' && nixbpe[4] == '0')) {
                    targetAddr = disp + baseAddr;
                    symbol = findSymbol(targetAddr, symElement);
                    originalSym = symbol;
                    symbol = "@" + symbol;
                }
                
                // Immediate addressing
                if ((nixbpe[0] == '0' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '0') || (nixbpe[0] == '0' && nixbpe[1] == '1' && nixbpe[2] == '0' && nixbpe[3] == '0' && nixbpe[4] == '1')) {
                    if ((-2048 <= disp) && (disp <= 2047)) { // PC relative mode
                        targetAddr = disp + tAddr + 3;
                        if (symElement.find(targetAddr) != symElement.end()) {
                            symbol = findSymbol(targetAddr, symElement);
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
                            symbol = findSymbol(targetAddr, symElement);
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
                        symbol = findSymbol(targetAddr, symElement);
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
                data[1] = "2";
                data[4] = "1";
                
                // Format 2
            } else if (format[0] == '2'){
                data[0] = getInstruction(optabIndex);
                data[1] = "4";
                data[2] = "2"; // 2 registers
                data[4] = "2";
            }
        }
    }
    return data;
}

