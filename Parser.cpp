/*
 * Name: Colin Nugent
 * Red ID: 820151963
 * Class Username: cssc0440
 * Class Information: CS530, Spring 2019
 * Assignment #3 File Parser
 * Filename: Parser.cpp
 * Purpose: Breaks down read file to individually validate
 */

#include "Parser.h"

void Parser::getFile() {
    char *input = new char[50]();
    char *inputFile = new char[50]();
    char *fileName =  new char[50]();
    string line;
    
    printf("Enter a text file name: \n");
    
    // Read text file name.
    if (fgets(input, 50, stdin) != NULL) {
        sscanf(input, "%s", fileName);
    }
    else {
        printf("Input file name.\n");
    }
    
    strcat(inputFile, fileName);
    strcat(inputFile, ".txt");
    
    
    // Open text file and assign each line to vector textContent.
    ifstream textFile(inputFile);
    if (textFile.is_open()) {
        while (textFile.good()) {
            getline(textFile,line);
            textContent.push_back(line);
        }
        textFile.close();
        // After reading file successfully. Run program:
        Parser::run();
    } else printf("Unable to open text file.\n");
}

void Parser::run() {
    
    // Open output file
    char *outFileName = new char[50]();
    strcat(outFileName, "out.txt");
    ofstream outfile(outFileName);
    FILE *outFile = fopen(outFileName, "wb");

    bool pass = true;
    fprintf(outFile, "Line                          Input                                   Validation \n");
    for(int i = 0; i < textContent.size(); i++) {
        // If line is not empty then proceed
        if(!textContent[i].empty()) {
            // Tokenize to string named token
            vector<string> token;
            istringstream f(textContent[i]);
            string s;
            while (getline(f, s, ' ')) {
                token.push_back(s);
            }
            
            // Write to out.txt
            vector<string> temp = chooseVal(token);
            printf(" %s \n",textContent[i].c_str());
            printf(" %s \n",temp[0].c_str());
            printf(" %s \n",temp[1].c_str());
            //fprintf(outFile, "%3d: %-60s  %-20s %-20s \n", i, textContent[i].c_str(), temp[0].c_str(), temp[1].c_str());
            // If there is an invalid instruction then set pass = false
            if (temp[0].length() > 16) {
                pass = false;
            }
        }
        else{
	        fprintf(outFile, "%3d: \n", i);
        }
    }
    printf("\n => File Validation: %s", pass ? "Pass":"Fail");
    printf(" \n");
    fprintf(outFile,"\n => File Validation: %s", pass ? "Pass":"Fail");
    //printf("Done parsing! Written output to out.txt successfully! \n");
    outfile.close();
}
// Choose an approriate validation for each instruction
vector<string> Parser::chooseVal(vector<string> token) {
    vector<string> result(2), temp(2);
    stringstream ss;
    Validate *validate = new Validate;
    for (int i = 0; i < token.size(); i++) {
        if (token[i] == "=") { //if an = exists check assignments
            temp = validate->validateAssg(token);
            if (temp[0] == "1"){
                result[0] = "Valid Assignment.";
                result[1] = " ";
                return result;
            }
            result[0] = "Invalid Assignment.";
            result[1] = temp[1];
            return result;
        }
    }
    temp = validate->validateExp(token); //otherwise check for valid expressions
    if (temp[0] == "1") {
        result[0] = "Valid Expression.";
        result[1] = " ";
        return result;
    }
    result[0] = "Invalid Expression.";
    result[1] = temp[1];
    return result;
}

