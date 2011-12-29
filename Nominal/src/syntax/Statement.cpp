// Copyright Colin Hill 2011

#include "../Pass.h"
#include "../syntax/Block.h"

#include "Statement.h"

Statement::Statement(const SourceLocation& sourceLocation) :
        SyntaxNode(sourceLocation) {
}

LetStatement::LetStatement(const SourceLocation& sourceLocation, const std::string& objectName, Expression* expression) :
        Statement(sourceLocation),
        objectName(objectName),
        expression(expression) {
}

void LetStatement::accept(Pass* pass) {
    pass->visitLetStatement(this);
}

ReturnStatement::ReturnStatement(const SourceLocation& sourceLocation, Expression* expression) :
        Statement(sourceLocation),
        expression(expression) {
}

void ReturnStatement::accept(Pass* pass) {
    pass->visitReturnStatement(this);
}

IfStatement::IfStatement(const SourceLocation& sourceLocation, Expression* conditionExpression, Block* thenBlock, Block* elseBlock) :
        Statement(sourceLocation),
        conditionExpression(conditionExpression),
        thenBlock(thenBlock),
        elseBlock(elseBlock) {
}

void IfStatement::accept(Pass* pass) {
    pass->visitIfStatement(this);
}

ExpressionStatement::ExpressionStatement(const SourceLocation& sourceLocation, Expression* expression) :
        Statement(sourceLocation),
        expression(expression) {
}

void ExpressionStatement::accept(Pass* pass) {
    pass->visitExpressionStatement(this);
}