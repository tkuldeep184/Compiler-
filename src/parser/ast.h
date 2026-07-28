#pragma once
#include <memory>
#include "../lexer/token.h"
#include <iostream>

struct BinaryExpr;
struct IntegerLiteral;
struct Identifier;

struct Visitor {
    virtual void visit(BinaryExpr& expr) = 0;
    virtual void visit(IntegerLiteral& expr) = 0;
    virtual void visit(Identifier& expr) = 0;
    virtual ~Visitor() = default;
};

struct Expr {
    virtual void accept(Visitor& visitor) = 0;
    virtual ~Expr() = default;
};

struct BinaryExpr : public Expr {
    Token op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;

    BinaryExpr(Token op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) : op(op), left(std::move(left)), right(std::move(right)) {}
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

struct IntegerLiteral : public Expr {
    int value;
    IntegerLiteral(int value) : value(value) {}
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

struct Identifier : public Expr {
    std::string name;
    Identifier(const std::string name) : name(name) {}
    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};
