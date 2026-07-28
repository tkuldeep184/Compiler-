#pragma once
#include "../lexer/token.h"
#include "../parser/ast.h"
#include <vector>
#include <memory>

class Parser
{
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Expr> parse();

private:
    const std::vector<Token>& tokens;
    size_t pos = 0;

    Token peek();
    Token advance();
    bool match(TokenType type);
    std::unique_ptr<Expr> parsePrimary();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseExpression();
};
