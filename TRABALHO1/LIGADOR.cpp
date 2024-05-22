#include "LIGADOR.hpp"

Module Linker::readModule(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    Module module;
    std::string section;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (line == "USO")
        {
            section = "uso";
        }
        else if (line == "DEF")
        {
            section = "def";
        }
        else if (line == "REAL")
        {
            section = "real";
        }
        else if (section == "uso")
        {
            std::istringstream iss(line);
            std::string symbol;
            int addr;
            iss >> symbol;
            while (iss >> addr)
            {
                module.uso[symbol].push_back(addr);
            }
        }
        else if (section == "def")
        {
            std::istringstream iss(line);
            std::string symbol;
            int addr;
            iss >> symbol >> addr;
            module.defs[symbol] = addr;
        }
        else if (section == "real")
        {
            module.real = line;
            section = "code";
        }
        else if (section == "code")
        {
            std::istringstream iss(line);
            int instr;
            while (iss >> instr)
            {
                module.code.push_back(instr);
            }
        }
    }

    file.close();
    return module;
}

std::vector<int> Linker::linkModules(Module &modA, Module &modB)
{
    int correctionB = modA.code.size();

    std::map<std::string, int> globalDefs = modA.defs;
    for (const auto &[symbol, addr] : modB.defs)
    {
        globalDefs[symbol] = addr + correctionB;
    }

    for (const auto &[symbol, addrs] : modA.uso)
    {
        for (int addr : addrs)
        {
            modA.code[addr] += globalDefs[symbol];
        }
    }

    for (const auto &[symbol, addrs] : modB.uso)
    {
        for (int addr : addrs)
        {
            modB.code[addr] += globalDefs[symbol];
        }
    }

    for (int &instr : modB.code)
    {
        if (instr != 0)
        {
            instr += correctionB;
        }
    }

    std::vector<int> linkedCode = modA.code;
    linkedCode.insert(linkedCode.end(), modB.code.begin(), modB.code.end());

    return linkedCode;
}

void Linker::writeLinkedModule(const std::vector<int> &linkedCode, const std::string &filename)
{
    std::ofstream outfile(filename);
    for (int instr : linkedCode)
    {
        outfile << instr << " ";
    }
    outfile << std::endl;
    outfile.close();
}
