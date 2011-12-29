// Copyright Colin Hill 2011

#include <algorithm>

#include "AnalysisPass.h"

void AnalysisPass::visitModule(Module* module) {
    std::for_each(std::begin(module->definitions), std::end(module->definitions), [&](const std::shared_ptr<Definition>& definition) {
        definition->accept(this);
    });
}

void AnalysisPass::visitFunctionDefinition(FunctionDefinition* functionDefinition) {
}

void AnalysisPass::visitParameter(Parameter* parameter) {
}

void AnalysisPass::visitBlock(Block* block) {
}

void AnalysisPass::visitLetStatement(LetStatement* letStatement) {
}

void AnalysisPass::visitReturnStatement(ReturnStatement* returnStatement) {
}

void AnalysisPass::visitIfStatement(IfStatement* ifStatement) {
}

void AnalysisPass::visitExpressionStatement(ExpressionStatement* expressionStatement) {
}

void AnalysisPass::visitBinaryExpression(BinaryExpression* binaryExpression) {
}

void AnalysisPass::visitMethodExpression(MethodExpression* methodExpression) {
}

void AnalysisPass::visitFunctionExpression(FunctionExpression* functionExpression) {
}

void AnalysisPass::visitObjectExpression(ObjectExpression* objectExpression) {
}

void AnalysisPass::visitIntegerExpression(IntegerExpression* integerExpression) {
}