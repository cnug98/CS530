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
    string line;
    ifstream textFile("ex.txt"); //open ex.txt for tokenization
    if (textFile.is_open()) {
        while (textFile.good()) {
            getline(textFile,line);
            textContent.push_back(line); //assign each line to textContent
        }
        textFile.close();
        Parser::run();
    } else printf("Unable to open text file.\n");
}

void Parser::run() {
    bool pass = true;
    for(int i = 0; i < textContent.size(); i++) {
        if(!textContent[i].empty()) {
            vector<string> token;
            istringstream f(textContent[i]);
            string s;
            while (getline(f, s, ' ')) { //tokenizes each token into a string vector
                token.push_back(s);
            }           
            vector<string> temp = getVal(token);
            printf("Line %d:", i + 1); //print line #
            printf(" %s \n",textContent[i].c_str()); //prints the full line, if the grammar is valid, and why it did not pass the test if applicable
            printf(" %s \n",temp[0].c_str());          
            printf(" %s \n",temp[1].c_str());
            if (temp[0].length() > 16) //if an invalid instruction is found, the file fails inspection
                pass = false; 
        }

    }
    printf("\n File Validation: %s", pass ? "Pass":"Fail");
    printf(" \n");
}
vector<string> Parser::getVal(vector<string> token) { //retrieves the validity of each statement
    vector<string> result(2), temp(2);
    stringstream ss;
    Validate *validate = new Validate;
    for (int i = 0; i < token.size(); i++) {
        if (token[i] == "=") { //if an = exists check assignments
            temp = validate->validateAssg(token);
            if (temp[0] == "1"){
                result[0] = "Valid assignment";
                result[1] = " ";
                return result;
            }
            result[0] = "Invalid assignment";
            result[1] = temp[1];
            return result;
        }
    }
    temp = validate->validateExp(token); //otherwise check for expressions
    if (temp[0] == "1") {
        result[0] = "Valid expression";
        result[1] = " ";
        return result;
    }
    result[0] = "Invalid expression";
    result[1] = temp[1];
    return result;
}

