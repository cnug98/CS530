#Members: Colin Nugent, Carter Andrews, Patrick Perrine
#Red ID's: 820151963, 820214623, and 820486635
#Class Users: cssc0440, cssc0425, cssc0426
#Class Information: CS530, Spring 2019
#Assignment #2, SIC/XE Disassembler
#Filename: makefile

CC=g++
CFLAGS=-c
SOURCES=xed.cpp opcode.cpp convert.cpp RecordProcess.cpp  #groups in all non-header files
OBJECTS=$(SOURCES:.cpp=.o)
EXEC=xed #xed is the only exec file

all: $(SOURCES) $(EXEC) 

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o core $(EXEC)
