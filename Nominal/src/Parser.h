// Copyright Colin Hill 2011

#pragma once

#include "Token.h"

#include "syntax/Block.h"
#include "syntax/Module.h"
#include "syntax/Definition.h"
#include "syntax/Expression.h"
#include "syntax/Parameter.h"
#include "syntax/Statement.h"

#include <map>
#include <stack>

class Parser {
public:
    Parser(const std::vector<Token>* tokens);

    Module* parseModule();
    ModuleName* parseModuleName();

    Definition* parseDefinition();
    FunctionDefinition* parseFunctionDefinition();
    DefinitionArray parseDefinitions();

    Parameter* parseParameter();
    ParameterArray parseParameters();

    Block* parseBlock();

    Statement* parseStatement();
    LetStatement* parseLetStatement();
    ReturnStatement* parseReturnStatement();
    IfStatement* parseIfStatement();
    ExpressionStatement* parseExpressionStatement();

    Expression* parseExpression();
    Expression* parseBinaryExpression(int exprPrec, Expression* leftExpression);
    Expression* parsePrimaryExpression();
    ObjectExpression* parseObjectExpression();

    ExpressionArray parseArguments();

    const bool didErrorOccur() const;

    const std::string getErrorMessage() const;

private:
    bool consumeKeyword(const std::string& keyword);
    bool consumeOperator(const std::string& op);
    bool consumeSymbol(const std::string& symbol);
    bool consumeDelimiter();

    void nextToken();

    void pushState();
    void popState();

    int operatorPrecedence();

    void error(const std::string& errorMessage, const SourceLocation& sourceLocation);

private:
    const std::vector<Token>* tokens;

    int tokenIndex;
    std::stack<int> tokenStack;

    bool errorOccurred;
    std::string errorMessage;

    const Token* token;

    std::map<std::string, int> binaryOperatorPrecedences;
};
