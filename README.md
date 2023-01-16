# VirtualMachine
A simple stack based Virtual Machine written in C++.
The stack holds at a maximum 1000000 blocks of 8 bytes.
Each block contains a char which holds the type of the data inside the block, and a union in which we can find a char, an int, and a float, which represent the current value that is held inside that block. There is another type of block, and that is a Terminator block, which can be found at the end of only and only strings.

As of now, there are functions from the VirtualMachine class, which allocate and deallocate memory and variables, but in the future, there will be a programming language which will operate on the Virtual Machine space and only there, it will not have access to the memory outside the Virtual Machine memory.



# LION
Is the name of the soon to be programming language that will operate on this Virtual Machine. It will be a procedural programming language, which will have 3 data types: string, int, float. There will be arrays for these data types.

LION will contain the following:
    -Data Types: string, int, float
    -Array support
    -Functions
    -Pointers(maybe)
    -Complex data types(something alike "struct" in C/C++)