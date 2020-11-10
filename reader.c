////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: reader.c
//
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "reader.h"
#include "buildSpecGraph.h"

// check if target is valid
void checkTarget(char *line, unsigned int size, int numOfLines) {
    if (line[0] == ' ') {
        fprintf(stderr, "%d Wrong format: %s", numOfLines, line);
        exit(1);
    }
    if (size == 0) {
        fprintf(stderr, "%d Wrong format: %s", numOfLines, line);
        exit(1);
    }
    int col = 0;
    int space = 0;
    int charNum = 0;

    // for each char
    for (unsigned int i = 0; i < size; i++) {
        // look for colonn
        if (line[i] == ':') {
            col++;
        } else if(line[i] == ' '){
            space++;
        } else{
            if(col == 0 && space > 0){
                fprintf(stderr, "%d Wrong format: %s", numOfLines, line);
                exit(1);
            }
            if(charNum == 0 && col > 0){ //no target
                fprintf(stderr, "%d Wrong format: %s", numOfLines, line);
                exit(1);
            }
            charNum++;
        }
    }
    if(charNum == 0){
        fprintf(stderr, "%d Wrong format: %s", numOfLines, line);
        exit(1);
    }
    if (col != 1) {
        fprintf(stderr, "%d Wrong format: %s", numOfLines, line);
        exit(1);
    }
}


// check if command is valid
void checkCommand(char *line, unsigned int size, int numOfLine) {
    if (size == 0) {
        fprintf(stderr, "%d Wrong format: %s", numOfLine, line);
        exit(1);
    }
    if(line[0] != '\t'){
        fprintf(stderr, "%d Wrong format: %s", numOfLine, line);
        exit(1);
    }
}

// remove whitespace from string
char *whiteSpace(char *str) {
    char *refactored = malloc(MAX_DEP * sizeof(char));
    int i = 0;
    while (*str != '\0') {
        if (*str != ' ') {
            *(refactored + i++) = *str;
        }
        str++;
    }
    return refactored;
}

// second main runner for prog, reads makefile
void reader(argStruct obj) {
    // find makefile
    FILE *fp;
    char *line = (char *) malloc(MAX_DEP * sizeof(char));
    if (line == NULL) {
        fprintf(stderr, "Unable to allocate memory for reader input buffer");
        exit(1);
    }
    Node *graphNodeArray[MAX_DEP];
    if(obj.output != NULL){
        fp = fopen(obj.output, "r");
        if(!fp){
            fprintf(stderr, "cant locate %s", obj.output);
            exit(1);
        }
    } else {
        fp = fopen("Makefile", "r");
        if (!fp) {
            fp = fopen("makefile", "r"); //now it has checked both m/Makefile
            if (!fp) {
                fprintf(stderr, "No m/Makefile");
                exit(1);
            }
        }
    }
    unsigned int pointerToNode = 0;
    unsigned int index = 0;
    int command_count = 0;
    int lineNum = 0;
    Node *node = NULL;
    int check;

    // iterate through makefile
    do {
        lineNum++;
        // look for null chars
        do {
            check = fgetc(fp);
            if(check == '\0'){
                fprintf(stderr, "Null character at line No %d. Exiting", lineNum);
            }
            line[index++] = (char) check;
        } while (check != '\n' && check != EOF && index < MAX_DEP);

        // line too big
        if (index >= MAX_DEP) {
            fprintf(stderr, "Line passed MaxBuffer %d. Exiting\n", MAX_DEP);
            free(line);
            exit(1);
        } else {
            line[index - 1] = '\0';
        }

        // look for commands
        char *token;
        if (line[0] == '\t') {
            // Commands
            checkCommand(line, index, lineNum);
            token = strtok(line, "\t");
            if(!token){
                fprintf(stderr, "\n%d Wrong format on line : %s\n", lineNum, line);
                exit(1);
            }
            if (node == NULL) {
                fprintf(stderr, "\n%d Wrong format on line : %s\n", lineNum, line);
                exit(1);
            }
            else {
                // assign command
                node->commands[command_count] = (char*) malloc(MAX_DEP * sizeof(char));
                strcpy(node->commands[command_count], token);
                command_count+=1;
            }

        }
        // commented line
        else if (line[0] == '\0' || line[0] == '#') {
            index = 0;
            free(line);
            line = (char *) malloc(MAX_DEP * sizeof(char));
            continue;
        }
        // if new target
        else {
            if (node != NULL) {
                node->commands[command_count] = NULL;
            }
            command_count = 0;

            // chekc if valid target
            checkTarget(line, index, lineNum);
            token = strtok(line, ":");
            if (!token) {
                fprintf(stderr, "%d: Invalid line : %s\n", lineNum, line);
                exit(EXIT_FAILURE);
            }

            // get target name
            char *targetName = malloc(sizeof(char) * MAX_DEP);
            strncpy(targetName, token, MAX_DEP);
            targetName = whiteSpace(targetName);
            token = strtok(NULL, " ");

			//new target
            node = createNode(targetName);
            graphNodeArray[pointerToNode++] = node;

            // get target dependencies
            int total_dep = 0;
            while (token != NULL) { //arrange dependencies
                if (strlen(token) > 0) {
                    node->dependencies[total_dep] = (char*) malloc(MAX_DEP * sizeof(char));
                    strcpy(node->dependencies[total_dep], token);
                    total_dep++;
                }
                token = strtok(NULL, " ");
            }
            node->dependencies[total_dep] = NULL; // null terminator
        }
        index = 0;
        free(line);
        line = (char *) malloc(MAX_DEP * sizeof(char));


    } while (check != EOF);
    if (node != NULL) {
        node->commands[command_count] = NULL; //setting node correctly
    }

    free(line);
    if(pointerToNode == 0){
        fprintf(stderr, "537make: * No targets.  Stop.\n");
        exit(EXIT_FAILURE);
    }
    buildGraph(graphNodeArray, pointerToNode);

    // execute
    if(obj.connections[0] == NULL){
        int executed = traverseAndExecute(graphNodeArray[0]);
        if (!executed) {
            printf("537make: '%s' is up to date.\n", graphNodeArray[0]->name);
        }
    }
    else {
        for (unsigned int x = 0; x < MAX_DEP; x++) {
            if (obj.connections[x] == NULL) {
                break;
            }
            int targetFound = 0;
            for (unsigned int i = 0; i < pointerToNode; i++) {
                if (strcmp(graphNodeArray[i]->name, obj.connections[x]) == 0) {
                    targetFound = 1;
                    int executed = traverseAndExecute(graphNodeArray[i]);
                    if (!executed) {
                        printf("537make: '%s' is up to date.\n", graphNodeArray[i]->name);
                    }
                    break;
                }
            }
            if (!targetFound) {
                fprintf(stderr, "Specified target %s not found", obj.connections[x]);
            }
        }
    }
    if (fclose(fp)) {
        fprintf(stderr, "Failed to close makefile or Makefile");
        exit(1);
    }
}