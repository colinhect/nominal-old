// Copyright Colin Hill 2011

#include "Scope.h"

Scope::Scope() :
    innerScope(0) {
}

Scope::~Scope() {
    if (innerScope) {
        delete innerScope;
        innerScope = 0;
    }
}

void Scope::beginScope() {
    if (!innerScope) {
        innerScope = new Scope();
    } else {
        innerScope->beginScope();
    }
}

void Scope::endScope() {
    if (innerScope && !innerScope->innerScope) {
        delete innerScope;
        innerScope = 0;
    } else {
        innerScope->endScope();
    }
}

bool Scope::objectInScope(const std::string& identifier) const {
    bool inCurrentScope = objects.find(identifier) != objects.end();

    if (!inCurrentScope && innerScope) {
        return innerScope->objectInScope(identifier);
    } else {
        return inCurrentScope;
    }
}

llvm::Value* Scope::getObjectValue(const std::string& identifier) {

    // Check inner scope first.
    if (innerScope) {
        llvm::Value* value = innerScope->getObjectValue(identifier);

        // Return the value if not nil.
        if (value) {
            return value;
        }
    }

    // Then check current scope.
    bool inCurrentScope = objects.find(identifier) != objects.end();
    if (inCurrentScope) {
        return objects[identifier];
    } else {
       return 0;
    }
}

bool Scope::setObjectValue(const std::string& identifier, llvm::Value* value) {
    bool inCurrentScope = objects.find(identifier) != objects.end();

    if (!innerScope || (innerScope && !innerScope->setObjectValue(identifier, value))) {
        if (!inCurrentScope) {
            return false;
        } else {
            objects[identifier] = value;
            return true;
        }
    } else {
        return true;
    }
}

bool Scope::defineObject(const std::string& identifier, llvm::Value* value) {
    if (innerScope) {
        return innerScope->defineObject(identifier, value);
    } else {
        if (!objectInScope(identifier)) {
            objects[identifier] = value;

            return true;
        } else {
            return false;
        }
    }
}