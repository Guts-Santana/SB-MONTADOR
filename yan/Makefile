# Definir o compilador
CXX = g++

# Definir as opções do compilador
CXXFLAGS = -Wall -std=c++11

# Nome do executável
EXEC = montador

# Arquivos de origem
SRCS = MONTADOR.cpp
HEADERS = MONTADOR.hpp

# Geração do executável
$(EXEC): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(SRCS)

# Limpeza dos arquivos gerados
clean:
	rm -f $(EXEC) *.o

PRE: pre_processamento.cpp pre_processamento.hpp
	g++ -std=c++11 -o montador pre_processamento.cpp
	./montador -p "Arquivos/$(ARQUIVO)"

PROC: montador.cpp montador.hpp
	g++ -std=c++11 -o montador montador.cpp
	./montador -o "Arquivos/$(ARQUIVO)"
