#include "MONTADOR.hpp"
#include <algorithm>
#include <cctype>

Montador::Montador() : currentAddress(0), isBeginEnd(false)
{

    instructionSet = {
        {"ADD", 1}, {"SUB", 2}, {"MUL", 3}, {"DIV", 4}, {"JMP", 5}, {"JMPN", 6}, {"JMPP", 7}, {"JMPZ", 8}, {"COPY", 9}, {"LOAD", 10}, {"STORE", 11}, {"INPUT", 12}, {"OUTPUT", 13}, {"STOP", 14}};
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
    backpatch();
    writeObjectFile(outputFile);
}

void Montador::parseLine(const std::string &line)
{
    std::string trimmedLine = trim(line);

    if (trimmedLine.empty() || trimmedLine[0] == ';')
        return;

    auto tokens = split(trimmedLine, ' ');

    if (tokens[0].back() == ':')
    {
        std::string label = tokens[0].substr(0, tokens[0].length() - 1);
        if (symbolTable.find(label) != symbolTable.end() && symbolTable[label].defined)
        {
            std::cerr << "Error: Symbol redefined: " << label << std::endl;
            return;
        }
        symbolTable[label] = {currentAddress, false, false, true};
        tokens.erase(tokens.begin());
    }

    if (!tokens.empty())
    {
        if (tokens.size() > 1)
        {
            std::string directiveOrInstruction = tokens[0];
            std::string operand = tokens[1];

            if (isDirective(directiveOrInstruction))
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
            if (isDirective(tokens[0]))
            {
                handleDirective(tokens[0], "");
            }
            else
            {
                handleInstruction(tokens[0], "");
            }
        }
    }
}

bool Montador::isDirective(const std::string &token)
{
    std::string upperToken = token;
    std::transform(upperToken.begin(), upperToken.end(), upperToken.begin(), ::toupper);
    return upperToken == "BEGIN" || upperToken == "END" || upperToken == "EXTERN" || upperToken == "PUBLIC" || upperToken == "CONST" || upperToken == "SPACE";
}

void Montador::handleDirective(const std::string &directive, const std::string &operand)
{
    std::string upperDirective = directive;
    std::transform(upperDirective.begin(), upperDirective.end(), upperDirective.begin(), ::toupper);

    if (upperDirective == "BEGIN")
    {
        isBeginEnd = true;
    }
    else if (upperDirective == "END")
    {
    }
    else if (upperDirective == "EXTERN")
    {
        symbolTable[operand] = {0, true, false, false};
    }
    else if (upperDirective == "PUBLIC")
    {
        if (symbolTable.find(operand) != symbolTable.end())
        {
            symbolTable[operand].isPublic = true;
        }
        else
        {
            symbolTable[operand] = {currentAddress, false, true, false};
        }
    }
    else if (upperDirective == "CONST")
    {
        if (!operand.empty())
        {
            int value = std::stoi(operand);
            objectCode.push_back(toHex(value));
            relocationTable.push_back(0);
            currentAddress++;
        }
    }
    else if (upperDirective == "SPACE")
    {
        objectCode.push_back("00");
        relocationTable.push_back(0);
        currentAddress++;
    }
}

void Montador::handleInstruction(const std::string &instruction, const std::string &operands)
{
    std::string upperInstruction = instruction;
    std::transform(upperInstruction.begin(), upperInstruction.end(), upperInstruction.begin(), ::toupper);

    if (instructionSet.find(upperInstruction) == instructionSet.end())
    {
        std::cerr << "Error: Unknown instruction " << instruction << std::endl;
        return;
    }

    int opcode = instructionSet[upperInstruction];
    std::stringstream ss;

    ss << std::hex << std::setw(2) << std::setfill('0') << opcode;

    if (upperInstruction == "COPY")
    {
        auto operandsTokens = split(operands, ',');
        if (operandsTokens.size() != 2)
        {
            std::cerr << "Error: COPY instruction requires two operands." << std::endl;
            return;
        }

        for (size_t i = 0; i < operandsTokens.size(); ++i)
        {
            std::string operand = trim(operandsTokens[i]);

            if (symbolTable.find(operand) == symbolTable.end())
            {
                symbolTable[operand] = {0, false, false, false};
                symbolTable[operand].references.push_back(currentAddress + 1 + i);
            }
            else if (!symbolTable[operand].defined)
            {
                symbolTable[operand].references.push_back(currentAddress + 1 + i);
            }

            ss << " " << std::setw(2) << std::setfill('0') << symbolTable[operand].address;

            if (symbolTable[operand].isExtern)
            {
                updateUsageTable(operand, currentAddress + 1 + i);
            }
            relocationTable.push_back(1);
        }

        currentAddress += 3;
    }
    else
    {
        std::string operand = trim(operands);

        if (symbolTable.find(operand) == symbolTable.end())
        {
            symbolTable[operand] = {0, false, false, false};
            symbolTable[operand].references.push_back(currentAddress + 1);
        }
        else if (!symbolTable[operand].defined)
        {
            symbolTable[operand].references.push_back(currentAddress + 1);
        }

        ss << " " << std::setw(2) << std::setfill('0') << symbolTable[operand].address;

        if (symbolTable[operand].isExtern)
        {
            updateUsageTable(operand, currentAddress + 1);
        }
        relocationTable.push_back(0);
        relocationTable.push_back(1);

        currentAddress += 2;
    }

    objectCode.push_back(ss.str());
}

void Montador::updateUsageTable(const std::string &symbol, int address)
{
    usageTable.emplace_back(Usage{symbol, address});
}

void Montador::backpatch()
{
    for (auto &entry : symbolTable)
    {
        for (int addr : entry.second.references)
        {
            std::stringstream ss;
            ss << std::hex << std::setw(2) << std::setfill('0') << entry.second.address;
            int instructionIndex = addr / 2;
            std::string updatedInstruction = objectCode[instructionIndex].substr(0, 3) + ss.str();
            objectCode[instructionIndex] = updatedInstruction;
        }
    }
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
            outfile << entry.symbol << " " << entry.address << "\n";
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
        for (const auto &entry : relocationTable)
        {
            outfile << entry;
        }
        outfile << "\n";
    }

    for (const auto &code : objectCode)
    {
        outfile << code << " ";
    }

    outfile.close();
}

void Montador::writePendingList(const std::string &outputFile)
{
    std::ofstream outfile(outputFile, std::ios_base::app);

    if (!outfile)
    {
        std::cerr << "Error: Cannot open file " << outputFile << std::endl;
        return;
    }

    outfile << "PENDING REFERENCES\n";
    for (const auto &entry : symbolTable)
    {
        if (!entry.second.defined && !entry.second.references.empty())
        {
            outfile << entry.first << ": ";
            for (const int &ref : entry.second.references)
            {
                outfile << ref << " ";
            }
            outfile << "\n";
        }
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

std::string Montador::toHex(int num)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << num;
    return ss.str();
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " -o <inputfile.asm> or " << argv[0] << " -p <inputfile.asm>" << std::endl;
        return 1;
    }

    std::string option = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = inputFile.substr(0, inputFile.find_last_of('.')) + ".obj";

    Montador montador;

    if (option == "-o")
    {
        montador.assemble(inputFile, outputFile);
    }
    else if (option == "-p")
    {
        montador.assemble(inputFile, outputFile);
        montador.writePendingList(outputFile);
    }
    else
    {
        std::cerr << "Unknown option: " << option << std::endl;
        return 1;
    }

    return 0;
}