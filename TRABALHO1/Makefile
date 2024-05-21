PRE: pre_processamento.cpp pre_processamento.hpp
	g++ -std=c++11 -o montador pre_processamento.cpp
	./montador -p "Arquivos/$(ARQUIVO)"

PROC: montador.cpp montador.hpp
	g++ -std=c++11 -o montador montador.cpp
	./montador -o "Arquivos/$(ARQUIVO)"

LIG: LIGADOR.cpp LIGADOR.hpp
	g++ -std=c++17 -o LIGADOR LIGADOR.cpp
	./LIGADOR -l Arquivos/$(ARQUIVO1) Arquivos/$(ARQUIVO2)

clean:
	rm -f montador Arquivos/*.obj Arquivos/*.pre Arquivos/*.e
