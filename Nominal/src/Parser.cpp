// Copyright Colin Hill 2011

#include "Parser.h"

Parser::Parser(const std::vector<Token>* tokens) :
        tokens(tokens),
        tokenIndex(0),
        token(0),
        errorOccurred(false) {
    binaryOperatorPrecedences["="]   = 2;

    binaryOperatorPrecedences[".."]  = 4;

    binaryOperatorPrecedences["or"]  = 5;
    binaryOperatorPrecedences["and"] = 5;

    binaryOperatorPrecedences["=="]  = 10;
    binaryOperatorPrecedences["/="]  = 10;

    binaryOperatorPrecedences["<"]   = 20;
    binaryOperatorPrecedences["<="]  = 20;
    binaryOperatorPrecedences[">"]   = 20;
    binaryOperatorPrecedences[">="]  = 20;

    binaryOperatorPrecedences["+"]   = 40;
    binaryOperatorPrecedences["-"]   = 40;
    binaryOperatorPrecedences["*"]   = 80;
    binaryOperatorPrecedences["/"]   = 80;

    nextToken();
}

Module* Parser::parseModule() {
    SourceLocation moduleSourceLocation = token->getSourceLocation();

    if (!consumeKeyword("module")) {
        error("Expected keyword 'module'.", token->getSourceLocation());
        return 0;
    }

    ModuleName* moduleName = parseModuleName();
    if (errorOccurred) {
        return 0;
    }

    if (!consumeDelimiter()) {
        delete moduleName;
        error("Expected delimiter after module name.", token->getSourceLocation());
        return 0;
    }

    DefinitionArray definitions = parseDefinitions();
    if (errorOccurred) {
        return 0;
    }

    if (token->getType() != TOKEN_EOF) {
        error("Unexpected token '" + token->getValue() + "'", token->getSourceLocation());
        return 0;
    }

    return new Module(moduleSourceLocation, moduleName, definitions);
}

ModuleName* Parser::parseModuleName() {
    ModuleName* moduleName = new ModuleName();

    do {
        if (token->getType() != TOKEN_IDENTIFIER || !token->isUpperIdentifier()) {
            delete moduleName;
            error("Invalid module name.", token->getSourceLocation());
            return 0;
        }

        moduleName->addNameSpace(token->getStringValue());
        nextToken();
    } while (consumeSymbol("."));

    return moduleName;
}

Definition* Parser::parseDefinition() {
    return parseFunctionDefinition();
}

FunctionDefinition* Parser::parseFunctionDefinition() {
    SourceLocation sourceLocation = token->getSourceLocation();

    if (!token->isLowerIdentifier()) {
        error("Expected function name.", token->getSourceLocation());
        return 0;
    }

    std::string name = token->getStringValue();
    nextToken();

    ParameterArray parameters = parseParameters();
    if (errorOccurred) {
        return 0;
    }

    std::string returnTypeName = "Nil";
    if (token->isUpperIdentifier()) {
        returnTypeName = token->getStringValue();
        nextToken();
    }
    
    Block* block = parseBlock();
    if (errorOccurred) {
        return 0;
    }

    return new FunctionDefinition(sourceLocation, name, parameters, returnTypeName, block);
}

DefinitionArray Parser::parseDefinitions() {
    DefinitionArray definitions;
    while (token->getType() != TOKEN_EOF) {
        definitions.push_back(std::shared_ptr<Definition>(parseDefinition()));

        if (errorOccurred) {
            return DefinitionArray();
        }

        if (!consumeDelimiter() && token->getType() != TOKEN_EOF) {
            error("Expected delimiter after definition.", token->getSourceLocation());
            return DefinitionArray();
        }
    }

    return definitions;
}

Parameter* Parser::parseParameter() {
    SourceLocation sourceLocation = token->getSourceLocation();

    if (!token->isLowerIdentifier()) {
        error("Expected parameter name.", token->getSourceLocation());
        return 0;
    }

    std::string name = token->getStringValue();
    nextToken();

    if (!token->isUpperIdentifier()) {
        error("Expected parameter type name.", token->getSourceLocation());
        return 0;
    }

    std::string typeName = token->getStringValue();
    nextToken();

    return new Parameter(sourceLocation, name, typeName);
}

