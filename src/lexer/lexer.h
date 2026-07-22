#pragma once
#include "token.h"
#include <string>
#include <vector>

class Lexer
{
public:
    Lexer(std::string source);
    std::vector<Token> tokenize();

private:
    std::string source;
    std::vector<Token> tokens;
    size_t start;
    size_t current;
    int line;
    int column;
    char advance();
    char peek();
    void scanToken();
};