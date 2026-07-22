#include "lexer.h"

Lexer::Lexer(std::string source)
{
    // initialize your fields here
    this->source = source;
    this->start = 0;
    this->current = 0;
    this->line = 1;
    this->column = 1;
}

std::vector<Token> Lexer::tokenize()
{
    while (current < source.size())
    {
        start = current;
        scanToken();
    }

    // add EOF token at the end
    tokens.push_back({TokenType::END_OF_FILE, "", line, column});
    return tokens;
}

char Lexer::advance()
{
    if (current < source.size())
    {
        char c = source[current];
        current++;
        column++;
        return c;
    }
    return '\0'; // Return null character if at the end of the source
}

char Lexer::peek()
{
    if (current < source.size())
    {
        return source[current];
    }
    return '\0'; // Return null character if at the end of the source
}

void Lexer::scanToken()
{
    char c = advance();
    switch (c)
    {
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace
        break;
    case '\n':
        line++;
        column = 1;
        break;
    case '+':
        tokens.push_back({TokenType::PLUS, "+", line, column});
        break;
    case '-':
        tokens.push_back({TokenType::MINUS, "-", line, column});
        break;
    case '*':
        tokens.push_back({TokenType::STAR, "*", line, column});
        break;
    case '/':
        tokens.push_back({TokenType::SLASH, "/", line, column});
        break;
    case '=':
        if (peek() == '=')
        {
            advance();
            tokens.push_back({TokenType::EQUAL, "==", line, column});
        }
        else
        {
            tokens.push_back({TokenType::ASSIGN, "=", line, column});
        }
        break;
    case '!':
        if (peek() == '=')
        {
            advance();
            tokens.push_back({TokenType::NOT_EQUAL, "!=", line, column});
        }
        else
        {
            tokens.push_back({TokenType::UNKNOWN, "!", line, column});
        }
        break;
    case '<':
        tokens.push_back({TokenType::LESS, "<", line, column});
        break;
    case '>':
        tokens.push_back({TokenType::GREATER, ">", line, column});
        break;
    case '(':
        tokens.push_back({TokenType::LEFT_PAREN, "(", line, column});
        break;
    case ')':
        tokens.push_back({TokenType::RIGHT_PAREN, ")", line, column});
        break;
    case '{':
        tokens.push_back({TokenType::LEFT_BRACE, "{", line, column});
        break;
    case '}':
        tokens.push_back({TokenType::RIGHT_BRACE, "}", line, column});
        break;
    case ';':
        tokens.push_back({TokenType::SEMICOLON, ";", line, column});
        break;
    case ',':
        tokens.push_back({TokenType::COMMA, ",", line, column});
        break;
    default:
        if (isdigit(c))
        {
            // keep reading while next char is also a digit
            while (isdigit(peek()))
            {
                advance();
            }
            // now build the token from source[start] to source[current]
            std::string num = source.substr(start, current - start);
            tokens.push_back({TokenType::INTEGER, num, line, column});
        }
        else if (isalpha(c))
        {
            while (isalpha(peek()) || isdigit(peek()))
            {
                advance();
            }
            std::string word = source.substr(start, current - start);

            // check if it's a keyword
            TokenType type;
            if (word == "let")
                type = TokenType::LET;
            else if (word == "const")
                type = TokenType::CONST;
            else if (word == "if")
                type = TokenType::IF;
            else if (word == "else")
                type = TokenType::ELSE;
            else if (word == "while")
                type = TokenType::WHILE;
            else if (word == "return")
                type = TokenType::RETURN;
            else
                type = TokenType::IDENTIFIER;

            tokens.push_back({type, word, line, column});
        }
        else
        {
            tokens.push_back({TokenType::UNKNOWN, std::string(1, c), line, column});
        }
        break;
    }
}