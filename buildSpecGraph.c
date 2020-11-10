////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecGraph.c
//
////////////////////////////////////////////////////////////////////////////////
#include "buildSpecGraph.h"
#include "537make.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// creates new node with mem allocated
Node* createNode(char* name) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (!node) {
        fprintf(stderr, "memory allocation error for node\n");
        exit(1);
    }

    // initialize variables //
    node->name = name;

    // allocate memory //
    node->dependencies = (char **) malloc(MAX_DEP * sizeof(char **));
    if (!node->dependencies) {
        fprintf(stderr, "memory allocation error for node dependencies\n");
        exit(1);
    }

    node->commands = (char **) malloc(MAX_DEP * sizeof(char *));
    if (!node->commands) {
        fprintf(stderr, "memory allocation error for node commands\n");
        exit(1);
    }

    node->children = (Node **) malloc(MAX_DEP * sizeof(Node *));
    if (!node->children) {
        fprintf(stderr, "memory allocation error for node children\n");
        exit(1);
    }

    return node;
}

// takes all nodes and connects them
Node* buildGraph(Node** nodes, unsigned int list_size) {
    for(int i=0; i < list_size; i++) {
        nodes[i]->index = i;
    }

    Node* root = NULL;

    for(int i=0; i < list_size; i++) {
        // set first node to root
        if (root == NULL) {
            root = nodes[i];
        }

        // iterate through nodes dependencies
        int cur_depend = 0;
        int childCount = 0;
        while (nodes[i]->dependencies[cur_depend] != NULL) {
            int isNode = 0;

            // is dependency another node
            for(int j=0; j < list_size; j++) {
                if(strcmp(nodes[i]->dependencies[cur_depend], nodes[j]->name) == 0) {
                    isNode = 1;
                    nodes[i]->children[childCount] = nodes[j];
                    childCount++;
                    break;
                }
            }

            // is dependency a file
            FILE *fp = fopen(nodes[i]->dependencies[cur_depend], "r");

            // must be target or file otherwise error.
            if (isNode == 0 && fp == NULL) {
                fprintf(stderr, "Error: Target '%s' has no way to be made.\n", nodes[i]->name);
                fclose(fp);
                exit(1);
            }
            // if ok then move on to next dependency
            else {
                if(fp != NULL) {
                    fclose(fp);
                }
                cur_depend++;
            }
        }
    }

    // check for cycles
    if (detectCycle(nodes, list_size) == 1) {
        fprintf(stderr, "Error: There was a cycle in the dependency graph.\n");
        exit(1);
    }

    return root;
}

// helper function for recursive cycle checking
int detectCycleHelper(Node** nodes, Node* node, unsigned int index, int* visited, int* recurse) {
    // if not visited
    if(!visited[index]) {
        // set node as visited
        visited[index] = 1;
        recurse[index] = 1;
    }

    // Recurse for all dependencies
    int i = 0;
    while (i < MAX_DEP && node->children[i] != NULL) {
        // if not visited and cycle through rest
        if (!visited[node->children[i]->index] && detectCycleHelper(nodes, nodes[index]->children[i] ,node->children[i]->index, visited, recurse)) {
            return 1;
        }
        else if (recurse[node->children[i]->index]) {
            return 1;
        }
        i++;
        if (node->children[i] != NULL) {
        }
        else {
        }
    }

    recurse[node->index] = 0;
    return 0;
}


// checks if there is a cycle in the graph
int detectCycle(Node** nodes, unsigned int list_size) {
    int visited[list_size];
    int recurse[list_size];

    // initialize lists to false for each node
    for(int i=0; i<list_size;i++ ){
        visited[i] = 0;
        recurse[i] = 0;
    }

    // call recursive function for each node to check for cycles
    for(int i=0; i < list_size; i++) {
        if (detectCycleHelper(nodes, nodes[i], nodes[i]->index, visited, recurse)) {
            printf("cycle detected in '%s' dependencies\n", nodes[i]->name);
            return 1;
        }
    }
    return 0;
}

int traverseAndExecute(Node* root) {

    // if the first target has no dependencies
    if (root->children[0] == NULL) {
        return execute(root);
    }

    // recurse into the graph
    for (unsigned int i = 0; i < MAX_DEP; ++i) {
        if (root->children[i] == NULL) {
            break;
        }
        traverseAndExecute(root->children[i]);
    }

    return execute(root);
}
