// Copyright Colin Hill 2011

#pragma once

#include "../syntax/SyntaxNode.h"

#include <memory>
#include <vector>

class Parameter : public SyntaxNode {
public:
    Parameter(const SourceLocation& sourceLocation, const std::string& name, const std::string& typeName);
    virtual ~Parameter() { }

    void accept(Pass* pass);

public:
    std::string name;
    std::string typeName;
};

typedef std::vector<std::shared_ptr<Parameter>> ParameterArray;