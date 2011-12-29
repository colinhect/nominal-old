// Copyright Colin Hill 2011

#pragma once

#include "syntax/Block.h"
#include "syntax/Module.h"
#include "syntax/Definition.h"
#include "syntax/Expression.h"
#include "syntax/Parameter.h"
#include "syntax/Statement.h"

class Pass {
public:
    virtual ~Pass() { }

    virtual void visitModule(Module* module) = 0;

    virtual void visitFunctionDefinition(FunctionDefinition* functionDefinition) = 0;

    virtual void visitParameter(Parameter* parameter) = 0;

    virtual void visitBlock(Block* block) = 0;

    virtual void visitLetStatement(LetStatement* letStatement) = 0;
    virtual void visitReturnStatement(ReturnStatement* returnStatement) = 0;
    virtual void visitIfStatement(IfStatement* ifStatement) = 0;
    virtual void visitExpressionStatement(ExpressionStatement* expressionStatement) = 0;

    virtual void visitBinaryExpression(BinaryExpression* binaryExpression) = 0;
    virtual void visitMethodExpression(MethodExpression* methodExpression) = 0;
    virtual void visitFunctionExpression(FunctionExpression* functionExpression) = 0;
    virtual void visitObjectExpression(ObjectExpression* objectExpression) = 0;
    virtual void visitIntegerExpression(IntegerExpression* integerExpression) = 0;
};