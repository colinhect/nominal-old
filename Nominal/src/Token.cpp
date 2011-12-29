// Copyright Colin Hill 2011

#include <cctype>
#include <sstream>

#include "Token.h"

Token::Token(TokenType type, SourceLocation sourceLocation, const std::string& stringValue) :
    type(type),
    sourceLocation(sourceLocation),
    stringValue(stringValue),
    integerValue(0) {
}

Token::Token(TokenType type, SourceLocation sourceLocation, int integerValue) :
    type(type),
    sourceLocation(sourceLocation),
    stringValue(),
    integerValue(integerValue) {
}

const bool Token::isLowerIdentifier() const {
    return type == TOKEN_IDENTIFIER  && islower(stringValue[0]);
}

const bool Token::isUpperIdentifier() const {
    return type == TOKEN_IDENTIFIER  && isupper(stringValue[0]);
}

const bool Token::isLowerIdentifier(const std::string& identifier) const {
    return type == TOKEN_IDENTIFIER  && islower(stringValue[0]) && stringValue == identifier;
}

const bool Token::isUpperIdentifier(const std::string& identifier) const {
    return type == TOKEN_IDENTIFIER  && isupper(stringValue[0]) && stringValue == identifier;
}

const bool Token::isKeyword(const std::string& keyword) const {
    return type == TOKEN_KEYWORD && stringValue == keyword;
}

const bool Token::isSymbol(const std::string& symbol) const {
    return type == TOKEN_SYMBOL && stringValue == symbol;
}

const TokenType Token::getType() const {
    return type;
}

const SourceLocation& Token::getSourceLocation() const {
    return sourceLocation;
}

const std::string& Token::getStringValue() const {
    return stringValue;
}

const int Token::getIntegerValue() const {
    return integerValue;
}

const std::string Token::getValue() const {
    if (type == TOKEN_INTEGER) {
        std::stringstream ss;
        ss << integerValue;
        return ss.str();
    } else if (type == TOKEN_DELIMITER) {
        return ";";
    } else if (type == TOKEN_EOF) {
        return "<eof>";
    } else {
        return getStringValue();
    }
}

const std::string Token::toString() const {
    if (type == TOKEN_KEYWORD) {
        return "[Keyword: " + stringValue + "]";
    } else if (type == TOKEN_IDENTIFIER) {
        return "[Identifier: " + stringValue + "]";
    } else if (type == TOKEN_INTEGER) {
        std::stringstream ss;
        ss << integerValue;
        return "[Integer: " + ss.str() + "]";
    } else if (type == TOKEN_IDENTIFIER) {
        return "[Identifier: " + stringValue + "]";
    } else if (type == TOKEN_SYMBOL) {
        return "[Symbol: " + stringValue + "]";
    } else if (type == TOKEN_DELIMITER) {
        return "[Delimiter]";
    } else if (type == TOKEN_EOF) {
        return "[EOF]";
    } else {
        return "[Unknown]";
    }
}