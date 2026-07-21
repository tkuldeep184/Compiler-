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