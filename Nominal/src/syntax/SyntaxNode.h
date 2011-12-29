// Copyright Colin Hill 2011

#pragma once

#include "../SourceLocation.h"

class Pass;

class SyntaxNode {
public:
    SyntaxNode(const SourceLocation& sourceLocation);
    virtual ~SyntaxNode() { }

    virtual void accept(Pass* pass) = 0;

public:
    SourceLocation sourceLocation;
};