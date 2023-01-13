# VirtualMachine
A simple stack based Virtual Machine written in C++.
The stack holds at a maximum 65535(2^16) blocks of 4 bytes.
Each block contains a int16_t which holds the value of the block, a char which holds the type of the block(storage or program), and a char which holds the data type of the storage block or the return type of the program that starts from the block.
