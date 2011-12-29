// Copyright Colin Hill 2011

#include "Pass.h"

#include "Scope.h"

#pragma warning(disable : 4624)
#pragma warning(disable : 4146)
#pragma warning(disable : 4996)

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"

class CompilePass : public Pass {
public:
    CompilePass();

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

    void dump();

    const bool didErrorOccur() const;
    const std::string getErrorMessage() const;

private:
    void error(const std::string& errorMessage, const SourceLocation& sourceLocation);

private:
    llvm::Module* module;
    llvm::IRBuilder<> builder;

    llvm::Value* value;

    const llvm::Type* nilType;
    const llvm::Type* integerType;

    bool errorOccurred;
    std::string errorMessage;

    Scope scope;
};
