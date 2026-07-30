#pragma once
#include "ast.h"
#include <iostream>

struct PrintVisitor : public Visitor {
    void visit(BinaryExpr& expr) override {
        std::cout << "BinaryExpr(";
        expr.left->accept(*this);
        std::cout << " " << expr.op.lexeme << " ";
        expr.right->accept(*this);
        std::cout << ")";
    }
    void visit(IntegerLiteral& expr) override {
        std::cout << "IntegerLiteral(" << expr.value << ")";
    }
    void visit(Identifier& expr) override {
        std::cout << "Identifier(" << expr.name << ")";
    }
    void visit(CallExpr& expr) override {
        std::cout << "CallExpr(" << expr.callee << ", args=" << expr.arguments.size() << ")";
    }
};
