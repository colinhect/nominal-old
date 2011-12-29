// Copyright Colin Hill 2011

#pragma once

#include "../SourceLocation.h"
#include "../syntax/Expression.h"
#include "../syntax/SyntaxNode.h"

#include <memory>
#include <vector>

class Block;

class Statement : public SyntaxNode {
public:
    Statement(const SourceLocation& sourceLocation);
    virtual ~Statement() { }
};

typedef std::vector<std::shared_ptr<Statement>> StatementArray;

class LetStatement : public Statement {
public:
    LetStatement(const SourceLocation& sourceLocation, const std::string& objectName, Expression* expression);
    virtual ~LetStatement() { }

    void accept(Pass* pass);

public:
    std::string objectName;
    std::unique_ptr<Expression> expression;
};

class ReturnStatement : public Statement {
public:
    ReturnStatement(const SourceLocation& sourceLocation, Expression* expression);
    virtual ~ReturnStatement() { }

    void accept(Pass* pass);

public:
    std::unique_ptr<Expression> expression;
};

class IfStatement : public Statement {
public:
    IfStatement(const SourceLocation& sourceLocation, Expression* conditionExpression, Block* thenBlock, Block* elseBlock);
    virtual ~IfStatement() { }

    void accept(Pass* pass);

public:
    std::unique_ptr<Expression> conditionExpression;
    std::unique_ptr<Block> thenBlock;
    std::unique_ptr<Block> elseBlock;
};

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(const SourceLocation& sourceLocation, Expression* expression);
    virtual ~ExpressionStatement() { }

    void accept(Pass* pass);

public:
    std::unique_ptr<Expression> expression;
};