// Copyright Colin Hill 2011

#pragma once

#include "../SourceLocation.h"
#include "../syntax/SyntaxNode.h"

#include <memory>
#include <vector>

class Expression : public SyntaxNode {
public:
    Expression(const SourceLocation& sourceLocation);
    virtual ~Expression() { }
};

typedef std::vector<std::shared_ptr<Expression>> ExpressionArray;

class BinaryExpression : public Expression {
public:
    BinaryExpression(const SourceLocation& sourceLocation, const std::string op, Expression* leftExpression, Expression* rightExpression);
    virtual ~BinaryExpression() { }

    void accept(Pass* pass);

public:
    std::string op;
    std::unique_ptr<Expression> leftExpression;
    std::unique_ptr<Expression> rightExpression;
};

class MethodExpression : public Expression {
public:
    MethodExpression(const SourceLocation& sourceLocation, const std::string& methodName, Expression* objectExpression, const ExpressionArray& arguments);
    virtual ~MethodExpression() { }

    void accept(Pass* pass);

public:
    std::string methodName;
    std::unique_ptr<Expression> objectExpression;
    ExpressionArray arguments;
};

class FunctionExpression : public Expression {
public:
    FunctionExpression(const SourceLocation& sourceLocation, const std::string& functionName, const ExpressionArray& arguments);
    virtual ~FunctionExpression() { }

    void accept(Pass* pass);

public:
    std::string functionName;
    ExpressionArray arguments;
};

class ObjectExpression : public Expression {
public:
    ObjectExpression(const SourceLocation& sourceLocation, const std::string& objectName);
    virtual ~ObjectExpression() { }

    void accept(Pass* pass);

public:
    std::string objectName;
};

class IntegerExpression : public Expression {
public:
    IntegerExpression(const SourceLocation& sourceLocation, int value);
    virtual ~IntegerExpression() { }

    void accept(Pass* pass);

public:
    int value;
};