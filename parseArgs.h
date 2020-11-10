////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: parseArgs.h
//
////////////////////////////////////////////////////////////////////////////////
#ifndef PARSEARGS
#define PARSEARGS

typedef struct argStruct {
    char* output;
    char** connections;
} argStruct;

argStruct defaultArg();

argStruct getNextArg(int argc, char **argv);

#endif
