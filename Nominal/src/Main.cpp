#include "Lexer.h"
#include "Parser.h"
#include "CompilePass.h"
#include "PrettyPrintPass.h"

#include <algorithm>
#include <iostream>
#include <fstream>

std::string readFile(const std::string& filePath);
std::string readInputStream(std::istream& inputStream);

void outputTokens(const std::vector<Token>& tokens, std::ostream& outputStream);

int main() {
    std::string fileName("Test.ns");

    std::string sourceInput = readFile(fileName);

    std::cout << "Source Input: \n" << sourceInput << "\n\n";

    Lexer lexer(fileName, sourceInput);
    std::vector<Token> tokens = lexer.lexInput();

    std::cout << "Tokens: \n";
    outputTokens(tokens, std::cout);
    std::cout << "\n";

    Parser parser(&tokens);
    Module* module = parser.parseModule();

    if (parser.didErrorOccur()) {
        std::cout << parser.getErrorMessage() << std::endl;
    } else {
        PrettyPrintPass prettyPrintPass(std::cout);
        module->accept(&prettyPrintPass);

        CompilePass compilePass;
        module->accept(&compilePass);

        if (compilePass.didErrorOccur()) {
            std::cout << compilePass.getErrorMessage() << std::endl;
        } else {
            compilePass.dump();
        }
    }

    system("PAUSE");
    return 0;
}

std::string readFile(const std::string& filePath) {
    std::ifstream inputStream(filePath.c_str());

    if (inputStream.is_open()) {
        return readInputStream(inputStream);
    } else {
        throw std::runtime_error("Failed to open input file.");
    }
}

std::string readInputStream(std::istream& inputStream) {
    std::string input;
    while (!inputStream.eof()) {
        input += inputStream.get();
    }

    return input;
}

void outputTokens(const std::vector<Token>& tokens, std::ostream& outputStream) {
    std::for_each(std::begin(tokens), std::end(tokens), [&](const Token& token) {
        outputStream << token.toString() << " ";
    });
}