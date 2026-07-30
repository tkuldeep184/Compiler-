#include "interpreter.h"
#include <iostream>
#include <stdexcept>

int Interpreter::evaluate(Expr* e)
{
    e->accept(*this);
    return result;
}

void Interpreter::execute(Stmt* s)
{
    s->accept(*this);
}

// ---------- Expression visitors ----------

void Interpreter::visit(IntegerLiteral& expr)
{
    result = expr.value;
}

void Interpreter::visit(Identifier& expr)
{
    result = env.get(expr.name);
}

void Interpreter::visit(BinaryExpr& expr)
{
    int left = evaluate(expr.left.get());
    int right = evaluate(expr.right.get());

    switch (expr.op.type)
    {
    case TokenType::PLUS:
        result = left + right;
        break;
    case TokenType::MINUS:
        result = left - right;
        break;
    case TokenType::STAR:
        result = left * right;
        break;
    case TokenType::SLASH:
        if (right == 0)
        {
            throw std::runtime_error("Division by zero");
        }
        result = left / right;
        break;
    case TokenType::EQUAL:
        result = (left == right) ? 1 : 0;
        break;
    case TokenType::NOT_EQUAL:
        result = (left != right) ? 1 : 0;
        break;
    case TokenType::LESS:
        result = (left < right) ? 1 : 0;
        break;
    case TokenType::GREATER:
        result = (left > right) ? 1 : 0;
        break;
    default:
        throw std::runtime_error("Unknown binary operator '" + expr.op.lexeme + "'");
    }
}

// ---------- Statement visitors ----------

void Interpreter::visit(LetStatement& stmt)
{
    int value = evaluate(stmt.initializer.get());
    env.define(stmt.name, value);
}

void Interpreter::visit(AssignmentStatement& stmt)
{
    int value = evaluate(stmt.value.get());
    env.assign(stmt.name, value);
}

void Interpreter::visit(BlockStatement& stmt)
{
    for (auto& s : stmt.statements)
    {
        execute(s.get());
    }
}

void Interpreter::visit(IfStatement& stmt)
{
    if (evaluate(stmt.condition.get()) != 0)
    {
        execute(stmt.thenBranch.get());
    }
    else if (stmt.elseBranch)
    {
        execute(stmt.elseBranch.get());
    }
}

void Interpreter::visit(WhileStatement& stmt)
{
    while (evaluate(stmt.condition.get()) != 0)
    {
        execute(stmt.body.get());
    }
}

// ---------- Entry point ----------

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& program)
{
    try
    {
        for (auto& stmt : program)
        {
            execute(stmt.get());
        }
    }
    catch (const std::runtime_error& error)
    {
        std::cout << "Runtime error: " << error.what() << std::endl;
    }
}
