// Copyright Colin Hill 2011

#include "../Pass.h"

#include <algorithm>
#include <sstream>

#include "Module.h"

void ModuleName::addNameSpace(const std::string& nameSpace) {
    nameSpaces.push_back(nameSpace);
}

const std::string ModuleName::toString() const {
    std::stringstream ss;

    int i = 1, nameSpaceCount = nameSpaces.size();
    std::for_each(std::begin(nameSpaces), std::end(nameSpaces), [&](const std::string& nameSpace) {
        ss << nameSpace;

        if (i++ < nameSpaceCount) {
            ss << ".";
        }
    });

    return ss.str();
}

Module::Module(const SourceLocation& sourceLocation, ModuleName* name, const DefinitionArray& definitions) :
        SyntaxNode(sourceLocation),
        name(name),
        definitions(definitions) {
}

void Module::accept(Pass* pass) {
    pass->visitModule(this);
}