

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using UsageTable = std::map<std::string, std::vector<int>>;
using DefinitionTable = std::map<std::string, std::vector<int>>;
using RelocationTable = std::vector<int>;
using Code = std::vector<int>;

std::ofstream outputStream("linkedFile.obj");

UsageTable usageTable1;
DefinitionTable definitionTable1;
RelocationTable relocationTable1;
Code code1;

UsageTable usageTable2;
DefinitionTable definitionTable2;
RelocationTable relocationTable2;
Code code2;

void linkFiles(std::string file1, std::string file2)
{
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
        else if (section == "REAL" && line == "")
        {
            break;
        }
        if (line.empty())
            continue;
        if (section == "USO")
        {
            std::istringstream iss(line);
            std::string label;
            int value;
            iss >> label >> value;
            ut[label].push_back(value);
        }
        else if (section == "DEF")
        {
            std::istringstream iss(line);
            std::string label;
            int value;
            iss >> label >> value;
            dt[label].push_back(value);
        }
        else if (section == "REAL")
        {
            int value;
            std::istringstream iss(line);
            iss >> value;
            rt.push_back(value);
        }
        else if (section == "CODE")
        {
            int value;
            std::istringstream iss(line);
            iss >> value;
            c.push_back(value);
        }
    }
}