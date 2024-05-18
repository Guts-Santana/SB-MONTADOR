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

/**
 * @brief Essa função é a responsável por escrever o arquivo.obj atraves 
 * de um vetor PROGRAMA que já tem os endereços dos opcodes e simbolos
 * 
 * @param filename essa função recebe o nome do arquivo como endereço
 */
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

/**
*@brief essa função procura definição de label e caso encontre,
*salva o endereço da label e retorna a linha sem a label.
*Caso ela encontre a definição de uma label já usada,
*Essa função irá escrever o endereço correto no programa
*@param parameter-linha Recebe a linha lida do arquivo pre
*@return retorna a linha lida, sem a definição da label
*/
std::vector<std::string> PROCURA_LABEL(std::vector<std::string> linha){
	if (linha[0].back() == ':'){
		std::string temp = linha[0];
		temp.pop_back();
		auto it = std::find(LABELS.begin(), LABELS.end(), temp);
		if (it != LABELS.end()){ //PROCURA SE A LABEL JÁ FOI USADA
			std::cout << "teste" << std::flush;
			auto it3 = std::find(LABEL_NODEF.begin(), LABEL_NODEF.end(), temp);
			if (it3 != LABEL_NODEF.end()){ // CONFERE SE JA FOI DEFINIDA OU SÓ USADA
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
			else{//CASO JA TENHA SIDO DEFINIDA ANTES, ERRO SEMANTICO
				it3 = std::find(LABEL_NODEF.begin(), LABEL_NODEF.end(), linha[0]);
				if (it3 == LABEL_NODEF.end())
					throw std::invalid_argument("Erro Semantico, LABEL REDEFINIDA");
			}

		}
		else if (linha[1] != "SPACE" && linha[1] != "CONST"){ //SALVA A LABEL E O ENDERECO DELA
			linha[0].pop_back();
			LABELS.push_back(linha[0]);
			LABEL_ENDERECO.push_back(std::to_string(PC));
			linha.erase(linha.begin());
		}
	}
	return linha;
}

/**
 * @brief essa funcao recebe a linha lida e conforme vai lendo os indices da linha,
 *  essa função vai escrevendo os valores de endereço e opcodes em um vetor
 * que será utilizado para escrever o arquivo.obj
 * @param linha a linha lida do arquivo.pre, sem definição de label.
 */
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

/**
 * @brief Essa função é a responsável por ler os simbolos e seus enderecos e
 * alterar os endereços temporarios escritos pelo programa com o algoritmo de 
 * passagem única e escrever os endereços reais dos simbolos durante o programa.
 * @param linha a linha lida do arquivo.pre, sem definição de label.
 */
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

/**
 * @brief Essa função é a responsável por separar os simbolos
 * usados pela instrução copy, que estão separados por uma vírgula.
 * @param linha a linha lida do arquivo.pre, sem definição de label.
 * @return retorna a linha, pós separar os símbolos.
 */
std::vector<std::string> INS_COPY(std::vector<std::string> linha){
	std::stringstream ss(linha[1]);
	std::string a, b;
	std::getline(ss, a, ',');
    std::getline(ss, b, ',');
	linha[1] = a;
	linha.push_back(b);
	return linha;
}