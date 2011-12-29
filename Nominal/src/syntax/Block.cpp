// Copyright Colin Hill 2011

#include "../Pass.h"

#include "Block.h"

Block::Block(const SourceLocation& sourceLocation, const StatementArray& statements) :
        SyntaxNode(sourceLocation),
        statements(statements) {
}

void Block::accept(Pass* pass) {
    pass->visitBlock(this);
}