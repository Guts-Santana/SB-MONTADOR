PRE: pre_processamento.cpp pre_processamento.hpp
	g++ -std=c++11 -o montador pre_processamento.cpp
	./montador -p $(ARQUIVO)

PROC: montador.cpp montador.hpp
	g++ -std=c++11 -o montador montador.cpp
	./montador -o $(ARQUIVO)

clean:
	rm -rf montador *.o *.exe *.gc* *.pre