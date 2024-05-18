PRE: pre_processamento.cpp pre_processamento.hpp
	g++ -std=c++11 -o montador pre_processamento.cpp
	./montador -p "Arquivos/$(ARQUIVO)"

PROC: montador.cpp montador.hpp
	g++ -std=c++11 -o montador montador.cpp
	./montador -o "Arquivos/$(ARQUIVO)"

clean:
	rm -f montador Arquivos/*.obj Arquivos/*.pre Arquivos/*.e
