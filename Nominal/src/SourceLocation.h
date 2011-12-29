// Copyright Colin Hill 2011

#pragma once

#include <string>

class SourceLocation {
public:
    SourceLocation();
    SourceLocation(const std::string& fileName, int line, int column);

    const std::string& getFileName() const;

    const int getLine() const;
    const int getColumn() const;

    const std::string toString() const;

private:
    std::string fileName;

    int line;
    int column;
};