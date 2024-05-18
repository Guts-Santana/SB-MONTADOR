#include "MONTADOR.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>

Montador::Montador() : currentAddress(0), isBeginEnd(false)
{
    // Mapa de instruções para opcode (simulação)
    instructionSet = {
        {"INPUT", 12}, {"LOAD", 10}, {"DIV", 4}, {"STORE", 11}, {"MUL", 3}, {"SUB", 2}, {"OUTPUT", 13}, {"COPY", 9}, {"JMPP", 7}, {"JMP", 5}, {"CONST", -1}, {"SPACE", -1}};
}

void Montador::assemble(const std::string &inputFile, const std::string &outputFile)
{
    std::ifstream infile(inputFile);
    std::string line;

    if (!infile)
    {
        std::cerr << "Error: Cannot open file " << inputFile << std::endl;
        return;
    }

    while (std::getline(infile, line))
    {
        parseLine(line);
    }

    infile.close();
    writeObjectFile(outputFile);
}

void Montador::parseLine(const std::string &line)
{
    std::string trimmedLine = trim(line);

    // Ignore empty lines and comments
    if (trimmedLine.empty() || trimmedLine[0] == ';')
        return;

    auto tokens = split(trimmedLine, ' ');

    if (tokens.size() > 1)
    {
        std::string directiveOrInstruction = tokens[0];
        std::string operand = tokens[1];

        if (directiveOrInstruction == "BEGIN" || directiveOrInstruction == "END" ||
            directiveOrInstruction == "EXTERN" || directiveOrInstruction == "PUBLIC")
        {
            handleDirective(directiveOrInstruction, operand);
        }
        else
        {
            handleInstruction(directiveOrInstruction, operand);
        }
    }
    else
    {
        handleInstruction(tokens[0], "");
    }
}

void Montador::handleDirective(const std::string &directive, const std::string &operand)
{
    if (directive == "BEGIN")
    {
        isBeginEnd = true;
    }
    else if (directive == "END")
    {
        // No special action needed
    }
    else if (directive == "EXTERN")
    {
        symbolTable[operand] = {0, true, false};
    }
    else if (directive == "PUBLIC")
    {
        if (symbolTable.find(operand) != symbolTable.end())
        {
            symbolTable[operand].isPublic = true;
        }
        else
        {
            symbolTable[operand] = {currentAddress, false, true};
        }
    }
}

void Montador::handleInstruction(const std::string &instruction, const std::string &operand)
{
    if (instructionSet.find(instruction) == instructionSet.end())
    {
        std::cerr << "Error: Unknown instruction " << instruction << std::endl;
        return;
    }

    int opcode = instructionSet[instruction];
    if (opcode != -1)
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << opcode;
        if (!operand.empty() && symbolTable.find(operand) != symbolTable.end())
        {
            ss << " " << std::setw(2) << std::setfill('0') << symbolTable[operand].address;
            if (symbolTable[operand].isExtern)
            {
                updateUsageTable(operand, currentAddress);
            }
        }
        else
        {
            ss << " 00";
        }
        objectCode.push_back(ss.str());
    }
    else if (instruction == "CONST")
    {
        objectCode.push_back(operand);
    }
    else if (instruction == "SPACE")
    {
        objectCode.push_back("00");
    }

    if (!operand.empty() && symbolTable.find(operand) == symbolTable.end())
    {
        symbolTable[operand] = {currentAddress, false, false};
    }

    currentAddress++;
}

void Montador::updateUsageTable(const std::string &symbol, int address)
{
    usageTable.emplace_back(symbol, address);
}

void Montador::writeObjectFile(const std::string &outputFile)
{
    std::ofstream outfile(outputFile);

    if (!outfile)
    {
        std::cerr << "Error: Cannot open file " << outputFile << std::endl;
        return;
    }

    if (isBeginEnd)
    {
        outfile << "USO\n";
        for (const auto &entry : usageTable)
        {
            outfile << entry.first << " " << entry.second << "\n";
        }

        outfile << "DEF\n";
        for (const auto &entry : symbolTable)
        {
            if (entry.second.isPublic)
            {
                outfile << entry.first << " " << entry.second.address << "\n";
            }
        }

        outfile << "REAL\n";
    }

    for (const auto &code : objectCode)
    {
        outfile << code << " ";
    }

    outfile.close();
}

std::vector<std::string> Montador::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(trim(token));
    }
    return tokens;
}

std::string Montador::trim(const std::string &s)
{
    auto start = s.find_first_not_of(" \t\n\r");
    auto end = s.find_last_not_of(" \t\n\r");
    return (start == std::string::npos || end == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " -o <inputfile.asm>" << std::endl;
        return 1;
    }

    std::string option = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = inputFile.substr(0, inputFile.find_last_of('.')) + ".obj";

    if (option == "-o")
    {
        Montador montador;
        montador.assemble(inputFile, outputFile);
    }
    else
    {
        std::cerr << "Unknown option: " << option << std::endl;
        return 1;
    }

    return 0;
}
