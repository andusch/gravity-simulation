#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

#endif // IO_H