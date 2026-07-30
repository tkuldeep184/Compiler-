#include "lexer/token.h"
#include "lexer/lexer.h"
#include "parser/ast.h"
#include "parser/stmt.h"
#include "parser/parser.h"
#include "interpreter/interpreter.h"
#include <memory>
#include <iostream>
#include <vector>

int main()
{
    std::string source =
        "// Week 4 core feature check\n"
        "function fib(n) {\n"
        "    if (n <= 1) {\n"
        "        return n;\n"
        "    }\n"
        "    return fib(n - 1) + fib(n - 2);\n"
        "}\n"
        "\n"
        "let i = 0;\n"
        "while (i < 8) {\n"
        "    print fib(i);\n"
        "    i = i + 1;\n"
        "}\n"
        "\n"
        "let a = 5;\n"
        "if (a > 0 && a < 10) {\n"
        "    print a;\n"
        "}\n";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> program;

    try
    {
        program = parser.parseProgram();
    }
    catch (const ParseError& error)
    {
        std::cout << "Parse error: " << error.what() << std::endl;
        return 1;
    }

    Interpreter interpreter;

    try
    {
        interpreter.interpret(program);
    }
    catch (const std::runtime_error& error)
    {
        std::cout << "Runtime error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}
