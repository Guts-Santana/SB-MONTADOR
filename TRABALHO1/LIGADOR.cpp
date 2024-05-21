#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

struct Module
{
    map<string, vector<int>> uso;
    map<string, int> defs;
    string real;
    vector<int> code;
};

Module readModule(const string &filename)
{
    ifstream file(filename);
    string line;
    Module module;
    string section;

    while (getline(file, line))
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
            istringstream iss(line);
            string symbol;
            int addr;
            iss >> symbol;
            while (iss >> addr)
            {
                module.uso[symbol].push_back(addr);
            }
        }
        else if (section == "def")
        {
            istringstream iss(line);
            string symbol;
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
            istringstream iss(line);
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

vector<int> linkModules(Module &modA, Module &modB)
{
    int correctionB = modA.code.size();

    map<string, int> globalDefs = modA.defs;
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

    vector<int> linkedCode = modA.code;
    linkedCode.insert(linkedCode.end(), modB.code.begin(), modB.code.end());

    return linkedCode;
}

void writeLinkedModule(const vector<int> &linkedCode, const string &filename)
{
    ofstream outfile(filename);
    for (int instr : linkedCode)
    {
        outfile << instr << " ";
    }
    outfile << endl;
    outfile.close();
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <MOD_A.obj> <MOD_B.obj>" << endl;
        return 1;
    }

    string modAFile = argv[1];
    string modBFile = argv[2];

    Module modA = readModule(modAFile);
    Module modB = readModule(modBFile);

    vector<int> linkedCode = linkModules(modA, modB);

    writeLinkedModule(linkedCode, "EXEC.obj");

    cout << "Modules linked successfully. Output written to EXEC.obj" << endl;

    return 0;
}