ParameterArray Parser::parseParameters() {
    if (!consumeSymbol("(")) {
        error("Expected opening '('.", token->getSourceLocation());
        return ParameterArray();
    }

    ParameterArray parameters;
    if (!token->isSymbol(")")) {
        do {
            parameters.push_back(std::shared_ptr<Parameter>(parseParameter()));

            if (errorOccurred) {
                return ParameterArray();
            }
        } while (consumeSymbol(","));
    }

    if (!consumeSymbol(")")) {
        error("Expected closing ')'.", token->getSourceLocation());
        return ParameterArray();
    }

    return parameters;
}

Block* Parser::parseBlock() {
    SourceLocation sourceLocation = token->getSourceLocation();

    if (!consumeSymbol("{")) {
        error("Expected opening '{'.", token->getSourceLocation());
        return 0;
    }

    consumeDelimiter();

    StatementArray statements;
    do {
        if (token->isSymbol("}")) {
            break;
        }

        statements.push_back(std::shared_ptr<Statement>(parseStatement()));

        if (errorOccurred) {
            return 0;
        }
    } while (consumeDelimiter());

    if (!consumeSymbol("}")) {
        error("Expected closing '}'.", token->getSourceLocation());
        return 0;
    }

    return new Block(sourceLocation, statements);
}

Statement* Parser::parseStatement() {
    if (token->isKeyword("let")) {
        return parseLetStatement();
    } else if (token->isKeyword("return")) {
        return parseReturnStatement();
    } else if (token->isKeyword("if")) {
        return parseIfStatement();
    } else {
        return parseExpressionStatement();
    }
}

LetStatement* Parser::parseLetStatement() {
    SourceLocation sourceLocation = token->getSourceLocation();

    if (!consumeKeyword("let")) {
        error("Expected keyword 'let'.", token->getSourceLocation());
        return 0;
    }

    if (!token->isLowerIdentifier()) {
        error("Expected object name.", token->getSourceLocation());
        return 0;
    }

    std::string objectName = token->getStringValue();
    nextToken();

    if (!consumeSymbol("=")) {
        error("Expected symbol '='.", token->getSourceLocation());
        return 0;
    }

    Expression* expression = parseExpression();
    if (errorOccurred) {
        return 0;
    }

    return new LetStatement(sourceLocation, objectName, expression);
}

ReturnStatement* Parser::parseReturnStatement() {
    SourceLocation sourceLocation = token->getSourceLocation();

    if (!consumeKeyword("return")) {
        error("Expected keyword 'return'.", token->getSourceLocation());
        return 0;
    }

    Expression* expression = parsePrimaryExpression();
    if (errorOccurred) {
        return 0;
    }

    return new ReturnStatement(sourceLocation, expression);
}

IfStatement* Parser::parseIfStatement() {
    SourceLocation sourceLocation = token->getSourceLocation();

    if (!consumeKeyword("if")) {
        error("Expected keyword 'if'.", token->getSourceLocation());
        return 0;
    }

    Expression* conditionExpression = parseExpression();
    if (errorOccurred) {
        return 0;
    }

    Block* thenBlock = parseBlock();
    if (errorOccurred) {
        return 0;
    }

    Block* elseBlock = 0;
    if (consumeKeyword("else")) {
        elseBlock = parseBlock();
        if (errorOccurred) {
            return 0;
        }
    } else {
        elseBlock = new Block(token->getSourceLocation(), StatementArray());
    }

    return new IfStatement(sourceLocation, conditionExpression, thenBlock, elseBlock);
}

ExpressionStatement* Parser::parseExpressionStatement() {
    SourceLocation sourceLocation = token->getSourceLocation();

    Expression* expression = parseExpression();
    if (errorOccurred) {
        return 0;
    }

    return new ExpressionStatement(sourceLocation, expression);
}

Expression* Parser::parseExpression() {
    return parseBinaryExpression(0, parsePrimaryExpression());
}

Expression* Parser::parseBinaryExpression(int exprPrec, Expression* leftExpression) {
    while (1) {
        int opPrec = operatorPrecedence();

        // No operator.
        if (opPrec < exprPrec) {
            return leftExpression;
        }

        // Operator.
        std::string op = token->getStringValue();
        nextToken();

        // Right hand side.
        Expression* rightExpression;
        rightExpression = parsePrimaryExpression();
        if (errorOccurred) {
            delete leftExpression;
            return 0;
        }

        // Operator precedence.
        int nextOpPrec = operatorPrecedence();

        if (opPrec < nextOpPrec) {
            rightExpression = parseBinaryExpression(opPrec + 1, rightExpression);
            if (errorOccurred) {
                delete leftExpression;
                return 0;
            }
        }

        leftExpression = new BinaryExpression(leftExpression->sourceLocation, op, leftExpression, rightExpression);
    }

    return 0;
}

