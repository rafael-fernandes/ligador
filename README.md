## Trabalho 2 de Software Básico - 1º/2018

**Aluno:** Rafael Alves Fernandes

**Matrícula:** 14/00030395

---

#### Problema

Modificar o Macro-Assembler em C/C++ para a linguagem Assembly hipotética vista em sala de aula para aceitar módulos e fazer a correta ligação dos mesmos.

---

#### Objetivo

Fixar o funcionamento de um processo de tradução.

---

#### Desenvolvimento

O programa foi feito em C++ 11, no MacOS High Sierra 10.13.4.

`gcc --version`

```
Configured with: --prefix=/Library/Developer/CommandLineTools/usr --with-gxx-include-dir=/usr/include/c++/4.2.1
Apple LLVM version 9.1.0 (clang-902.0.39.1)
Target: x86_64-apple-darwin17.5.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
```

---

#### Estrutura de Arquivos e Pastas

`bin`: contém os arquivos executáveis, após compilação

`include`: contém os headers

`obj`: contém os códigos objeto do programa, após compilação

`processed`: conterá os arquivos de saída do pré-processamento, processamento de macros e montagem

`src`: contém o código fonte do programa

`tmp`: arquivos temporários das etapas de processamento de macros: `MDT.txt` e `MNT.txt`

`main.cpp`: arquivo principal do programa

---

#### Compilação e execução

Certifique-se de excluir arquivos gerados por outras compilações e execuções:

```
make clean
```

Para compilar o programa, execute:

```
make
```

O programa pode aceitar até dois programas de entrada:

```
./bin/main prog1 prog2
```

Caso haja 2 programas de entrada, a ligação será feita automaticamente e a saída estará em `processed/object.o`.

Lembre-se que o nome do arquivo de entrada não deve possuir extensão.

---

### Calculadora

Para montar, utilize:
```
nasm -f  elf calculadora.asm -o calculadora.o
```

Para ligar, utilize:
```
ld -m elf_i386 -o calculadora calculadora.o
```

Para executar, utilize:
```
./calculadora
```
---

#### Testes

O programa foi testado no Ubuntu 18.04 LTS com o compilador GCC 7.3.