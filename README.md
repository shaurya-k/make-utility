# make utility

This program is used validate, read and exec the makefiles which would run by the "make" command.

## 537make module
This module contains the main func which essentially is the driver. It also contains the execute function with its helper to execute the commands associated with nodes that passed through and handles the building for target.

## reader modules
This module reads through the cmdline args and creates the graph structure that is needed to handle the program to help order the execution. It also contains a method to remove the whitespaces and check the commands/targets.

## parseArgs module
This module contains functions that help you parse lines in the makefile. It splits a line into an array of strings, checking whether the line begins with a tab or regular character, and filters out blank lines.

## buildSpecGraph module
This module is used to build the graph with the args that are passed through and then traverse to check for cycles using a post order traversal method. It represents an object with its dependencies.



