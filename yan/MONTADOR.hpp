#ifndef MONTADOR_HPP
#define MONTADOR_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>

class Montador
{
public:
    Montador();
    void assemble(const std::string &inputFile, const std::string &outputFile);
    void writePendingList(const std::string &outputFile);

private:
    struct Symbol
    {
        int address;
        bool isExtern;
        bool isPublic;
        bool defined;
        std::vector<int> references;
    };

    struct Usage
    {
        std::string symbol;
        int address;
    };

    void parseLine(const std::string &line);
    bool isDirective(const std::string &token);
    void handleDirective(const std::string &directive, const std::string &operand);
    void handleInstruction(const std::string &instruction, const std::string &operands);
    void updateUsageTable(const std::string &symbol, int address);
    void backpatch();
    void writeObjectFile(const std::string &outputFile);
    std::vector<std::string> split(const std::string &s, char delimiter);
    std::string trim(const std::string &s);
    std::string toHex(int num);

    int currentAddress;
    bool isBeginEnd;
    std::map<std::string, int> instructionSet;
    std::map<std::string, Symbol> symbolTable;
    std::vector<std::string> objectCode;
    std::vector<int> relocationTable;
    std::vector<Usage> usageTable;
};

#endif // MONTADOR_HPP
