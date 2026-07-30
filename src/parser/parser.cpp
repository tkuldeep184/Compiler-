#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

Token Parser::peek()
{
    return tokens[pos];
}

Token Parser::peekNext()
{
    if (pos + 1 < tokens.size())
    {
        return tokens[pos + 1];
    }
    return tokens.back();
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

Token Parser::expect(TokenType type, const std::string& message)
{
    if (peek().type == type)
    {
        return advance();
    }
    throw ParseError(message + " at line " + std::to_string(peek().line));
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

        // Function call: identifier immediately followed by '('
        if (peek().type == TokenType::LEFT_PAREN)
        {
            advance(); // consume '('
            std::vector<std::unique_ptr<Expr>> arguments;
            if (peek().type != TokenType::RIGHT_PAREN)
            {
                arguments.push_back(parseExpression());
                while (match(TokenType::COMMA))
                {
                    arguments.push_back(parseExpression());
                }
            }
            expect(TokenType::RIGHT_PAREN, "expected ')' after arguments");
            return std::make_unique<CallExpr>(token.lexeme, std::move(arguments));
        }

        return std::make_unique<Identifier>(token.lexeme);
    }
    if (peek().type == TokenType::LEFT_PAREN)
    {
        advance(); // consume '('
        std::unique_ptr<Expr> expr = parseExpression();
        expect(TokenType::RIGHT_PAREN, "expected ')' after expression");
        return expr;
    }
    throw ParseError("unexpected token '" + peek().lexeme + "' at line " + std::to_string(peek().line));
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

std::unique_ptr<Expr> Parser::parseAdditive()
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

std::unique_ptr<Expr> Parser::parseComparison()
{
    std::unique_ptr<Expr> left = parseAdditive();

    while (peek().type == TokenType::LESS || peek().type == TokenType::GREATER ||
           peek().type == TokenType::LESS_EQUAL || peek().type == TokenType::GREATER_EQUAL)
    {
        Token op = advance();
        std::unique_ptr<Expr> right = parseAdditive();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseEquality()
{
    std::unique_ptr<Expr> left = parseComparison();

    while (peek().type == TokenType::EQUAL || peek().type == TokenType::NOT_EQUAL)
    {
        Token op = advance();
        std::unique_ptr<Expr> right = parseComparison();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseLogicalAnd()
{
    std::unique_ptr<Expr> left = parseEquality();

    while (peek().type == TokenType::AND)
    {
        Token op = advance();
        std::unique_ptr<Expr> right = parseEquality();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parseExpression()
{
    std::unique_ptr<Expr> left = parseLogicalAnd();

    while (peek().type == TokenType::OR)
    {
        Token op = advance();
        std::unique_ptr<Expr> right = parseLogicalAnd();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<Expr> Parser::parse()
{
    return parseExpression();
}

std::vector<std::unique_ptr<Stmt>> Parser::parseProgram()
{
    std::vector<std::unique_ptr<Stmt>> statements;
    while (peek().type != TokenType::END_OF_FILE)
    {
        statements.push_back(parseStatement());
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::parseStatement()
{
    switch (peek().type)
    {
    case TokenType::LET:
        return parseLetStatement();
    case TokenType::IF:
        return parseIfStatement();
    case TokenType::WHILE:
        return parseWhileStatement();
    case TokenType::PRINT:
        return parsePrintStatement();
    case TokenType::FUNCTION:
        return parseFunctionStatement();
    case TokenType::RETURN:
        return parseReturnStatement();
    case TokenType::LEFT_BRACE:
        return parseBlock();
    case TokenType::IDENTIFIER:
        if (peekNext().type == TokenType::ASSIGN)
        {
            return parseAssignmentStatement();
        }
        throw ParseError("expected '=' after identifier at line " + std::to_string(peek().line));
    default:
        throw ParseError("unexpected token '" + peek().lexeme + "' at line " + std::to_string(peek().line));
    }
}

std::unique_ptr<Stmt> Parser::parseLetStatement()
{
    expect(TokenType::LET, "expected 'let'");
    Token name = expect(TokenType::IDENTIFIER, "expected identifier after 'let'");
    expect(TokenType::ASSIGN, "expected '=' in let statement");
    std::unique_ptr<Expr> initializer = parseExpression();
    expect(TokenType::SEMICOLON, "expected ';' after let statement");
    return std::make_unique<LetStatement>(name.lexeme, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::parseAssignmentStatement()
{
    Token name = expect(TokenType::IDENTIFIER, "expected identifier in assignment");
    expect(TokenType::ASSIGN, "expected '=' in assignment");
    std::unique_ptr<Expr> value = parseExpression();
    expect(TokenType::SEMICOLON, "expected ';' after assignment");
    return std::make_unique<AssignmentStatement>(name.lexeme, std::move(value));
}

std::unique_ptr<Stmt> Parser::parseIfStatement()
{
    expect(TokenType::IF, "expected 'if'");
    expect(TokenType::LEFT_PAREN, "expected '(' after 'if'");
    std::unique_ptr<Expr> condition = parseExpression();
    expect(TokenType::RIGHT_PAREN, "expected ')' after if condition");
    std::unique_ptr<BlockStatement> thenBranch = parseBlock();

    std::unique_ptr<BlockStatement> elseBranch = nullptr;
    if (match(TokenType::ELSE))
    {
        elseBranch = parseBlock();
    }

    return std::make_unique<IfStatement>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::parseWhileStatement()
{
    expect(TokenType::WHILE, "expected 'while'");
    expect(TokenType::LEFT_PAREN, "expected '(' after 'while'");
    std::unique_ptr<Expr> condition = parseExpression();
    expect(TokenType::RIGHT_PAREN, "expected ')' after while condition");
    std::unique_ptr<BlockStatement> body = parseBlock();
    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<BlockStatement> Parser::parseBlock()
{
    expect(TokenType::LEFT_BRACE, "expected '{'");
    std::vector<std::unique_ptr<Stmt>> statements;
    while (peek().type != TokenType::RIGHT_BRACE && peek().type != TokenType::END_OF_FILE)
    {
        statements.push_back(parseStatement());
    }
    expect(TokenType::RIGHT_BRACE, "expected '}' after block");
    return std::make_unique<BlockStatement>(std::move(statements));
}

std::unique_ptr<Stmt> Parser::parsePrintStatement()
{
    expect(TokenType::PRINT, "expected 'print'");
    std::unique_ptr<Expr> expression = parseExpression();
    expect(TokenType::SEMICOLON, "expected ';' after print statement");
    return std::make_unique<PrintStatement>(std::move(expression));
}

std::unique_ptr<Stmt> Parser::parseFunctionStatement()
{
    expect(TokenType::FUNCTION, "expected 'function'");
    Token name = expect(TokenType::IDENTIFIER, "expected function name");
    expect(TokenType::LEFT_PAREN, "expected '(' after function name");

    std::vector<std::string> params;
    if (peek().type != TokenType::RIGHT_PAREN)
    {
        Token first = expect(TokenType::IDENTIFIER, "expected parameter name");
        params.push_back(first.lexeme);
        while (match(TokenType::COMMA))
        {
            Token p = expect(TokenType::IDENTIFIER, "expected parameter name");
            params.push_back(p.lexeme);
        }
    }
    expect(TokenType::RIGHT_PAREN, "expected ')' after parameters");

    std::unique_ptr<BlockStatement> body = parseBlock();
    return std::make_unique<FunctionStatement>(name.lexeme, std::move(params), std::move(body));
}

std::unique_ptr<Stmt> Parser::parseReturnStatement()
{
    expect(TokenType::RETURN, "expected 'return'");
    std::unique_ptr<Expr> value = parseExpression();
    expect(TokenType::SEMICOLON, "expected ';' after return statement");
    return std::make_unique<ReturnStatement>(std::move(value));
}
