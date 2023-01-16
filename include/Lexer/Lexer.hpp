#ifndef _LEXER_HPP
#define _LEXER_HPP

#include <string>
#include <vector>
#include <fstream>

typedef std::vector<std::string> fileContent;

class Lexer
{
    private:
        fileContent fileStrings;
    public:
        Lexer(const std::string &filePath);
        ~Lexer();

        fileContent returnContent();
};

#endif