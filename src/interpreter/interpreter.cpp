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
    result = current->get(expr.name);
}

void Interpreter::visit(BinaryExpr& expr)
{
    // Short-circuit logical operators: evaluate the right operand only when
    // the left operand does not already decide the outcome.
    if (expr.op.type == TokenType::AND)
    {
        int left = evaluate(expr.left.get());
        if (left == 0)
        {
            result = 0;
            return;
        }
        result = (evaluate(expr.right.get()) != 0) ? 1 : 0;
        return;
    }
    if (expr.op.type == TokenType::OR)
    {
        int left = evaluate(expr.left.get());
        if (left != 0)
        {
            result = 1;
            return;
        }
        result = (evaluate(expr.right.get()) != 0) ? 1 : 0;
        return;
    }

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
    case TokenType::LESS_EQUAL:
        result = (left <= right) ? 1 : 0;
        break;
    case TokenType::GREATER_EQUAL:
        result = (left >= right) ? 1 : 0;
        break;
    default:
        throw std::runtime_error("Unknown binary operator '" + expr.op.lexeme + "'");
    }
}

void Interpreter::visit(CallExpr& expr)
{
    auto it = functions.find(expr.callee);
    if (it == functions.end())
    {
        throw std::runtime_error("Undefined function '" + expr.callee + "'");
    }
    FunctionStatement* fn = it->second;

    if (expr.arguments.size() != fn->params.size())
    {
        throw std::runtime_error("Function '" + expr.callee + "' expects " +
                                 std::to_string(fn->params.size()) + " argument(s) but got " +
                                 std::to_string(expr.arguments.size()));
    }

    // Evaluate arguments in the caller's environment first.
    std::vector<int> argValues;
    argValues.reserve(expr.arguments.size());
    for (auto& arg : expr.arguments)
    {
        argValues.push_back(evaluate(arg.get()));
    }

    // New call-local environment whose parent is always the global scope
    // (no closures — nested function declarations are unsupported).
    Environment callEnv(&globals);
    for (size_t i = 0; i < fn->params.size(); ++i)
    {
        callEnv.define(fn->params[i], argValues[i]);
    }

    Environment* previous = current;
    current = &callEnv;

    int returnValue = 0;
    try
    {
        for (auto& s : fn->body->statements)
        {
            execute(s.get());
        }
    }
    catch (ReturnSignal& r)
    {
        returnValue = r.value;
    }

    current = previous;
    result = returnValue;
}

// ---------- Statement visitors ----------

void Interpreter::visit(LetStatement& stmt)
{
    int value = evaluate(stmt.initializer.get());
    current->define(stmt.name, value);
}

void Interpreter::visit(AssignmentStatement& stmt)
{
    int value = evaluate(stmt.value.get());
    current->assign(stmt.name, value);
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

void Interpreter::visit(PrintStatement& stmt)
{
    int value = evaluate(stmt.expression.get());
    std::cout << value << "\n";
}

void Interpreter::visit(FunctionStatement& stmt)
{
    // Declarations are collected during the hoisting pass; executing one is a
    // no-op so that a FunctionStatement appearing in the statement stream does
    // not re-run its body.
    (void)stmt;
}

void Interpreter::visit(ReturnStatement& stmt)
{
    int value = evaluate(stmt.value.get());
    throw ReturnSignal{value};
}

// ---------- Entry point ----------

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>>& program)
{
    // Hoisting pass: collect every top-level function declaration so functions
    // can call each other regardless of order and recursion works.
    for (auto& stmt : program)
    {
        if (auto* fn = dynamic_cast<FunctionStatement*>(stmt.get()))
        {
            functions[fn->name] = fn;
        }
    }

    for (auto& stmt : program)
    {
        execute(stmt.get());
    }
}
