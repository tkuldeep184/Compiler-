#include "lexer/token.h"
#include "lexer/lexer.h"
#include <iostream>

int main()
{
    Lexer lexer(R"(let x = 10 + 2;
                if (x == 12) {
                let y = x * 2;
                } )");
    std::vector<Token> tokens = lexer.tokenize();

    for (Token t : tokens)
    {
        std::cout << t.toString() << std::endl;
    }

    return 0;
}