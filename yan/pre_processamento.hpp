#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>

std::vector<std::string> VALOR_EQU;
std::vector<std::string> LABEL_EQU;

void LEITURA(std::string filename);

bool EIF(std::vector<std::string> linha);

bool IFS(std::vector<std::string> linha);

bool DEFINE_EQUS(std::vector<std::string> linha);

std::string ALTERA_EQUS(std::string word);

std::string CASO(std::string word);

bool COMENTARIO(std::string word);