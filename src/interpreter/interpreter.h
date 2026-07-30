#pragma once
#include "../parser/ast.h"
#include "../parser/stmt.h"
#include "environment.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

// Control-flow signal for `return` — not an error, just unwinds the call stack
// back to the CallExpr evaluator. Deliberately not derived from std::exception.
struct ReturnSignal {
    int value;
};

class Interpreter : public Visitor, public StmtVisitor
{
public:
    void interpret(std::vector<std::unique_ptr<Stmt>>& program);
    Environment& getEnvironment() { return globals; }

    // Expression visitors
    void visit(BinaryExpr& expr) override;
    void visit(IntegerLiteral& expr) override;
    void visit(Identifier& expr) override;
    void visit(CallExpr& expr) override;

    // Statement visitors
    void visit(LetStatement& stmt) override;
    void visit(AssignmentStatement& stmt) override;
    void visit(IfStatement& stmt) override;
    void visit(WhileStatement& stmt) override;
    void visit(BlockStatement& stmt) override;
    void visit(PrintStatement& stmt) override;
    void visit(FunctionStatement& stmt) override;
    void visit(ReturnStatement& stmt) override;

private:
    Environment globals;
    Environment* current = &globals;
    std::unordered_map<std::string, FunctionStatement*> functions;
    int result = 0;

    int evaluate(Expr* e);
    void execute(Stmt* s);
};
