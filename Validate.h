/*
 * Name: Colin Nugent
 * Red ID: 820151963
 * Class Username: cssc0440
 * Class Information: CS530, Spring 2019
 * Assignment #3 File Parser
 * Filename: Validate.h
 * Purpose: Contains header methods for Parser functions as well as structs which contain each possible digit, char and operator
 */

#ifndef __prog3__Validate__
#define __prog3__Validate__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Validate {

public:
    bool validateID (string token);
    vector<string> validateExp (vector<string> token);
    vector<string> validateAssg (vector<string> token);
    
private:
    static bool validateCharacter (char character);
    static bool validateDigit (char digit);
    static bool validateOperators (char operators);
};

struct charDigit {
    char charDigitZeroToNine;
};

const struct charDigit charDigitTable[] = {
    { '0' },   { '1' },
    { '2' },   { '3' },
    { '4' },   { '5' },
    { '6' },   { '7' },
    { '8' },   { '9' },
};

struct operators {
    char charOperators;
};

const struct operators charOperatorsTable [] = {
    { '+' },   { '-' },
    { '*' },   { '/' },
    { '%' },
};

struct characters {
    char character_aToZ;
};

const struct characters characterTable [] = {
    { 'a' },   { 'b' },    { 'c' },    { 'd' },
    { 'e' },   { 'f' },    { 'g' },    { 'h' },
    { 'i' },   { 'j' },    { 'k' },    { 'l' },
    { 'm' },   { 'n' },    { 'o' },    { 'p' },
    { 'q' },   { 'r' },    { 's' },    { 't' },
    { 'u' },   { 'v' },    { 'w' },    { 'x' },
    { 'y' },   { 'z' },    { 'A' },    { 'B' },
    { 'C' },   { 'D' },    { 'E' },    { 'F' },
    { 'G' },   { 'H' },    { 'I' },    { 'J' },
    { 'K' },   { 'L' },    { 'M' },    { 'N' },
    { 'O' },   { 'P' },    { 'Q' },    { 'R' },
    { 'S' },   { 'T' },    { 'U' },    { 'V' },
    { 'W' },   { 'X' },    { 'Y' },    { 'Z' },
};

#endif /* defined(__prog3__Validate__) */
