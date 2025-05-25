#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 128
#define MAX_VARS 100

// Estrutura para armazenar variáveis no formato nome:valor
typedef struct {
    char name[32];
    char value[32];
} Var;

Var dataSection[MAX_VARS];
int dataCount = 0;

char codeSection[100][MAX_LINE];
int codeCount = 0;

// Função que remove espaços em branco no final de uma string
void trim(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[len-1] = '\0';
        len--;
    }
}

// Função que verifica se a string é um número
int isNumber(const char *s) {
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

// Função para adicionar variáveis à seção de dados
void addData(const char *name, const char *value) {
    strcpy(dataSection[dataCount].name, name);
    strcpy(dataSection[dataCount].value, value);
    dataCount++;
}

// Função para processar o arquivo de entrada .lpn
void parseFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char line[MAX_LINE];
    int inCode = 0;

    while (fgets(line, sizeof(line), fp)) {
        trim(line);

        if (strncmp(line, "PROGRAMA", 8) == 0) {
            continue;  // Ignora "PROGRAMA"
        } else if (strcmp(line, "INICIO") == 0) {
            continue;  // Ignora "INICIO"
        } else if (strcmp(line, "FIM") == 0) {
            break;  // Encerra o processamento ao encontrar "FIM"
        } else if (strchr(line, '=') && strchr(line, '+')) {
            // Código: X = A + B
            char dest[32], op1[32], op2[32];
            sscanf(line, "%s = %s + %s", dest, op1, op2);

            // Comentário no formato "; X = A + B"
            char comment[MAX_LINE];
            snprintf(comment, sizeof(comment), "; %s = %s + %s", dest, op1, op2);
            strcpy(codeSection[codeCount++], comment);

            // Geração do código Neander
            snprintf(codeSection[codeCount++], MAX_LINE, "LDA %s ; %s", op1, op1);
            snprintf(codeSection[codeCount++], MAX_LINE, "ADD %s ; %s", op2, op2);
            snprintf(codeSection[codeCount++], MAX_LINE, "STA %s ; %s", dest, dest);

            // Adiciona a variável à seção de dados
            addData(dest, "?");
            inCode = 1;
        } else if (strchr(line, '=')) {
            // Variáveis: A = 10 ou B = ?
            char name[32], val[32];
            sscanf(line, "%s = %s", name, val);
            addData(name, val);
        }
    }

    fclose(fp);
}

// Função para gerar o arquivo de saída no formato Neander
void generateASM(const char *outFile) {
    FILE *fp = fopen(outFile, "w");
    if (!fp) {
        perror("Erro ao criar arquivo de saída");
        exit(1);
    }

    // Escreve a seção de dados (.DATA)
    fprintf(fp, ".DATA\n");
    for (int i = 0; i < dataCount; i++) {
        fprintf(fp, "%s DB %s\n", dataSection[i].name, dataSection[i].value);
    }

    // Escreve a seção de código (.CODE)
    fprintf(fp, "\n.CODE\n.ORG 0\n");

    // Escreve as instruções de código geradas
    for (int i = 0; i < codeCount; i++) {
        fprintf(fp, "%s\n", codeSection[i]);
    }

    // Finaliza com a instrução HLT
    fprintf(fp, "HLT\n");

    fclose(fp);
    printf("Arquivo .asm gerado com sucesso!\n");
}


int main() {
    parseFile("programa.lpn");  // Lê o arquivo .lpn
    generateASM("saida.asm");   // Gera o arquivo .asm
    return 0;
}
