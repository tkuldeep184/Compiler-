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
        "let x = 10 + 2;\n"
        "if (x == 12) { let y = x * 2; } else { let y = 0; }\n"
        "while (y > 0) { y = y - 1; }\n";

    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    std::vector<std::unique_ptr<Stmt>> program = parser.parseProgram();

    Interpreter interpreter;
    interpreter.interpret(program);

    // Environment dump to verify results (no print statement this week)
    Environment& env = interpreter.getEnvironment();
    std::cout << "x = " << env.get("x") << std::endl;
    std::cout << "y = " << env.get("y") << std::endl;

    return 0;
}
