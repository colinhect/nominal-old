// Copyright Colin Hill 2011

#pragma once

#pragma warning(disable : 4624)
#pragma warning(disable : 4146)
#pragma warning(disable : 4996)

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"

#include <map>
#include <string>

class Scope {
public:
    Scope();
    ~Scope();

    void beginScope();
    void endScope();

    bool objectInScope(const std::string& identifier) const;
    llvm::Value* getObjectValue(const std::string& identifier);
    bool setObjectValue(const std::string& identifier, llvm::Value* value);
    bool defineObject(const std::string& identifier, llvm::Value* value);

private:
    std::map<std::string, llvm::Value*> objects;
    Scope* innerScope;
};