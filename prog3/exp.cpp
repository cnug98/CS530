/*
 * Name: Colin Nugent
 * Red ID: 820151963
 * Class Username: cssc0440
 * Class Information: CS530, Spring 2019
 * Assignment #3 File Parser
 * Filename: exp.cpp
 * Purpose: Main executable for program
 */

#include <iostream>
#include "Parser.h"
#include "Validate.h"

int main(int argc, const char * argv[])
{
    Parser *test = new Parser;
    test -> getFile();
    return 0;
}

