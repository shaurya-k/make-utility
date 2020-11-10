////////////////////////////////////////////////////////////////////////////////
// Main File: 537make.c
// This File: buildSpecGraph.h
//
////////////////////////////////////////////////////////////////////////////////
#ifndef PROJ3_CS537_BUILDSPECGRAPH_H
#define PROJ3_CS537_BUILDSPECGRAPH_H

#define MAX_DEP 4096

typedef struct Node {
    unsigned int index;
    char* name;
    char** dependencies;
    char** commands;
    struct Node** children;
} Node;

Node* createNode(char* name);

Node* buildGraph(struct Node** nodes, unsigned int list_size);

int detectCycleHelper(Node** nodes, Node* node, unsigned int index, int visited[], int* recurse);

int detectCycle(Node** nodes, unsigned int list_size);

int traverseAndExecute(Node* root);

#endif //PROJ3_CS537_BUILDSPECGRAPH_H
