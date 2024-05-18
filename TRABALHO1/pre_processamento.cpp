#include "pre_processamento.hpp"

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

/*
Essa função ocorre a leitura do arquivo assembly e 
chama as funções de pré-processamento
*/
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
		if (EIF(linha)){
			if (IFS(linha))
				std::getline(file,line);
			linha.clear();
		}
		if(DEFINE_EQUS(linha))
			linha.clear();

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

/**
 * @brief Confere se a linha é uma declaração de if
 * Se for, returna true, e entra na função IFS
 * @param linha a linha lida do arquivo.asm
 * @return true caso seja um IF
 * @return false caso nao seja IF
 */
bool EIF(std::vector<std::string> linha){
	if (linha.size() == 2){
		if (linha[0] == "IF")
			return true;
		}
	if (linha.size() >= 2){
		if (linha[0] == "IF" || linha[1] == "IF")
			return true;
	}
	return false;
}

/**
 * @brief analisa se a linha lida é um if ou nao,
 * caso a linha lida SEJA UM IF, ANALISA SE O VALOR DO IF É 1 OU 0
 * @param linha a linha lida do arquivo.asm
 * @return true caso o if seja 0 e tenha que apagar a proxima linha
 * @return false caso o if seja verdadeiro e a proxima linha deve ser lida
 */
bool IFS(std::vector<std::string> linha){	
	//RETORNA UMA FLAG CASO IF 0;
	if (linha.size() == 2){
		if (linha[1] == "0")
			return true;
	}
	if (linha.size() > 2){
		if (linha[1] == "0" || linha[2] == "0")
			return true;
	}
	return false;
}

/**
 * @brief A funcao confere se a linha descrita é uma definição de EQU
 * Se for, adiciona a label e o valor dela em vetores correspondentes
 * @param linha a linha lida do arquivo.asm
 * @return true caso seja definicao de EQU e deva apagar a proxima linha
 * @return false caso nao seja definicao de EQU[
 * 
 */
bool DEFINE_EQUS(std::vector<std::string> linha){
	if (linha.size()>=3){
		if (linha[1] == "EQU"){
			linha[0].pop_back();
			LABEL_EQU.push_back(linha[0]);
			VALOR_EQU.push_back(linha[2]);
			return true;
		}
	}
	return false;
}

/**
 * @brief confere se a linha lida é usada o EQU definido,
 * caso seja, essa função é responsavel por alterar a EQU
 * @param linha a linha lida do arquivo.asm
 * @return retorna a linha com a alteração do EQU
 */
std::string ALTERA_EQUS(std::string word){
	auto it = std::find(LABEL_EQU.begin(), LABEL_EQU.end(), word);
	if (it != LABEL_EQU.end()){
    	int posicao = std::distance(LABEL_EQU.begin(), it);
		word = VALOR_EQU[posicao];
	}
	return word;
}

/**
 * @brief Essa função altera as palavras lidas para maiusculas
 * Para que assim o código nao seja sensivel ao caso de letras 
 * maiuscula ou minusculas
 * 
 * @param word recebe palavra a palavra lida pelo arquivo.asm
 * @return retorna a word no formato maiusculo
 */
std::string CASO(std::string word){
	std::transform(word.begin(), word.end(), word.begin(),
        [](unsigned char c){ return std::toupper(c); });
	return word;
}

/*
Função que remove comentario
Se TRUE a linha inteira é ignorada
*/
bool COMENTARIO(std::string word){
	if (word[0]==';')
		return true;
	return false;
}