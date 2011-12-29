// Copyright Colin Hill 2011

#include "../Pass.h"

#include "Parameter.h"

Parameter::Parameter(const SourceLocation& sourceLocation, const std::string& name, const std::string& typeName) :
        SyntaxNode(sourceLocation),
        name(name),
        typeName(typeName) {
}

void Parameter::accept(Pass* pass) {
    pass->visitParameter(this);
}