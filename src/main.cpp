#include "lexer/token.h"
#include "lexer/lexer.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "parser/print_visitor.h"
#include <memory>
#include <iostream>

int main()
{
    Lexer lexer(("10 + 2 * 3"));
    std::vector<Token> tokens = lexer.tokenize();

    for (Token t : tokens)
    {
        std::cout << t.toString() << std::endl;
    }

    Parser parser(tokens);
    std::unique_ptr<Expr> root = parser.parse();

    PrintVisitor printer;
    root->accept(printer);
    std::cout << std::endl;

    return 0;
}