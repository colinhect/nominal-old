// Copyright Colin Hill 2011

#pragma once

#include "../SourceLocation.h"
#include "../syntax/Block.h"
#include "../syntax/Parameter.h"
#include "../syntax/Statement.h"
#include "../syntax/SyntaxNode.h"

#include <memory>
#include <vector>

class Definition : public SyntaxNode {
public:
    Definition(const SourceLocation& sourceLocation);
    virtual ~Definition() { }
};

typedef std::vector<std::shared_ptr<Definition>> DefinitionArray;

class FunctionDefinition : public Definition {
public:
    FunctionDefinition(const SourceLocation& sourceLocation, const std::string& name, const ParameterArray& parameters, const std::string& returnTypeName, Block* block);
    virtual ~FunctionDefinition() { }

    void accept(Pass* pass);

public:
    std::string name;
    ParameterArray parameters;
    std::string returnTypeName;
    std::unique_ptr<Block> block;
};