#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void generate_bf_code(int a, int b, char op, char *bf_code) {
    bf_code[0] = '\0';
    char buffer[500];

    switch (op) {
        case '+':
            for (int i = 0; i < a; i++) strcat(bf_code, "+");
            strcat(bf_code, ">");
            for (int i = 0; i < b; i++) strcat(bf_code, "+");
            strcat(bf_code, "<[->+<]>");
            break;

        case '-':
            for (int i = 0; i < a; i++) strcat(bf_code, "+");
            strcat(bf_code, ">");
            for (int i = 0; i < b; i++) strcat(bf_code, "+");
            strcat(bf_code, "<[->-<]>");
            break;

        case '*':
            // multiplicação a * b usando célula 0 = a, célula 1 = b, célula 2 = resultado
            sprintf(buffer,
                "[-]>[-]>[-]");           // limpa células 0,1,2
            strcat(bf_code, buffer);
            for (int i = 0; i < a; i++) strcat(bf_code, "+"); // célula 0 = a
            strcat(bf_code, ">");
            for (int i = 0; i < b; i++) strcat(bf_code, "+"); // célula 1 = b
            strcat(bf_code, "<[>[->+>+<<]>>[-<<+>>]<<<-]>>>"); // mult
            break;

        case '/':
            // divisão inteira simples: célula 0 = a, célula 1 = b, célula 2 = quociente
            sprintf(buffer,
                "[-]>[-]>[-]");           // limpa células 0,1,2
            strcat(bf_code, buffer);
            for (int i = 0; i < a; i++) strcat(bf_code, "+"); // célula 0 = a
            strcat(bf_code, ">");
            for (int i = 0; i < b; i++) strcat(bf_code, "+"); // célula 1 = b
            strcat(bf_code,
                "<[->>+>+<<<]>>>[-<<<+>>>]<" // duplica A em 2 lugares
                ">[-<<<[->->+<<]>>[-<<+>>]<<<->>[-]>[-<+>]<]>>>" // loop de subtração
            );
            break;

        default:
            fprintf(stderr, "Operador inválido! Use +, -, *, /\n");
            exit(1);
    }
}

int main() {
    char input[100];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // remove '\n'

    char *expr = strchr(input, ':');
    if (!expr) {
        fprintf(stderr, "Formato inválido! Use: 'nome: expressão'\n");
        return 1;
    }
    expr += 2;

    int a, b;
    char op;
    if (sscanf(expr, "%d %c %d", &a, &op, &b) != 3) {
        fprintf(stderr, "Expressão inválida. Ex: 2 + 3\n");
        return 1;
    }

    char bf_code[2000];
    generate_bf_code(a, b, op, bf_code);
    printf("%s\n", bf_code);
    return 0;
}
