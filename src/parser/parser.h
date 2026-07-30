#pragma once
#include "../lexer/token.h"
#include "../parser/ast.h"
#include "../parser/stmt.h"
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error
{
public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser
{
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Expr> parse();
    std::vector<std::unique_ptr<Stmt>> parseProgram();

private:
    const std::vector<Token>& tokens;
    size_t pos = 0;

    Token peek();
    Token peekNext();
    Token advance();
    bool match(TokenType type);
    Token expect(TokenType type, const std::string& message);

    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<Stmt> parseLetStatement();
    std::unique_ptr<Stmt> parseAssignmentStatement();
    std::unique_ptr<Stmt> parseIfStatement();
    std::unique_ptr<Stmt> parseWhileStatement();
    std::unique_ptr<Stmt> parsePrintStatement();
    std::unique_ptr<Stmt> parseFunctionStatement();
    std::unique_ptr<Stmt> parseReturnStatement();
    std::unique_ptr<BlockStatement> parseBlock();

    std::unique_ptr<Expr> parsePrimary();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseAdditive();
    std::unique_ptr<Expr> parseComparison();
    std::unique_ptr<Expr> parseEquality();
    std::unique_ptr<Expr> parseLogicalAnd();
    std::unique_ptr<Expr> parseExpression();
};
