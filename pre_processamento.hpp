#include <iostream>
#include <string>
#include <vector>

void LEITURA(std::string filename);

bool EIF(std::vector<std::string> linha);

bool IFS(std::vector<std::string> linha);

void DEFINE_EQUS(std::vector<std::string> linha);

std::string ALTERA_EQUS(std::string word);

std::string CASO(std::string word);

int COMENTARIO(std::string word);