
////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: reader.h
//
////////////////////////////////////////////////////////////////////////////////
#ifndef READER
#define READER
#include "buildSpecGraph.h"
#include "parseArgs.h"

void checkTarget(char *line, unsigned int size, int numOfLines);

void checkCommand(char *line, unsigned int size, int numOfLine);

char *whiteSpace(char *str);

void reader(argStruct obj);

#endif
