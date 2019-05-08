/*
 * Name: Colin Nugent
 * Red ID: 820151963
 * Class Username: cssc0440
 * Class Information: CS530, Spring 2019
 * Assignment #3 File Parser
 * Filename: Validate.cpp
 * Purpose: Processes each parsed line to check if grammar is valid, returns results 
 */

#include "Validate.h"

bool Validate::validateDigit (char digit){
    for(int i = 0; i < 10; i++){ //cycles through all digits to see if the given digit is valid
        if(charDigitTable[i].charDigitZeroToNine == digit)
            return true;
    }
    return false;
}

bool Validate::validateOperators (char op){
    for(int i = 0; i < 5; i++){ //cycles through all operators to see if the given operator is valid
        if(charOperatorsTable[i].charOperators == op)
            return true;
    }
    return false;
}

bool Validate::validateCharacter(char character) {
    for(int i = 0; i < 52; i++) { //cycles through all chars to see if the given char is valid
        if(characterTable[i].character_aToZ == character)
            return true;
    }
    return false;
}

bool Validate::validateID (string token) {
    if(validateCharacter(token[0])) { // Check if the first char of token is letter
        for(int i = 1; i < token.length(); i++) { // scan the rest of the token for valid chars and digits
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
    if (token[0] != " " && validate->validateID(token[0])) { // Check for id op id format
        if (token[1].length() == 1 && validate->validateOperators(token[1][0])) {
            if (validate->validateID(token[token.size()-1])) {
                for (int i = 2; i < token.size()-1; i++) {
                    if (!(validate->validateID(token[i]))) { //check for ID
                        result[0] = "0";
                        ss << "The token \"" << token[i] << "\" is not a valid ID";
                        result[1] = ss.str();
                        return result;
                    }
                    i = i + 1; //if the statement scanned was an id, skip over it and scan the op next to it if it exists
                    if (!(validate->validateOperators(token[i][0]))) {
                        result[0] = "0";
                        ss << "The token \"" << token[i] << "\" is not a valid operator";
                        result[1] = ss.str();
                        return result;
                    }
                }
                result[0] = "1";
                result[1] = " ";
                return result;
            }
            result[0] = "0";
            ss << "The token \"" << token[token.size()-1] << "\" is not a valid ID"; //otherwise the last token is not a valid ID
            result[1] = ss.str();
            return result;
        }
        result[0] = "0";
        ss << "The token \"" << token[1] << "\" is not a valid operator"; //otherwise the second token is not an op
        result[1] = ss.str();
        return result;
    }
    result[0] = "0";
    ss << "The token \"" << token[0] << "\" is not a valid ID"; //if no valid ID is found first, the first token is invalid
    result[1] = ss.str();
    return result;
}

vector<string> Validate::validateAssg(vector<string> token){
    vector<string> result(2);
    stringstream ss;
    Validate *validate = new Validate();
    if (validate->validateID(token[0])) {// Check the first token for ID
        if (token[1] == "=") {// Check for = in second token
            if (token[token.size() - 1][0] == ';') { //check for semicolon at end of statement
                vector<string> theRest; //if true the rest of the statement is an expression
                for (int i = 2; i < token.size()-1; i++) {
                    theRest.push_back(token[i]);
                }
                return validateExp(theRest); //process the rest of the tokens as an expression
            }
            result[0] = "0";
            ss << "The last token \"" << token[token.size() - 1] << "\" is not a ;"; //no ; found
            result[1] = ss.str();
            return result;
        }
        result[0] = "0";
        ss << "The second token \"" << token[1] << "\" is not an ="; //incorrect use of = assignment indicator
        result[1] = ss.str();
        return result;
    }
    result[0] = "0";
    ss << "The first token \"" << token[0] << "\" is not a valid ID"; //otherwise the token isn't a proper ID
    result[1] = ss.str();
    return result;
}
