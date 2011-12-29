// Copyright Colin Hill 2011

#include "../Pass.h"

#include "Definition.h"

Definition::Definition(const SourceLocation& sourceLocation) :
        SyntaxNode(sourceLocation) {
}

FunctionDefinition::FunctionDefinition(const SourceLocation& sourceLocation, const std::string& name, const ParameterArray& parameters, const std::string& returnTypeName, Block* block) :
        Definition(sourceLocation),
        name(name),
        parameters(parameters),
        returnTypeName(returnTypeName),
        block(block) {
}

void FunctionDefinition::accept(Pass* pass) {
    pass->visitFunctionDefinition(this);
}