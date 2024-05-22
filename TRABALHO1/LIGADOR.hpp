#ifndef LIGADOR_HPP
#define LIGADOR_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

class Module
{
public:
    std::map<std::string, std::vector<int>> uso;
    std::map<std::string, int> defs;
    std::string real;
    std::vector<int> code;
};

class Linker
{
public:
    Module readModule(const std::string &filename);
    std::vector<int> linkModules(Module &modA, Module &modB);
    void writeLinkedModule(const std::vector<int> &linkedCode, const std::string &filename);
};

#endif
