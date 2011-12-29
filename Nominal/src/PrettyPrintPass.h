// Copyright Colin Hill 2011

#include "Pass.h"

class PrettyPrintPass : public Pass {
public:
    PrettyPrintPass(std::ostream& outputStream);

    void visitModule(Module* module);

    void visitFunctionDefinition(FunctionDefinition* functionDefinition);

    void visitParameter(Parameter* parameter);

    void visitBlock(Block* block);

    void visitLetStatement(LetStatement* letStatement);
    void visitReturnStatement(ReturnStatement* returnStatement);
    void visitIfStatement(IfStatement* ifStatement);
    void visitExpressionStatement(ExpressionStatement* expressionStatement);

    void visitBinaryExpression(BinaryExpression* binaryExpression);
    void visitMethodExpression(MethodExpression* methodExpression);
    void visitFunctionExpression(FunctionExpression* functionExpression);
    void visitObjectExpression(ObjectExpression* objectExpression);
    void visitIntegerExpression(IntegerExpression* integerExpression);

private:
    std::ostream& outputStream;
    int indent;
};
