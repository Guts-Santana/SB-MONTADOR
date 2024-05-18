#ifndef MONTADOR_HPP
#define MONTADOR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

struct Symbol
{
    int address;
    bool isExtern;
    bool isPublic;
    bool defined;
    std::vector<int> references;
};

class Montador
{
public:
    Montador();
    void assemble(const std::string &inputFile, const std::string &outputFile);

private:
    void parseLine(const std::string &line);
    void handleDirective(const std::string &directive, const std::string &operand);
    void handleInstruction(const std::string &instruction, const std::string &operand);
    void handleCopy(const std::string &instruction, const std::string &operand, std::string &operand2);

    void updateUsageTable(const std::string &symbol, int address);
    void backpatch();
    void writeObjectFile(const std::string &outputFile);

    std::map<std::string, Symbol> symbolTable;
    std::vector<std::pair<std::string, int>> usageTable;
    std::vector<std::string> objectCode;
    std::vector<int> relocationTable;
    int currentAddress;
    bool isBeginEnd;

    std::map<std::string, int> instructionSet;

    std::vector<std::string> split(const std::string &s, char delimiter);
    std::string trim(const std::string &s);
    bool isDirective(const std::string &token);
};

#endif