#include "montador.hpp"

int main(int argc, char *argv[]){
	if (argc != 3){
		std::cerr << "Erro ao abrir arquivo";
		return 1;
	}
	std::string filename = argv[2];
	try{
		LEITURA(filename);
	}
	catch (const std::invalid_argument& e){
		std::cerr << e.what() << '\n';
		std::cerr << "Na linha: " << contador_linha << '\n';
	}
	ESCRITA(filename);

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
		linha = PROCURA_LABEL(linha);
		if (linha[0] == "COPY") {
			linha = INS_COPY(linha);
		}
		ESCREVE_PROGRAMA(linha);
		if (linha.size() > 1 || (linha.size() == 1 && linha[0].back() != ':'))
			if (linha.size() != 1 && linha[0].back() != ':')
				PC+=linha.size();
			else
				PC++;
		//std::cout << PC << '\n';

		contador_linha++;
	}
}

void ESCRITA(std::string filename){
	filename.pop_back();
	filename.pop_back();
	filename.pop_back();
	filename = filename + "obj";
	std::ofstream arquivo(filename);
	if (arquivo.is_open()){
		for (std::string i : PROGRAMA){
			std::cout << i << ' ';
			arquivo << i << ' ';
		}
	}
	arquivo.close();
}


std::vector<std::string> PROCURA_LABEL(std::vector<std::string> linha){
	if (linha[0].back() == ':'){
		std::string temp = linha[0];
		temp.pop_back();
		auto it = std::find(LABELS.begin(), LABELS.end(), temp);
		if (it != LABELS.end()){
			std::cout << "teste" << std::flush;
			auto it3 = std::find(LABEL_NODEF.begin(), LABEL_NODEF.end(), temp);
			if (it3 != LABEL_NODEF.end()){
				std::cout << "teste" << std::flush;
				int posicao = std::distance(LABELS.begin(), it);
				PROGRAMA[std::stoi(LABEL_ENDERECO[posicao])] = std::to_string(PC);
				std::cout << PROGRAMA[std::stoi(LABEL_ENDERECO[posicao])] << '\n';
				LABEL_ENDERECO[posicao] = PC;
				posicao = std::distance(LABEL_NODEF.begin(), it3);
				LABEL_NODEF[posicao] = "0";
				linha.erase(linha.begin());
				return linha;
			}
			else{
				it3 = std::find(LABEL_NODEF.begin(), LABEL_NODEF.end(), linha[0]);
				if (it3 == LABEL_NODEF.end())
					throw std::invalid_argument("Erro Semantico, LABEL REDEFINIDA");
			}

		}
		else if (linha[1] != "SPACE" && linha[1] != "CONST"){
			linha[0].pop_back();
			LABELS.push_back(linha[0]);
			LABEL_ENDERECO.push_back(std::to_string(PC));
			linha.erase(linha.begin());
		}
	}
	return linha;
}

void ESCREVE_PROGRAMA(std::vector<std::string> linha){
	int posicao;
	for (int i = 0; i < linha.size(); i++){
		//TESTA SE É OPCODE
		auto it1 = std::find(OPCODES.begin(), OPCODES.end(), linha[i]);
		if (it1 != OPCODES.end()) {
			posicao = std::distance(OPCODES.begin(), it1);
			PROGRAMA.push_back(VALOR_OPCODES[posicao]);
			continue;
		}
		//TESTA SE É LABEL
		it1 = std::find(LABELS.begin(), LABELS.end(), linha[i]);
		if (it1 != LABELS.end()){
			posicao = std::distance(LABELS.begin(), it1);
			PROGRAMA.push_back(LABEL_ENDERECO[posicao]);
			continue;
		}
		//TESTA SE É SIMBOLO
		it1 = std::find(SIMBOLOS.begin(), SIMBOLOS.end(), linha[i]);
		if (it1 != SIMBOLOS.end()){
			posicao = std::distance(SIMBOLOS.begin(), it1);
			PROGRAMA.push_back(SIMBOLOS_ENDERECO[posicao]);
			SIMBOLOS_ENDERECO[posicao] = (std::to_string(PC+i));
			continue;
		}
		 
		if (linha[0].back() == ':'){
			ESCREVE_SIMBOLOS(linha);
			break;
		}
		else{
			if (linha[0] != "JMP" && linha[0] != "JMPP" &&
			linha[0] != "JMPZ" && linha[0] != "JMPN" ){
				SIMBOLOS.push_back(linha[i]);
				SIMBOLOS_ENDERECO.push_back(std::to_string(PC+i));
				PROGRAMA.push_back("-1");
			}
			else{
				LABELS.push_back(linha[i]);
				LABEL_ENDERECO.push_back(std::to_string(PC+i));
				LABEL_NODEF.push_back(linha[i]);
				PROGRAMA.push_back("-1");
			}
		}
	}
}

void ESCREVE_SIMBOLOS(std::vector<std::string> linha){
	linha[0].pop_back();
	if (linha.size() != 2 && linha.size() != 3)
		throw std::invalid_argument("Erro sintatico");
	int pos;
	int temp;
	auto it2 = std::find(SIMBOLOS.begin(), SIMBOLOS.end(), linha[0]);
	if (it2 != SIMBOLOS.end()){
		int posicao = std::distance(SIMBOLOS.begin(), it2);
		if (linha[1] == "CONST"){
			//if (linha.size() != 3)
			//	throw std::invalid_argument("Erro sintatico");
			PROGRAMA.push_back(linha[2]);
			
		}
		else if (linha[1] == "SPACE"){
			//if (linha.size() != 2)
			//	throw std::invalid_argument("Erro sintatico");
			PROGRAMA.push_back("0");
		}
		else{
			throw std::invalid_argument("Erro");
		}
		pos = std::stoi(SIMBOLOS_ENDERECO[posicao]);
		while (PROGRAMA[pos] != "-1"){
			temp = std::stoi(PROGRAMA[pos]);
			PROGRAMA[pos] = std::to_string(PC);
			pos = (int)temp;
		}
		PROGRAMA[pos] = std::to_string(PC);
	}
	//TLVZ ME PREOCUPAR COM DEFINICAO DE SIMBOLO NO COMECO
}

std::vector<std::string> INS_COPY(std::vector<std::string> linha){
	std::stringstream ss(linha[1]);
	std::string a, b;
	std::getline(ss, a, ',');
    std::getline(ss, b, ',');
	linha[1] = a;
	linha.push_back(b);
	return linha;
}