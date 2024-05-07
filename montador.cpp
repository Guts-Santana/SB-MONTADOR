#include "montador.hpp"

int contador_linha = 1;
std::vector<std::string> LABELS;
std::vector<std::string> PROGRAMA;

int main(int argc, char *argv[]){
	if (argc != 3){
		std::cerr << "Erro ao abrir arquivo";
		return 1;
	}
	std::string filename = argv[2];
	LEITURA(filename);

	return 0;
}

void LEITURA(std::string filename){
	std::string line;
	std::string word;
	std::vector<std::string> linha;

	std::ifstream file(filename);
	if (!file.is_open()){
		std::cerr << "Arquivo PRE não encontrado" << std::endl;
	}
	while (getline(file, line)){
		std::istringstream iss(line);
		linha.clear();
		while (iss >> word){
			linha.push_back(word);
			//std::cout << word << std::endl;
		}

	}
}
