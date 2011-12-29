// Copyright Colin Hill 2011

#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include "Lexer.h"

Lexer::Lexer(const std::string& fileName, const std::string& sourceInput) :
    fileName(fileName),
    sourceInput(sourceInput) {
}

const std::vector<Token> Lexer::lexInput() {
    std::vector<Token> tokens;

    int i = 0;
    int length = sourceInput.length();

    int line = 1;
    int column = 1;

    char c = sourceInput[i++];
    while (i < length) {

        // Delimiter.
        if (c == '\n') {
            tokens.push_back(Token(TOKEN_DELIMITER, SourceLocation(fileName, line, column - 1)));

            while (isspace(c)) {
                if (c == '\n') {
                    line++;
                    column = 1;
                }
                else if (c == '\t') {
                    column += 2;
                }
                else if (c == ' ') {
                    column++;
                }

                c = sourceInput[i++];
            }
            continue;
        }

        // Whitespace.
        if (isspace(c)) {
            while (isspace(c)) {
                if (c == '\t') {
                    column += 2;
                }
                else if (c == ' ') {
                    column++;
                }

                c = sourceInput[i++];
            }
            continue;
        }

        // Identifier.
        if (isalpha(c)) {
            std::string stringValue;

            while (isalnum(c)) {
                stringValue += c;
                c = sourceInput[i++];

                column++;
            }

            if (isKeyword(stringValue)) {
                tokens.push_back(Token(TOKEN_KEYWORD, SourceLocation(fileName, line, column - stringValue.length()), stringValue));
            }
            else {
                tokens.push_back(Token(TOKEN_IDENTIFIER, SourceLocation(fileName, line, column - stringValue.length()), stringValue));
            }
            continue;
        }

        // Integer or real.
        if (isdigit(c)) {
            std::string stringValue;

            while (isdigit(c)) {
                stringValue += c;
                c = sourceInput[i++];

                column++;
            }

            /*
            // Real.
            if (c == '.') {
                stringValue += c;
                c = sourceInput[i++];

                column++;

                while (isdigit(c)) {
                    stringValue += c;
                    c = sourceInput[i++];

                    column++;
                }

                double realValue = atof(stringValue.c_str());
                tokens.push_back(Token(TOKEN_REAL, SourceLocation(line, column - stringValue.length()), realValue));
            }

            // Integer.
            else {
                int integerValue = atoi(stringValue.c_str());
                tokens.push_back(Token(TOKEN_INTEGER, SourceLocation(line, column - stringValue.length()), integerValue));
            }
            */

            // Just integer.
            int integerValue = atoi(stringValue.c_str());
            tokens.push_back(Token(TOKEN_INTEGER, SourceLocation(fileName, line, column - stringValue.length()), integerValue));

            continue;
        }

        /*
        // String.
        if (c == '\"') {
            c = sourceInput[i++];
            column++;

            std::string stringValue;
            do {
                stringValue += c;
                c = sourceInput[i++];

                column++;
            } while(c != '\"');
            c = sourceInput[i++];
            column++;

            processString(stringValue);

            tokens.push_back(Token(TOKEN_STRING, SourceLocation(line, column - stringValue.length()), stringValue));
            continue;
        }
        */

        // Anything else is a symbol.
        std::string stringValue;

        do {
            stringValue += c;
            c = sourceInput[i++];

            column++;
        } while (isOperatorChar(c)); // Consume operator concatenations as single tokens.

        if (isOperator(stringValue)) {
            tokens.push_back(Token(TOKEN_OPERATOR, SourceLocation(fileName, line, column - stringValue.length()), stringValue));
        } else {
            column++;

            int i = 0;
            std::for_each(std::begin(stringValue), std::end(stringValue), [&](const char c) {
                ++i;
                std::string symbol;
                symbol.push_back(c);

                tokens.push_back(Token(TOKEN_SYMBOL, SourceLocation(fileName, line, column - (stringValue.length() - i)), symbol));
            });
        }
    }

    tokens.push_back(Token(TOKEN_EOF, SourceLocation(fileName, line, column)));
    return tokens;
}

bool Lexer::isKeyword(const std::string& string) {
    static std::string keywordStrings[] = {
        "module", "import", "let", "this", "class", "return", "if", "else", "true", "false", "nil"
    };

    static int numKeywords = sizeof(keywordStrings) / sizeof(std::string);

    for (int i = 0; i < numKeywords; ++i) {
        if (keywordStrings[i] == string) {
            return true;
        }
    }

    return false;
}

bool Lexer::isOperator(const std::string& string) {
    static std::string operatorStrings[] = {
        "+", "-", "*", "/", "==", "/=", ">", "<", ">=", "<="
    };

    static int numOperators = sizeof(operatorStrings) / sizeof(std::string);

    for (int i = 0; i < numOperators; ++i) {
        if (operatorStrings[i] == string) {
            return true;
        }
    }

    return false;
}

bool Lexer::isOperatorChar(const char c) {
    static char operatorChars[] = {
        '+', '-', '*', '/', '=', '>', '<'
    };

    static int numOperatorChars = sizeof(operatorChars) / sizeof(std::string);

    for (int i = 0; i < numOperatorChars; ++i) {
        if (operatorChars[i] == c) {
            return true;
        }
    }

    return false;
}

void Lexer::processString(std::string& value) {
    searchAndReplace(value, "\\n", "\n");
    searchAndReplace(value, "\\t", "\t");
}

void Lexer::searchAndReplace(std::string& value, const std::string& search, const std::string& replace) {
    std::string::size_type  next;

    for (next = value.find(search); next != std::string::npos; next = value.find(search,next)) {
        value.replace(next,search.length(),replace);
        next += replace.length();
    }
}