# VirtualMachine
A simple stack based Virtual Machine written in C++.
The stack holds at a maximum 1000000 blocks of 8 bytes.
Each block contains a char which holds the type of the data inside the block, and a union in which we can find a char, an int, and a float, which represent the current value that is held inside that block. There is another type of block, and that is a Terminator block, which can be found at the end of arrays.
