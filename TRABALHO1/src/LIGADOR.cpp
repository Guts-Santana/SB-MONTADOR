

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using UsageTable = std::map<std::string, std::vector<int>>;
using DefinitionTable = std::map<std::string, std::vector<int>>;
using RelocationTable = std::vector<int>;
using Code = std::vector<int>;

UsageTable usageTable1;
DefinitionTable definitionTable1;
RelocationTable relocationTable1;
Code code1;

UsageTable usageTable2;
DefinitionTable definitionTable2;
RelocationTable relocationTable2;
Code code2;

int correctionFactor;

void applyCorrectionFactor(Code &c, RelocationTable &rt);
void generateTables(std::ifstream &file, UsageTable &ut, DefinitionTable &dt, RelocationTable &rt, Code &c);

void linkFiles(std::string file1, std::string file2)
{
    std::ofstream outputStream(file1.substr(0, file1.size() - 2) + "_linked" + ".obj");

    // 10 9 1 20 11 30 7 10 14 5 13 19 13 5 13 19 5 8 10
    // 10 9 1 20 11 30 7 10 14 5 13 19 13 5 13 19 5 8 10

    std::ifstream file1Stream(file1);
    std::ifstream file2Stream(file2);

    std::string section;

    if (!file1Stream.is_open() || !file2Stream.is_open() || !outputStream.is_open())
    {
        std::cout << "Erro ao abrir os arquivos" << std::endl;
        return;
    }

    generateTables(file1Stream, usageTable1, definitionTable1, relocationTable1, code1);
    generateTables(file2Stream, usageTable2, definitionTable2, relocationTable2, code2);

    correctionFactor = code1.size();

    applyCorrectionFactor(code2, relocationTable2);

    for (auto &[key, value] : usageTable1)
    {
        for (auto &v : value)
        {
            if (relocationTable1[v - 1] == 1)
            {
                definitionTable2[key][0] += correctionFactor;
            }

            code1[v - 1] = definitionTable2[key][0];
        }
    }

    for (auto &[key, value] : usageTable2)
    {
        for (auto &v : value)
        {
            code2[v - 1] = definitionTable1[key][0];
        }
    }

    for (int i = 0; i < code1.size(); i++)
    {
        outputStream << code1[i] << " ";
    }

    for (int i = 0; i < code2.size(); i++)
    {
        outputStream << code2[i] << " ";
    }

    file1Stream.close();
    file2Stream.close();
    outputStream.close();
}

void generateTables(std::ifstream &file, UsageTable &ut, DefinitionTable &dt, RelocationTable &rt, Code &c)
{
    std::string line;
    std::string section;
    while (std::getline(file, line))
    {
        if (line == "USO")
        {
            section = "USO";
            continue;
        }
        else if (line == "DEF")
        {
            section = "DEF";
            continue;
        }
        else if (line == "REAL")
        {
            section = "REAL";
            continue;
        }
        else if (section == "REAL" && line == "" || line.empty() || line == " ")
        {
            section = "CODE";
            continue;
        }

        else if (line.empty())
            continue;

        if (section == "USO")
        {
            std::istringstream iss(line);
            std::string label;
            int value;
            iss >> label >> value;
            ut[label].push_back(static_cast<int>(value));
        }
        else if (section == "DEF")
        {
            std::istringstream iss(line);
            std::string label;
            int value;
            iss >> label >> value;
            dt[label].push_back(static_cast<int>(value));
        }
        else if (section == "REAL")
        {
            for (char ch : line)
            {
                if (isdigit(ch))
                {
                    rt.push_back(ch - '0');
                }
            }
        }
        else if (section == "CODE")
        {
            int value;
            std::istringstream iss(line);
            while (iss >> value)
            {
                c.push_back(static_cast<int>(value));
            }
        }
        else
        {
            std::cerr << "Invalid section" << std::endl;
        }
    }
}
void applyCorrectionFactor(Code &c, RelocationTable &rt)
{
    int count;
    for (int i = 0; i < c.size(); i++)
    {
        if (rt[i] == 1)
            c[i] += correctionFactor;
    }

    count = 0;
}