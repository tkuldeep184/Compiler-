#pragma once
#include "../parser/ast.h"
#include "../parser/stmt.h"
#include "environment.h"
#include <memory>
#include <vector>

class Interpreter : public Visitor, public StmtVisitor
{
public:
    void interpret(std::vector<std::unique_ptr<Stmt>>& program);
    Environment& getEnvironment() { return env; }

    // Expression visitors
    void visit(BinaryExpr& expr) override;
    void visit(IntegerLiteral& expr) override;
    void visit(Identifier& expr) override;

    // Statement visitors
    void visit(LetStatement& stmt) override;
    void visit(AssignmentStatement& stmt) override;
    void visit(IfStatement& stmt) override;
    void visit(WhileStatement& stmt) override;
    void visit(BlockStatement& stmt) override;

private:
    Environment env;
    int result = 0;

    int evaluate(Expr* e);
    void execute(Stmt* s);
};
