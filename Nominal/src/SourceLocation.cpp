// Copyright Colin Hill 2011

#include <sstream>

#include "SourceLocation.h"

SourceLocation::SourceLocation() :
    line(0),
    column(0) {
}

SourceLocation::SourceLocation(const std::string& fileName, int line, int column) :
    fileName(fileName),
    line(line),
    column(column) {
}

const std::string& SourceLocation::getFileName() const {
    return fileName;
}

const int SourceLocation::getLine() const {
    return line;
}

const int SourceLocation::getColumn() const {
    return column;
}

const std::string SourceLocation::toString() const {
    std::stringstream ss;
    ss << fileName << ":" << line << ":" << column;

    return ss.str();
}