# Trabalho 1 - Montador e Ligador de uma linguagem assembly inventada

## Autores

Yan Tavares - 202014323
Gustavo Lima

## Como compilar e executar o montador e ligador

### Opção 1 - Usando `make` para compilar os arquivos e executar manualmente:

```bash
make
```

Isso irá criar o arquivo executável `montador` na pasta `bin`. Com ele, é possível montar e ligar arquivos `.asm` e `.obj`:

1. Para visualizar todas as opções disponíveis, execute:

```bash
./bin/montador --help
```

ou

```bash
./bin/montador -h
```

2. Para **apenas** pré-processar um arquivo `.asm` e gerar um arquivo `.pre`, execute:

```bash
./bin/montador -p <arquivo.asm>

```

3. Para montar um arquivo `.asm` e gerar um arquivo `.obj` (ou `.e`, no caso de arquivos ligáveis), execute:

```bash
./bin/montador -o <arquivo.asm>
```

4. Para ligar dois arquivos `.e` e gerar um arquivo `.obj` resultante, execute:

```bash
./bin/montador -l <arquivo1.e> <arquivo2.e>
```

O arquivo `obj` terá o nome do primeiro arquivo passado como argumento + `_linked.obj`

### Opção 2 - Usando `make` para compilar e executar os arquivos automaticamente:

1. Para compilar o montador, gerar o arquivo `.pre` e montar um arquivo `.asm`, execute:

```bash
make as file=<arquivo.asm>
```

2. Para compilar o montador, gerar o arquivo `.pre`, montar o arquivo `.e` e ligar dois arquivos, gerando o `obj` resultante, execute:

```bash
make ld file1=<arquivo1.asm> file2=<arquivo2.asm>
```

O arquivo `obj` terá o nome do primeiro arquivo passado como argumento + `_linked.obj`

3. Para limpar os arquivos `.o` gerados, execute:

```bash
make clean
```

## Observações

- Os arquivos gerados pelo montador são salvos na mesma pasta do arquivo de entrada.
- O montador e ligador foram testados nos ambientes (Ubuntu 20.04, POP_OS 22.04 e WSL no Windows 11).
- Os arquivos utilizados para teste podem ser encontrados na pasta `Arquivos/`.
- Diretiva `CONST` aceita números decimais e hexadecimais.

## Estrutura do projeto

```
TRABALHO1
│ README.md
│ makefile
│ main.cpp
│
├── bin
│ │ montador
│ │ simulador
│
├── src
│ │ main.cpp
│ │ LIGADOR.cpp
│ │ MONTADOR.cpp
│ │ PRE_PROCESSADOR.cpp
│
├── include
│ │ LIGADOR.hpp
│ │ MONTADOR.hpp
│ │ PRE_PROCESSADOR.hpp
│
├── docs
│ │ documentation.md
│
├── Arquivos
│ │ arquivo1.asm
│ │ ...
│
```
