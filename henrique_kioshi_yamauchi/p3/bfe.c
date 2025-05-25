#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_brainfuck(const char *code) {
    unsigned char tape[1000] = {0};
    int ptr = 0;
    int loop_stack[100];
    int stack_ptr = 0;

    for (int i = 0; code[i]; i++) {
        switch (code[i]) {
            case '>':
                ptr++;
                if (ptr >= 1000) {
                    fprintf(stderr, "Erro: ponteiro fora da fita\n");
                    exit(1);
                }
                break;
            case '<':
                ptr--;
                if (ptr < 0) {
                    fprintf(stderr, "Erro: ponteiro negativo\n");
                    exit(1);
                }
                break;
            case '+': tape[ptr]++; break;
            case '-': tape[ptr]--; break;
            case '.': putchar(tape[ptr]); break;
            case ',':
                tape[ptr] = getchar();
                break;
            case '[':
                if (tape[ptr] == 0) {
                    int depth = 1;
                    while (depth > 0) {
                        i++;
                        if (code[i] == '[') depth++;
                        else if (code[i] == ']') depth--;
                    }
                } else {
                    loop_stack[stack_ptr++] = i;
                }
                break;
            case ']':
                if (tape[ptr] != 0) {
                    i = loop_stack[--stack_ptr];
                } else {
                    stack_ptr--;
                }
                break;
        }
    }

    return tape[ptr];  // valor final da célula atual
}

int main() {
    char bf_code[2000];
    fgets(bf_code, sizeof(bf_code), stdin);
    int result = run_brainfuck(bf_code);
    printf("%d\n", result);
    return 0;
}
