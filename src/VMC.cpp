#include "../include/Stack/Stack.hpp"
#include "../include/VM/VirtualMachine.hpp"

#include <unistd.h>
#include <fstream>
#include <filesystem>

//ERRORS
#define NO_FILE_GIVEN               -1
#define FILE_DOES_NOT_EXIST         -2
#define UNSUPPORTED_FILE_TYPE       -3
#define UNKNOWN_FLAG 				-4
#define MISSING_ARGUMENT 			-5
#define PRINT_HELP 					-6
#define MAX_MEMORY_PASSED			-7


void printHelp()
{
    std::cout << "-m\tManually allocates memory for the Virtual Machine Stack, defaults to " << STACK_MEMORY_MAX << std::endl;
    std::cout << "-o\tGive an output path if you want to store the executable anywhere else" << std::endl;
}


//This file is used for running the Virtual Machine as a program, which runs until the user quits.
int main(int argc, char **argv)
{

    int vm_mem = 0;
    std::string outputPath;
    std::string filePath;

    int op;
    opterr = 0;
    while ((op = getopt(argc, argv, "m:o:h")) != -1)
    {
        switch(op)
        {
            case 'm':
                vm_mem = std::atoi(optarg);
                if (vm_mem > STACK_MEMORY_MAX)
                {
                    std::cerr << "CANNOT ALLOCATE MORE BLOCKS OF MEMORY THAN: " << STACK_MEMORY_MAX << std::endl;
                    return MAX_MEMORY_PASSED;
                }
                break;
            case 'o':
                outputPath = optarg;
                break;
            case 'h':
                printHelp();
                return PRINT_HELP;
                break;
            case '?':
                if (optopt == 'm')
                {
                    std::cerr << "Option -" << (char)optopt << " requires a <int>." << std::endl;
                    return MISSING_ARGUMENT;
                }
                else if (optopt == 'o')
                {
                    std::cerr << "Option -" << (char)optopt << " requires a <string>" << std::endl;
                    return MISSING_ARGUMENT;
                }
                else
                {
                    std::cerr << "Unknown option '" << (char)optopt << "'"<< std::endl;
                    return UNKNOWN_FLAG;
                }
        }
    }

    if (argc == 1)
    {
        std::cerr << "NO FILE NAME GIVEN TO COMPILE\nUSAGE: VMC <file> <flags>" << std::endl;
        return NO_FILE_GIVEN; 
    }

    if (!std::filesystem::exists(argv[argc - 1]))
    {
        std::cerr << "FILE DOES NOT EXIST.\n" << "Path: " << argv[argc - 1] << std::endl;
        return FILE_DOES_NOT_EXIST;
    }

    filePath = argv[argc - 1];
    
    std::string *file = new std::string{argv[argc - 1]};

    if (file->find(".lion") == std::string::npos)
    {
        std::cerr << "FILE TYPE UNSUPPORTED.\nSupported file types are: .lion" << std::endl;
        delete file;
        return UNSUPPORTED_FILE_TYPE;
    }
    delete file;


    

    std::ifstream nFile{filePath};
    std::string line;
    while (!nFile.eof())
    {
        std::getline(nFile, line);
        std::cout << line << std::endl;
    }

    std::cout << std::endl;



}