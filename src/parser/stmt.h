#pragma once
#include <memory>
#include <string>
#include <vector>
#include "ast.h"

struct LetStatement;
struct AssignmentStatement;
struct IfStatement;
struct WhileStatement;
struct BlockStatement;

struct StmtVisitor {
    virtual void visit(LetStatement& stmt) = 0;
    virtual void visit(AssignmentStatement& stmt) = 0;
    virtual void visit(IfStatement& stmt) = 0;
    virtual void visit(WhileStatement& stmt) = 0;
    virtual void visit(BlockStatement& stmt) = 0;
    virtual ~StmtVisitor() = default;
};

struct Stmt {
    virtual void accept(StmtVisitor& visitor) = 0;
    virtual ~Stmt() = default;
};

struct LetStatement : public Stmt {
    std::string name;
    std::unique_ptr<Expr> initializer;

    LetStatement(const std::string name, std::unique_ptr<Expr> initializer)
        : name(name), initializer(std::move(initializer)) {}
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct AssignmentStatement : public Stmt {
    std::string name;
    std::unique_ptr<Expr> value;

    AssignmentStatement(const std::string name, std::unique_ptr<Expr> value)
        : name(name), value(std::move(value)) {}
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct BlockStatement : public Stmt {
    std::vector<std::unique_ptr<Stmt>> statements;

    BlockStatement(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct IfStatement : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<BlockStatement> thenBranch;
    std::unique_ptr<BlockStatement> elseBranch;

    IfStatement(std::unique_ptr<Expr> condition,
                std::unique_ptr<BlockStatement> thenBranch,
                std::unique_ptr<BlockStatement> elseBranch)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elseBranch(std::move(elseBranch)) {}
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};

struct WhileStatement : public Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<BlockStatement> body;

    WhileStatement(std::unique_ptr<Expr> condition,
                   std::unique_ptr<BlockStatement> body)
        : condition(std::move(condition)), body(std::move(body)) {}
    void accept(StmtVisitor& visitor) override {
        visitor.visit(*this);
    }
};
