PRE: pre_processamento.cpp pre_processamento.hpp
	g++ -std=c++17 -o montador pre_processamento.cpp
	./montador -p "Arquivos/$(ARQUIVO)"

PROC: MONTADOR.cpp MONTADOR.hpp
	g++ -std=c++17 -o MONTADOR MONTADOR.cpp
	./MONTADOR -o "Arquivos/$(ARQUIVO)"

LIG: LIGADOR.cpp LIGADOR.hpp
	g++ -std=c++17 -o LIGADOR LIGADOR.cpp
	./LIGADOR -l Arquivos/$(ARQUIVO1) Arquivos/$(ARQUIVO2)

clean:
	rm -f montador Arquivos/*.obj Arquivos/*.pre Arquivos/*.e
