// Copyright Colin Hill 2011

#pragma once

#include "../syntax/Definition.h"
#include "../syntax/SyntaxNode.h"

#include <memory>
#include <vector>

class ModuleName {
public:
    void addNameSpace(const std::string& nameSpace);

    const std::string toString() const;

public:
    std::vector<std::string> nameSpaces;
};

class Module : public SyntaxNode {
public:
    Module(const SourceLocation& sourceLocation, ModuleName* name, const DefinitionArray& definitions);
    virtual ~Module() { }

    void accept(Pass* pass);

public:
    std::unique_ptr<ModuleName> name;
    DefinitionArray definitions;
};