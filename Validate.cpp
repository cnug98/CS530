/*
 * Name: Colin Nugent
 * Red ID: 820151963
 * Class Username: cssc0440
 * Class Information: CS530, Spring 2019
 * Assignment #3 File Parser
 * Filename: Validate.cpp
 * Purpose: Processes each parsed line to check if grammar is valid, Returns results 
 */

#include "Validate.h"

bool Validate::validateDigit (char digit){
    for(int i = 0; i < 10; i++){
        if(charDigitTable[i].charDigitZeroToNine == digit)
            return true;
    }
    return false;
}

bool Validate::validateOperators (char operators){
    for(int i = 0; i < 5; i++){
        if(charOperatorsTable[i].charOperators == operators)
            return true;
    }
    return false;
}

bool Validate::validateCharacter(char character) {
    for(int i = 0; i < 52; i++) {
        if(characterTable[i].character_aToZ == character)
            return true;
    }
    return false;
}

bool Validate::validateID (string token) {
    // Check if the first char of token is letter
    if(validateCharacter(token[0])) {
        // Check the rest char of token if they are either letter or number
        for(int i = 1; i < token.length(); i++) {
            if(validateCharacter(token[i]))
                continue;
            else if(validateDigit(token[i]))
                continue;
            else {
                return false;
            }
        }
        return true;
    }
    return false;
}

vector<string> Validate::validateExp(vector<string> token){
    vector<string> result(2);
    stringstream ss;
    Validate *validate = new Validate();
    // Check if the first token is ID
    if (token[0] != " " && validate->validateID(token[0])) {
        // Check the next token is Op
        if (token[1].length() == 1 && validate->validateOperators(token[1][0])) {
            // Check if the last token is ID
            if (validate->validateID(token[token.size()-1])) {
                for (int i = 2; i < token.size()-1; i++) {
                    if (!(validate->validateID(token[i]))) {
                        result[0] = "0";
                        ss << "The token \"" << token[i] << "\" is not a valid ID.";
                        result[1] = ss.str();
                        return result;
                    }
                    i = i + 1;
                    if (!(validate->validateOperators(token[i][0]))) {
                        result[0] = "0";
                        ss << "The token \"" << token[i] << "\" is not a valid Operator.";
                        result[1] = ss.str();
                        return result;
                    }
                }
                result[0] = "1";
                result[1] = " ";
                return result;
            }
            result[0] = "0";
            ss << "The token \"" << token[token.size()-1] << "\" is not a valid ID.";
            result[1] = ss.str();
            return result;
        }
        result[0] = "0";
        ss << "The token \"" << token[1] << "\" is not a valid Operator.";
        result[1] = ss.str();
        return result;
    }
    result[0] = "0";
    ss << "The token \"" << token[0] << "\" is not a valid ID.";
    result[1] = ss.str();
    return result;
}

vector<string> Validate::validateAssg(vector<string> token){
    vector<string> result(2);
    stringstream ss;
    Validate *validate = new Validate();
    if (validate->validateID(token[0])) {// Check the first token is ID
        if (token[1] == "=") {// Check for = in second token
            // Check for ; at end of string
            if (token[token.size() - 1][0] == ';') {
                vector<string> theRest;
                for (int i = 2; i < token.size()-1; i++) {
                    theRest.push_back(token[i]);
                }
                return validateExp(theRest);
            }
            result[0] = "0";
            ss << "The last token \"" << token[token.size() - 1] << "\" is not ;."; //no ; found
            result[1] = ss.str();
            return result;
        }
        result[0] = "0";
        ss << "The second token \"" << token[1] << "\" is not =.";
        result[1] = ss.str();
        return result;
    }
    result[0] = "0";
    ss << "The first token \"" << token[0] << "\" is not a valid ID.";
    result[1] = ss.str();
    return result;
}
