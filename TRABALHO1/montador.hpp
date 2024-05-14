#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <stdexcept>

int contador_linha = 1;
int PC = 0;
std::vector<std::string> LABELS;
std::vector<std::string> LABEL_ENDERECO;
std::vector<std::string> PROGRAMA;
std::vector<std::string> SIMBOLOS;
std::vector<std::string> SIMBOLOS_ENDERECO;
std::vector<std::string> OPCODES {"ADD", "SUB", "MUL", "DIV","JMP", 
"JMPN", "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"};
std::vector<std::string> VALOR_OPCODES {"1", "2", "3", "4", "5", "6", "7",
"8", "9", "10", "11", "12", "13", "14"};


void LEITURA(std::string filename);

void ESCRITA(std::string filename);

void CASO_BASE(std::vector<std::string> linha);

std::vector<std::string> PROCURA_LABEL(std::vector<std::string> linha);

void ESCREVE_PROGRAMA(std::vector<std::string> linha);

void ESCREVE_SIMBOLOS(std::vector<std::string> linha);