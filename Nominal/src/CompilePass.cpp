// Copyright Colin Hill 2011

#include <algorithm>

#include "CompilePass.h"

CompilePass::CompilePass() :
    builder(llvm::getGlobalContext()),
    value(0),
    errorOccurred(false) {
}

void CompilePass::visitModule(Module* module) {
    this->module = new llvm::Module(module->name->toString(), llvm::getGlobalContext());

    nilType = llvm::Type::getVoidTy(llvm::getGlobalContext());
    integerType = llvm::Type::getInt32Ty(llvm::getGlobalContext());

    this->module->addTypeName("Nil", nilType);
    this->module->addTypeName("Integer", integerType);

    std::for_each(std::begin(module->definitions), std::end(module->definitions), [&](const std::shared_ptr<Definition>& definition) {
        definition->accept(this);
    });
}

void CompilePass::visitFunctionDefinition(FunctionDefinition* functionDefinition) {
    const llvm::Type* returnType = module->getTypeByName(functionDefinition->returnTypeName);

    if (!returnType) {
        error("No such type '" + functionDefinition->returnTypeName + "'.", functionDefinition->sourceLocation);
        return;
    }

    std::vector<const llvm::Type*> parameterTypes;
    bool parameterError = false;
    std::for_each(std::begin(functionDefinition->parameters), std::end(functionDefinition->parameters), [&](const std::shared_ptr<Parameter>& parameter) {
        const llvm::Type* parameterType = module->getTypeByName(parameter->typeName);

        if (!parameterType) {
            error("Parameter '" + parameter->name + "' for function '" + functionDefinition->name + "' has invalid type '" + parameter->typeName + "'.", parameter->sourceLocation);
            parameterError = true;
            return;
        }

        parameterTypes.push_back(parameterType);
    });

    if (parameterError) {
        return;
    }

    llvm::FunctionType* functionType = llvm::FunctionType::get(returnType, parameterTypes, false);

    llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, functionDefinition->name, module);

    scope.beginScope();

    unsigned index = 0;
    for (llvm::Function::arg_iterator it = function->arg_begin(); index != functionDefinition->parameters.size(); ++it, ++index) {
        it->setName(functionDefinition->parameters[index]->name);
        scope.defineObject(functionDefinition->parameters[index]->name, it);
    }

    llvm::BasicBlock* basicBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", function);
    builder.SetInsertPoint(basicBlock);

    functionDefinition->block->accept(this);

    scope.endScope();
    scope.defineObject(functionDefinition->name, function);
}

void CompilePass::visitParameter(Parameter* parameter) {
}

void CompilePass::visitBlock(Block* block) {
    std::for_each(std::begin(block->statements), std::end(block->statements), [&](const std::shared_ptr<Statement>& statement) {
        statement->accept(this);
    });
}

void CompilePass::visitLetStatement(LetStatement* letStatement) {
    letStatement->expression->accept(this);
    
    llvm::AllocaInst* valuePtr = builder.CreateAlloca(value->getType());
    builder.CreateStore(value, valuePtr, false);
    llvm::LoadInst* loadedValue = builder.CreateLoad(valuePtr, false, letStatement->objectName);

    scope.defineObject(letStatement->objectName, loadedValue);
}

void CompilePass::visitReturnStatement(ReturnStatement* returnStatement) {
    if (returnStatement->expression) {
        returnStatement->expression->accept(this);
        builder.CreateRet(value);
        value = 0;
    } else {
        builder.CreateRetVoid();
    }
}

void CompilePass::visitIfStatement(IfStatement* ifStatement) {
}

void CompilePass::visitExpressionStatement(ExpressionStatement* expressionStatement) {
    expressionStatement->expression->accept(this);
}

void CompilePass::visitBinaryExpression(BinaryExpression* binaryExpression) {
    binaryExpression->leftExpression->accept(this);
    llvm::Value* leftValue = value;

    binaryExpression->rightExpression->accept(this);
    llvm::Value* rightValue = value;

    if (binaryExpression->op == "+") {
        value = builder.CreateAdd(leftValue, rightValue);
    } else if (binaryExpression->op == "-") {
        value = builder.CreateSub(leftValue, rightValue);
    } else if (binaryExpression->op == "*") {
        value = builder.CreateMul(leftValue, rightValue);
    } else if (binaryExpression->op == "/") {
        value = builder.CreateSDiv(leftValue, rightValue);
    }
}

void CompilePass::visitMethodExpression(MethodExpression* methodExpression) {
}

void CompilePass::visitFunctionExpression(FunctionExpression* functionExpression) {
    std::vector<llvm::Value*> arguments;
    std::for_each(std::begin(functionExpression->arguments), std::end(functionExpression->arguments), [&](const std::shared_ptr<Expression>& argument) {
        argument->accept(this);
        arguments.push_back(value);
    });

    /*
    llvm::Value* function = scope.getObjectValue(functionExpression->functionName);
    if (!function) {
        error("Undefined function '" + functionExpression->functionName + "'.", functionExpression->sourceLocation);
        return;
    }

    builder.CreateCall(function, std::begin(arguments), std::end(arguments));*/
}

void CompilePass::visitObjectExpression(ObjectExpression* objectExpression) {
    if (!scope.objectInScope(objectExpression->objectName)) {
        error("Object '" + objectExpression->objectName + "' not in scope.", objectExpression->sourceLocation);
        return;
    }

    value = scope.getObjectValue(objectExpression->objectName);
}

void CompilePass::visitIntegerExpression(IntegerExpression* integerExpression) {
    value = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, integerExpression->value, true));
}

void CompilePass::dump() {
    module->dump();
}

const bool CompilePass::didErrorOccur() const {
    return errorOccurred;
}

const std::string CompilePass::getErrorMessage() const {
    return errorMessage;
}

void CompilePass::error(const std::string& errorMessage, const SourceLocation& sourceLocation) {
    this->errorMessage = errorMessage + " (" + sourceLocation.toString() + ")";
    errorOccurred = true;
}