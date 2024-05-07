#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>

#include "pre_processamento.hpp"

std::vector<std::string> VALOR_EQU;
std::vector<std::string> LABEL_EQU;
int main(int argc, char *argv[]){

	if (argc != 3){
		std::cerr << "Erro ao abrir arquivo";
		return 1;
	}
	std::string filename = argv[2];

	std::ifstream file(filename);
	if (!file.is_open()){
		std::cerr << "Arquivo Assembly não encontrado" << std::endl;
		return 1;
	}
	LEITURA(filename);

	return 0;
}

void LEITURA(std::string filename){
	std::string line;
	std::string word;
	std::vector<std::string> linha;

	std::ifstream file(filename);
	if (!file.is_open()){
		std::cerr << "Arquivo Assembly não encontrado" << std::endl;
	}
	filename.pop_back();
	filename.pop_back();
	filename.pop_back();
	filename = filename + "pre";
	std::ofstream arquivo(filename);

	while (getline(file, line)){
		std::istringstream iss(line);
		linha.clear();
		while (iss >> word){
			word = CASO(word);
			if (COMENTARIO(word))
				break;
			word = ALTERA_EQUS(word);
			linha.push_back(word);
			//std::cout << word << std::endl;
		}
		if (linha.size()>=2){
			if (EIF(linha)){
				if (IFS(linha))
					std::getline(file,line);
				linha.clear();
			}
		}
		if (linha.size()>=3){
			if(DEFINE_EQUS(linha))
				linha.clear();
		}

		if (arquivo.is_open()){
			for (int i = 0; i < (int)linha.size(); i++){
				std::cout << linha[i] << " ";
				arquivo << linha[i] << " ";
			}
			if (linha.size() != 0){
				std::cout << "\n";
				arquivo << "\n";
			}
			
		}
	}
	arquivo.close();
}	
bool EIF(std::vector<std::string> linha){
	if (linha[0] == "IF"){
		return true;
	}
	return false;
	
}
bool IFS(std::vector<std::string> linha){	
	//RETORNA UMA FLAG CASO IF 0;
	if (linha[0] == "IF"){
		if (linha[1] == "0")
			return true;
	}
	return false;
}

bool DEFINE_EQUS(std::vector<std::string> linha){
	if (linha[1] == "EQU"){
		linha[0].pop_back();
		LABEL_EQU.push_back(linha[0]);
		VALOR_EQU.push_back(linha[2]);
		return true;
	}
	return false;
}

std::string ALTERA_EQUS(std::string word){
	auto it = std::find(LABEL_EQU.begin(), LABEL_EQU.end(), word);
	if (it != LABEL_EQU.end()){
    	int posicao = std::distance(LABEL_EQU.begin(), it);
		word = VALOR_EQU[posicao];
	}
	return word;
}

std::string CASO(std::string word){
	std::transform(word.begin(), word.end(), word.begin(),
        [](unsigned char c){ return std::toupper(c); });
	return word;
}

int COMENTARIO(std::string word){
	if (word[0]==';')
		return 1;
	return 0;
}