// Copyright Colin Hill 2011

#pragma once

#include "../SourceLocation.h"
#include "../syntax/Statement.h"
#include "../syntax/SyntaxNode.h"

class Block : public SyntaxNode {
public:
    Block(const SourceLocation& sourceLocation, const StatementArray& statements);
    virtual ~Block() { }

    void accept(Pass* pass);

public:
    StatementArray statements;
};
