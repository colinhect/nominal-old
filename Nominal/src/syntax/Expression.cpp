// Copyright Colin Hill 2011

#include "../Pass.h"

#include "Expression.h"

Expression::Expression(const SourceLocation& sourceLocation) :
        SyntaxNode(sourceLocation) {
}

BinaryExpression::BinaryExpression(const SourceLocation& sourceLocation, const std::string op, Expression* leftExpression, Expression* rightExpression) :
        Expression(sourceLocation),
        op(op),
        leftExpression(leftExpression),
        rightExpression(rightExpression) { 
}

void BinaryExpression::accept(Pass* pass) {
    pass->visitBinaryExpression(this);
}

MethodExpression::MethodExpression(const SourceLocation& sourceLocation, const std::string& methodName, Expression* objectExpression, const ExpressionArray& arguments) :
        Expression(sourceLocation),
        methodName(methodName),
        objectExpression(objectExpression),
        arguments(arguments) {
}

void MethodExpression::accept(Pass* pass) {
    pass->visitMethodExpression(this);
}

FunctionExpression::FunctionExpression(const SourceLocation& sourceLocation, const std::string& functionName, const ExpressionArray& arguments) :
        Expression(sourceLocation),
        functionName(functionName),
        arguments(arguments) {
}

void FunctionExpression::accept(Pass* pass) {
    pass->visitFunctionExpression(this);
}

ObjectExpression::ObjectExpression(const SourceLocation& sourceLocation, const std::string& objectName) :
        Expression(sourceLocation),
        objectName(objectName) {
}

void ObjectExpression::accept(Pass* pass) {
    pass->visitObjectExpression(this);
}

IntegerExpression::IntegerExpression(const SourceLocation& sourceLocation, int value) :
        Expression(sourceLocation),
        value(value) {
}

void IntegerExpression::accept(Pass* pass) {
    pass->visitIntegerExpression(this);
}