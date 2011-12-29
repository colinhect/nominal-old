// Copyright Colin Hill 2011

#include <algorithm>

#include "PrettyPrintPass.h"

PrettyPrintPass::PrettyPrintPass(std::ostream& outputStream) :
        outputStream(outputStream),
        indent(0) {
}

void PrettyPrintPass::visitModule(Module* module) {
    outputStream << "module " << module->name->toString() << "\n\n";

    std::for_each(std::begin(module->definitions), std::end(module->definitions), [&](const std::shared_ptr<Definition>& definition) {
        definition->accept(this);
        outputStream << "\n\n";
    });
}

void PrettyPrintPass::visitFunctionDefinition(FunctionDefinition* functionDefinition) {
    outputStream << functionDefinition->name << "(";

    int i = 1, parameterCount = functionDefinition->parameters.size();
    std::for_each(std::begin(functionDefinition->parameters), std::end(functionDefinition->parameters), [&](const std::shared_ptr<Parameter>& parameter) {
        parameter->accept(this);

        if (i++ < parameterCount) {
            outputStream << ", ";
        }
    });

    outputStream << ") " << functionDefinition->returnTypeName << " ";

    functionDefinition->block->accept(this);

    outputStream << "\n";
}

void PrettyPrintPass::visitParameter(Parameter* parameter) {
    outputStream << parameter->name << " " << parameter->typeName;
}

void PrettyPrintPass::visitBlock(Block* block) {
    outputStream << "{\n";
    ++indent;

    std::for_each(std::begin(block->statements), std::end(block->statements), [&](const std::shared_ptr<Statement>& statement) {
        for (int i = 0; i < indent; ++i) {
            outputStream << "    ";
        }

        statement->accept(this);
        outputStream << "\n";
    });

    --indent;

    for (int i = 0; i < indent; ++i) {
        outputStream << "    ";
    }
    outputStream << "}\n";
}

void PrettyPrintPass::visitLetStatement(LetStatement* letStatement) {
     outputStream << "let " << letStatement->objectName << " = ";
     letStatement->expression->accept(this);
}

void PrettyPrintPass::visitReturnStatement(ReturnStatement* returnStatement) {
    outputStream << "return ";
    returnStatement->expression->accept(this);
}

void PrettyPrintPass::visitIfStatement(IfStatement* ifStatement) {
    outputStream << "if ";
    ifStatement->conditionExpression->accept(this);
    outputStream << " ";
    ifStatement->thenBlock->accept(this);

    if (ifStatement->elseBlock->statements.size() > 0) {
        for (int i = 0; i < indent; ++i) {
            outputStream << "    ";
        }

        outputStream << "else ";
        ifStatement->elseBlock->accept(this);
    }
}

void PrettyPrintPass::visitExpressionStatement(ExpressionStatement* expressionStatement) {
    expressionStatement->expression->accept(this);
}

void PrettyPrintPass::visitBinaryExpression(BinaryExpression* binaryExpression) {
    outputStream << "(";
    binaryExpression->leftExpression->accept(this);
    outputStream << " " << binaryExpression->op << " ";
    binaryExpression->rightExpression->accept(this);
    outputStream << ")";
}

void PrettyPrintPass::visitMethodExpression(MethodExpression* methodExpression) {
    methodExpression->objectExpression->accept(this);
    outputStream << "." << methodExpression->methodName << "(";

    int i = 1, argumentCount = methodExpression->arguments.size();
    std::for_each(std::begin(methodExpression->arguments), std::end(methodExpression->arguments), [&](const std::shared_ptr<Expression>& argument) {
        argument->accept(this);

        if (i++ < argumentCount) {
            outputStream << ", ";
        }
    });

    outputStream << ")";
}

void PrettyPrintPass::visitFunctionExpression(FunctionExpression* functionExpression) {
    outputStream << functionExpression->functionName << "(";

    int i = 1, argumentCount = functionExpression->arguments.size();
    std::for_each(std::begin(functionExpression->arguments), std::end(functionExpression->arguments), [&](const std::shared_ptr<Expression>& argument) {
        argument->accept(this);

        if (i++ < argumentCount) {
            outputStream << ", ";
        }
    });

    outputStream << ")";
}

void PrettyPrintPass::visitObjectExpression(ObjectExpression* objectExpression) {
    outputStream << objectExpression->objectName;
}

void PrettyPrintPass::visitIntegerExpression(IntegerExpression* integerExpression) {
    outputStream << integerExpression->value;
}