Expression* Parser::parsePrimaryExpression() {
    SourceLocation sourceLocation = token->getSourceLocation();

    Expression* expression;

    if (consumeSymbol("(")) {
        expression = parsePrimaryExpression();
        if (errorOccurred) {
            return 0;
        }

        if (!consumeSymbol(")")) {
            error("Expected closing ')'.", token->getSourceLocation());
            return 0;
        }
    } else if (token->getType() == TOKEN_INTEGER) {
        int value = token->getIntegerValue();
        nextToken();

        return new IntegerExpression(sourceLocation, value);
    } else {
        std::string name = token->getStringValue();
        expression = parseObjectExpression();

        if (errorOccurred) {
            return 0;
        }

        // Function expression.
        if (token->isSymbol("(")) {
            ExpressionArray arguments = parseArguments();
            if (errorOccurred) {
                return 0;
            }

            return new FunctionExpression(sourceLocation, name, arguments);
        }
    }
 
    // Method expression.
    if (consumeSymbol(".")) {
        if (!token->isLowerIdentifier()) {
            error("Expected method name.", token->getSourceLocation());
            return 0;
        }

        std::string methodName = token->getStringValue();
        nextToken();

        ExpressionArray arguments = parseArguments();
        if (errorOccurred) {
            return 0;
        }

        return new MethodExpression(sourceLocation, methodName, expression, arguments);
    } else {
        return expression;
    }
}

ObjectExpression* Parser::parseObjectExpression() {
    std::string objectName = token->getStringValue();
    SourceLocation sourceLocation = token->getSourceLocation();

    nextToken();

    return new ObjectExpression(sourceLocation, objectName);
}

ExpressionArray Parser::parseArguments() {
    if (!consumeSymbol("(")) {
        error("Expected opening '('.", token->getSourceLocation());
        return ExpressionArray();
    }

    ExpressionArray arguments;
    do {
        if (token->isSymbol(")")) {
            break;
        }

        arguments.push_back(std::shared_ptr<Expression>(parsePrimaryExpression()));

        if (errorOccurred) {
            return ExpressionArray();
        }
    } while (consumeSymbol(","));

    if (!consumeSymbol(")")) {
        error("Expected closing ')'.", token->getSourceLocation());
        return ExpressionArray();
    }

    return arguments;
}

const bool Parser::didErrorOccur() const {
    return errorOccurred;
}

const std::string Parser::getErrorMessage() const {
    return errorMessage;
}

bool Parser::consumeKeyword(const std::string& keyword) {
    if (token->isKeyword(keyword)) {
        nextToken();

        return true;
    } else {
        return false;
    }
}

bool Parser::consumeSymbol(const std::string& symbol) {
    if (token->isSymbol(symbol)) {
        nextToken();

        return true;
    } else {
        return false;
    }
}

bool Parser::consumeDelimiter() {
    if (token->getType() == TOKEN_DELIMITER) {
        nextToken();

        return true;
    } else {
        return false;
    }
}

void Parser::nextToken() {
    if (tokenIndex > (int)(tokens->size() - 1)) {
        error("No more tokens.", token->getSourceLocation());
    }

    token = &(*tokens)[tokenIndex++];
}

void Parser::pushState() {
    tokenStack.push(tokenIndex - 1);
}

void Parser::popState() {
    tokenIndex = tokenStack.top();
    tokenStack.pop();

    nextToken();
}

int Parser::operatorPrecedence() {
    std::string op = token->getStringValue();

    if (token->getType() != TOKEN_OPERATOR) {
        return -1;
    } else if (binaryOperatorPrecedences.find(op) == binaryOperatorPrecedences.end()) {
        error("Invalid binary operator '" + op + "'.", token->getSourceLocation());
        return -1;
    } else {
        return binaryOperatorPrecedences[op];
    }
}

void Parser::error(const std::string& errorMessage, const SourceLocation& sourceLocation) {
    this->errorMessage = errorMessage + " (" + sourceLocation.toString() + ")";
    errorOccurred = true;
}