/*
 * Name: Colin Nugent
 * Red ID: 820151963
 * Class Username: cssc0440
 * Class Information: CS530, Spring 2019
 * Assignment #3 File Parser
 * Filename: Parser.h
 * Purpose: Contains header methods for Parser functions
 */

#include <iostream>
#include <fstream>
#include "Validate.h"
#include <cstring>
#include <string>

class Parser{
public:
    void getFile();
    void run();
    
private:
    vector<string> chooseVal(vector<string> token);
    vector<string> textContent;
};

