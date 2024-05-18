# SB-MONTADOR

Alunos:
	Gustavo Santana Lima - 211038235
	Yan Tavares de Oliveira - 202014323

O projeto consiste em codificar um montador de um assembly simplificado,
do pré-processamento até a parte do ligador, de modo a ser possível, a partir
de um arquivo assembly, rodar o programa e o arquivo assembly ser simplificado
para código máquina e a partir desse ponto ser simulado. 


## Índice

- [Instalação](#instalação)
- [Uso](#uso)
- [Contato](#contato)

## Instalação

Para fazer a instalação, basta clonar o repositorio do github
com o comando git clone https://github.com/Guts-Santana/SB-MONTADOR


## Uso

Foi criado um makefile para simplificação dos comandos.
Escolhido o arquivo assembly na pasta Arquivos, os comandos serão os seguintes:

make PRE ARQUIVO=arquivo_escolhido.asm"
Alterar "arquivo_escolhido" pelo nome do arquivo assembly
Assim retornará um arquivo com o nome "arquivo_escolhido.pre" que já deverá ter
retirado todas as diretivas de pré-processamento, após isso,

make PROC ARQUIVO=arquivo_escolhido.pre
Alterar "arquivo_escolhido" pelo nome do arquivo assembly
Assim retornará um arquivo com o nome "arquivo_escolhido.obj" que já deverá ter
sido compilado e caso não tenha erro e o arquivo não tiver módulos, esse arquivo
já poderá passar pelo simulador com o comando

./simulador arquivo_escohido.obj
Alterar "arquivo_escolhido" pelo nome do arquivo assembly
E assim seu programa será simulado no terminal.

caso o arquivo obj seja separado em módulos, ele deverá passar pelo ligador
atráves do comando:



## Contato

Se você quiser entrar em contato, envie um e-mail para [guts.santana@gmail.com](mailto:guts.santana@gmail.com).
