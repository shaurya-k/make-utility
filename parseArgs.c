////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: parseArgs.c
//
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "parseArgs.h"

// default args if none specified
argStruct defaultArg() {
    //this method sets a new object with the correct initializations
    argStruct argStruct;
    argStruct.connections = (char **) malloc(4096 * sizeof(char *));
    argStruct.connections[0] = NULL;
    argStruct.output = NULL;
    return argStruct;
}

// iterates through the args
argStruct getNextArg(int argc, char **argv) {
    int opt;
    int passed = 0;
    argStruct nextArg = defaultArg();
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        // if -f is specified
        switch (opt) {
            case 'f':
                passed = 1;
                nextArg.output = optarg;
                break;
            default: // Error Input Option
                fprintf(stderr, "Usage: %s [-f <file_name>]\n", argv[0]);
                exit(1);
        }
    }
    // check which itterate num to use
    if (argc > optind) {
        int iterate;
        if (passed) {
            iterate = 3;
        } else {
            iterate = 1;
        }
        int ind = 0;
        // iterate
        for (; iterate < argc; ++iterate) {
            nextArg.connections[ind] = argv[iterate];
            ind++;
        }
        nextArg.connections[ind] = NULL;
    }
    return nextArg;
}
