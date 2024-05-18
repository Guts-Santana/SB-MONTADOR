#ifndef MONTADOR_HPP
#define MONTADOR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "pre_processamento.hpp"

struct Symbol
{
    int address;
    bool isExtern;
    bool isPublic;
};

class Montador
{
public:
    Montador();
    void assemble(const std::string &inputFile, const std::string &outputFile);
    std::unordered_map<std::string, int> instructionSet;

private:
    void parseLine(const std::string &line);
    void handleDirective(const std::string &directive, const std::string &operand);
    void handleInstruction(const std::string &instruction, const std::string &operand);
    void updateUsageTable(const std::string &symbol, int address);
    void writeObjectFile(const std::string &outputFile);

    std::map<std::string, Symbol> symbolTable;
    std::vector<std::pair<std::string, int>> usageTable;
    std::vector<std::string> objectCode;
    int currentAddress;
    bool isBeginEnd;

    // Helper functions
    std::vector<std::string> split(const std::string &s, char delimiter);
    std::string trim(const std::string &s);
};

#endif // MONTADOR_HPP
