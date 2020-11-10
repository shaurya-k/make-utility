////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: 537make.c
//
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/stat.h>
#include "537make.h"
#include "parseArgs.h"
#include "reader.h"

// main runner function
int main(int argc, char *argv[]) {
    argStruct input = getNextArg(argc, argv);
    reader(input);
}

// checks if file is old or not
int do_execute(Node* node) {
    if(node->dependencies != NULL) {
        // check if file exists
        FILE *file = fopen(node->name, "r");
        // if not exists
        if (!file) {
            return 1;
        }
        else {
            // get timestamp of file
            struct stat file_attr;
            fstat(fileno(file), &file_attr);
            struct timespec target_time = file_attr.st_mtim;
            fclose(file);

            // for each dependency
            int cur_dep = 0;
            while(node->dependencies[cur_dep] != NULL) {
                // check if file exists
                FILE *dep_file = fopen(node->dependencies[cur_dep], "r");
                // if not exists then throw error
                if (!dep_file) {
                    fprintf(stderr, "error: dependency file missing\n");
                    exit(1);
                }
                else {
                    // get timestamp of file
                    struct stat dep_file_attr;
                    fstat(fileno(dep_file), &dep_file_attr);
                    struct timespec dep_time = dep_file_attr.st_mtim;
                    fclose(dep_file);

                    // check if newer version avail
                    if (target_time.tv_sec == dep_time.tv_sec) {
                        if (target_time.tv_nsec < dep_time.tv_nsec) {
                            return 1;
                        }
                    }
                    else {
                        if (target_time.tv_sec < dep_time.tv_sec) {
                            return 1;
                        }
                    }
                }
                cur_dep++;
            }
            // if file is up to date and exists
            return 0;
        }
    }
    // if no dependencies
    return 1;
}

// forks and executes commands
int execute(Node* node) {
    // need to execute?
    if(do_execute(node)) {
        char** commands = node->commands;
        int cur_command = 0;
        while(commands[cur_command] != NULL) {
            // fork process
            int f = fork();
            if (f < 0){
                fprintf(stderr, "error: fork failed\n");
                exit(1);
            }

            // parent proc
            else if (f > 0) {
                printf("%s\n", commands[cur_command]);
                // wait until child is done
                int status;
                wait(&status);
                if (!WIFEXITED(status)) {
                    fprintf(stderr, "error: fork child failed on exit\n");
                    exit(EXIT_FAILURE);
                }
                cur_command++;
            }

            // child proc
            else if (f == 0) {
                // parsed command init
                char** args = (char**)malloc(sizeof(char*) * (strlen(commands[cur_command]) + 1));

                // copy command string
                char* str = (char*) malloc(sizeof(commands[cur_command]));
                strcpy(str, commands[cur_command]);

                char * pch;
                int index = 0;
                pch = strtok (str," \t");
                while (pch != NULL) {
                    args[index] = pch;
                    pch = strtok (NULL, " \t");
                    index++;
                }
                args[index] = NULL;

                // execute prog
                if (args[0] == NULL) {
                    fprintf(stderr, "Null passed as command\n");
                    exit(1);
                }

                execvp(args[0], args);

                // will only get here if execvp fails
                exit(1);
            }
        }
        return 1;
    }
    return 0;
}
