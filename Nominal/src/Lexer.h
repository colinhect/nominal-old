// Copyright Colin Hill 2011

#include "Token.h"

class Lexer {
public:
    Lexer(const std::string& fileName, const std::string& sourceInput);

    const std::vector<Token> lexInput();

private:
    static bool isKeyword(const std::string& string);
    static bool isOperator(const std::string& string);
    static bool isOperatorChar(const char c);

    static void processString(std::string& value);
    static void searchAndReplace(std::string& value, const std::string& search, const std::string& replace);

private:
    std::string fileName;
    std::string sourceInput;
};
