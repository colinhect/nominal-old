// Copyright Colin Hill 2011

#pragma once

#include <vector>

#include "SourceLocation.h"

enum TokenType {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_SYMBOL,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_EOF
};

class Token {
public:
    Token(TokenType type, SourceLocation sourceLocation, const std::string& stringValue = "");
    Token(TokenType type, SourceLocation sourceLocation, int integerValue);

    const bool isLowerIdentifier() const;
    const bool isUpperIdentifier() const;
    const bool isLowerIdentifier(const std::string& identifier) const;
    const bool isUpperIdentifier(const std::string& identifier) const;
    const bool isKeyword(const std::string& keyword) const;
    const bool isSymbol(const std::string& symbol) const;

    const TokenType getType() const;

    const SourceLocation& getSourceLocation() const;

    const std::string& getStringValue() const;
    const int getIntegerValue() const;
    const std::string getValue() const;

    const std::string toString() const;

private:
    TokenType type;

    SourceLocation sourceLocation;

    std::string stringValue;
    int integerValue;
    double realValue;
    bool booleanValue;
};
