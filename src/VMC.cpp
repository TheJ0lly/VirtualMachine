#include "../include/VM/VirtualMachine.hpp"
#include <fstream>

#include <filesystem>


#include <stdlib.h>
#include <unistd.h>

/*

VMC main.lion -m -o

*/


int main(int argc, char **argv)
{
    std::string OutputPath = std::filesystem::current_path();
    std::string CFilePath;

    std::vector<std::string> ops;

    VM vm;


    
}








































































// while ( (opt = getopt(argc, argv, "p:o:v")) != -1)
    // {
    //     switch(opt)
    //     {
    //         case 'p':
    //             CFilePath = std::filesystem::current_path();
    //             CFilePath += "/";
    //             CFilePath += optarg;
    //             if (!std::filesystem::exists(CFilePath))
    //             {
    //                 std::cerr << "COMPILER ERROR: NO FILE FOUND => \"" << CFilePath << "\"" << std::endl;
    //                 return VM_FILE_NOT_FOUND;
    //             }

    //             if (std::filesystem::is_directory(CFilePath))
    //             {
    //                 std::cerr << "COMPILER ERROR: PASSING DIRECTORY PATH => \"" << CFilePath << "\"" << std::endl;
    //                 return VM_DIRECTORY_INSTEAD_FILE;
    //             }

    //             if (CFilePath.find(".lion") == std::string::npos)
    //             {
    //                 std::cerr << "COMPILER ERROR: FILE PASSED NOT OF EXTENSION \".lion\" => \"" << CFilePath << "\"" << std::endl;
    //                 return VM_NOT_SUPPORTED_FILE;
    //             }

    //             break;
    //         case 'o':
    //             OutputPath += optarg;
    //             break;
    //         case 'v':
    //             vm.getVMInfo();
    //             break;
    //     }
    // }

    // if (CFilePath == "")
    // {
    //     std::cerr << "COMPILER ERROR: NO FILE WAS GIVEN TO COMPILE" << std::endl;
    //     return VM_NO_FILE_GIVEN;
    // }

    // std::ifstream file{CFilePath};
    // while (!file.eof())
    // {
    //     std::string line;
    //     std::getline(file, line);

    //     ops.push_back(line);
    // }