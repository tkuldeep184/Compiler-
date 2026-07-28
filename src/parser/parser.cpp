#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

Token Parser::peek()
{
    return tokens[pos];
}

Token Parser::advance()
{
    return tokens[pos++];
}

bool Parser::match(TokenType type)
{
    if (peek().type == type)
    {
        advance();
        return true;
    }
    return false;
}

std::unique_ptr<Expr> Parser::parsePrimary()
{
    if (peek().type == TokenType::INTEGER)
    {
        Token token = advance();
        return std::make_unique<IntegerLiteral>(std::stoi(token.lexeme));
    }
    if (peek().type == TokenType::IDENTIFIER)
    {
        Token token = advance();
        return std::make_unique<Identifier>(token.lexeme);
    }
    throw std::runtime_error("unexpected token");
}

std::unique_ptr<Expr> Parser::parseTerm()
{
    std::unique_ptr<Expr> left = parsePrimary();

    while (peek().type == TokenType::STAR || peek().type == TokenType::SLASH)
    {
        Token op = advance();
        std::unique_ptr<Expr> right = parsePrimary();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseExpression()
{
    std::unique_ptr<Expr> left = parseTerm();

    while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS)
    {
        Token op = advance();
        std::unique_ptr<Expr> right = parseTerm();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parse()
{
    return parseExpression();
}
