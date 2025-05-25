Autor: Henrique Kioshi Yamauchi

Descrição:
-----------
Este projeto consiste em dois programas:

1. bfc: Compilador de expressões aritméticas simples (ex: 3 + 4) para Brainfuck.
2. bfe: Executor de código Brainfuck gerado, retornando o valor final da célula de resultado.

Uso:
----
Compilação:
    make

Execução:
    echo "nome: 2 + 3" | ./bfc | ./bfe

Operações suportadas:
    +   soma
    -   subtração
    *   multiplicação
    /   divisão inteira (truncada)

Exemplos:
    echo "teste: 5 + 10" | ./bfc | ./bfe    → 15
    echo "calc: 6 * 7" | ./bfc | ./bfe      → 42
    echo "teste: 20 / 5" | ./bfc | ./bfe    → 4

Limitações:
-----------
- Apenas números inteiros positivos.
- A fita tem tamanho fixo de 1000 células.
- Apenas expressões com dois operandos e um operador.

Limpeza:
    make clean
