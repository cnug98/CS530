# CS530Prog3

* Name: Colin Nugent
* RedID: 820151963
* Username: cssc0440
* Class: CS530, Spring 2019
* Assignment #3: Grammar Parsing

## Files Used
* Main Program: exp.cpp (no header file used)
* Primary File Processing: Validate.cpp, Validate.h
* Assistant Files: Parser.cpp, Parser.h (breaks up tokens to validate individually)
* Compiler: makefile
* Test Files: ex.txt (original test file from Blackboard, please replace this to test with alternate files)

## Compiler + Operation Instructions
* First, enter "make" (I used Putty to test on edoras, where the "%" character does not need to show before each command. If this is the case on the grader's end, accomodate for this accordingly for each step ) to compile all files given.
* Second, enter "exp". The program will automatically look for a file named "ex.txt" in the same folder (the test files must be named "ex.txt" or this program will not be able to find the files, this was the way the test files were supposed to be named). The output will be printed to the console, and no alternate file outputs are used.

## Use of Abstract Design Decisions
* Other than the use of vectors, which I used for the previous assignment, no other unorthodox methods were used to accomplish this assignment.

## Known Faults / Bugs
* Due to time constraints, parantheses cases were not fully implemented, and the program may interpret certain IDs and edge cases involving parentheses as invalid, regardless of if the ID was actually valid or not. Otherwise all other basic op, id, expression and assignment checks should be working optimally.
