#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 128
#define MAX_INSTRUCOES 128

unsigned char memoria[516]; // Agora a memória é byte a byte

typedef struct {
    char nome[10];
    int valor;
    int pos;
} Variavel;

typedef struct {
    int codigo;
    int endereco;
} Instrucao;

int acharInstrucao(char *x) {
    if (strncmp(x, "STA", 3) == 0) return 0x10;
    if (strncmp(x, "LDA", 3) == 0) return 0x20;
    if (strncmp(x, "ADD", 3) == 0) return 0x30;
    if (strncmp(x, "OR", 2)  == 0) return 0x40;
    if (strncmp(x, "AND", 3) == 0) return 0x50;
    if (strncmp(x, "NOT", 3) == 0) return 0x60;
    if (strncmp(x, "JMP", 3) == 0) return 0x80;
    if (strncmp(x, "JN", 2)  == 0) return 0x90;
    if (strncmp(x, "JZ", 2)  == 0) return 0xA0;
    if (strncmp(x, "HLT", 3) == 0) return 0xF0;
    return 0x00; // Instrução inválida
}

int acharEndereco(Variavel variaveis[], int varCount, char *nome) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(variaveis[i].nome, nome) == 0) {
            return variaveis[i].pos;
        }
    }
    return -1;
}

int corrigirPos(int pos) {
    return pos * 2 + 4;
}

int main() {

    FILE *file = fopen("programa.asm", "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    Variavel variaveis[MAX_VARS];
    Instrucao instrucoes[MAX_INSTRUCOES];
    int varCount = 0, instrCount = 0;
    int pos = -1;
    int posVar = 128;
    char linha[256];
    int secao = 0; // 1 = .DATA, 2 = .CODE

    while (fgets(linha, sizeof(linha), file)) {
        if (linha[0] == ';' || linha[0] == '\n') continue;

        if (strncmp(linha, ".DATA", 5) == 0) {
            secao = 1;
            continue;
        }
        if (strncmp(linha, ".CODE", 5) == 0) {
            secao = 2;
            continue;
        }
        if (strncmp(linha, ".ORG", 4) == 0) {
            sscanf(linha, ".ORG %d", &pos);
            continue;
        }

        if (secao == 1) {
            char nome[10], tipo[10], valorStr[10];
            sscanf(linha, "%s %s %s", nome, tipo, valorStr);
            strcpy(variaveis[varCount].nome, nome);
            if (strcmp(valorStr, "?") == 0) {
                variaveis[varCount].valor = 0;
            } else {
                variaveis[varCount].valor = atoi(valorStr);
            }
            variaveis[varCount].pos = posVar;
            varCount++;
            posVar++;
        } else if (secao == 2) {
            char var[10];
            char instr[10];
            sscanf(linha, "%s %s", instr, var);
            instrucoes[instrCount].codigo = acharInstrucao(instr);
            instrucoes[instrCount].endereco = acharEndereco(variaveis, varCount, var);
            instrCount++;
        }
    }
    fclose(file);

    memoria[0] = 3;
    memoria[1] = 0x4E; // 'N'
    memoria[2] = 0x44; // 'D'
    memoria[3] = 0x52; // 'R'

    int index = 4;
    for (int i = 0; i < instrCount; i++) {
        memoria[index] = instrucoes[i].codigo;
        memoria[index + 1] = 0x00;

        if (instrucoes[i].endereco == -1) {
            index += 2;
        } else {
            memoria[index + 2] = (unsigned char) instrucoes[i].endereco;
            memoria[index + 3] = 0x00;
            index += 4;
        }
    }

    while (index < 260) {
        memoria[index++] = 0x00;
    }

    int num = 0;
    while (index < 516) {
        if (num < varCount && corrigirPos(variaveis[num].pos) == index) {
            memoria[index] = (unsigned char) variaveis[num].valor;
            memoria[index + 1] = 0x00;
            index += 2;
            num++;
        } else {
            memoria[index++] = 0x00;
        }
    }

    FILE *memFile = fopen("programa.bin", "wb"); 
    if (memFile == NULL) {
        perror("Erro ao criar o arquivo de memória");
        return 1;
    }

    fwrite(memoria, sizeof(unsigned char), 516, memFile);
    fclose(memFile);

    return 0;
}
