#Name: Colin Nugent
#RedID: 820151963
#Class Username: cssc0440
#Class Information: CS530, Spring 2019
#Assignment #3 File Parser
#Filename: makefile

CC = g++
CFLAGS = -g
LDFLAGS = -lm

exp : exp.o Parser.o Validate.o
	${CC} ${CFLAGS} exp.o Parser.o Validate.o ${LDFLAGS} -o exp

Parser.o : Parser.cpp
	${CC} ${CFLAGS} -c Parser.cpp

Validate.o : Validate.cpp 
	${CC} ${CFLAGS} -c Validate.cpp
	
exp.o : exp.cpp
	${CC} ${CFLAGS} -c exp.cpp

clean: 
	rm -f exp *.o
