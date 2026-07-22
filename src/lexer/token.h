#pragma once
#include <string>

enum class TokenType
{
    // Special
    END_OF_FILE,
    UNKNOWN,
    // Literals
    IDENTIFIER,
    INTEGER,
    FLOAT,
    STRING,
    // Keywords
    LET,
    CONST,
    IF,
    ELSE,
    WHILE,
    RETURN,
    // Operators
    ASSIGN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    // Comparisons
    EQUAL,
    NOT_EQUAL,
    LESS,
    GREATER,
    // Symbols
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    SEMICOLON,
    COMMA
};

inline std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::END_OF_FILE:
        return "END_OF_FILE";
    case TokenType::UNKNOWN:
        return "UNKNOWN";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::INTEGER:
        return "INTEGER";
    case TokenType::FLOAT:
        return "FLOAT";
    case TokenType::STRING:
        return "STRING";
    case TokenType::LET:
        return "LET";
    case TokenType::CONST:
        return "CONST";
    case TokenType::IF:
        return "IF";
    case TokenType::ELSE:
        return "ELSE";
    case TokenType::WHILE:
        return "WHILE";
    case TokenType::RETURN:
        return "RETURN";
    case TokenType::ASSIGN:
        return "ASSIGN";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::STAR:
        return "STAR";
    case TokenType::SLASH:
        return "SLASH";
    case TokenType::EQUAL:
        return "EQUAL";
    case TokenType::NOT_EQUAL:
        return "NOT_EQUAL";
    case TokenType::LESS:
        return "LESS";
    case TokenType::GREATER:
        return "GREATER";
    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE:
        return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE:
        return "RIGHT_BRACE";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::COMMA:
        return "COMMA";
    

    default:
        return "UNKNOWN";
    }
}

struct Token
{
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    std::string toString() const
    {
        return "[" + tokenTypeToString(type) + ", " + lexeme + ", line " + std::to_string(line) + ", col " + std::to_string(column) + "]";
    }
};

