#include "../include/Lexer/Lexer.hpp"

Lexer::Lexer(const std::string &filePath)
{
    std::ifstream file{filePath};

    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);
        fileStrings.push_back(line);
    }
}

Lexer::~Lexer()
{

}

fileContent Lexer::returnContent()
{
    return this->fileStrings;
}