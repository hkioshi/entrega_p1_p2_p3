#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool Z = false;
bool N = false;
int mem[516];
int ac = 0;
int pc = 4;

void AbrirArquivo() {
    FILE *file = fopen("programa.bin", "rb");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    printf("Arquivo aberto com sucesso\n");

    memset(mem, 0, sizeof(mem));
    int i = 0;
    unsigned char byte;

    while (i < 516 && fread(&byte, 1, 1, file) == 1) {
        mem[i++] = byte;
    }

    fclose(file);
}

bool verificarArquivo() {
    return (mem[0] == 0x03 && mem[1] == 0x4E && mem[2] == 0x44 && mem[3] == 0x52);
}

int acharPos(int pos)
{
    return mem[pos*2+4];
}

int endereco(int pos) {
    return acharPos(acharPos(pos+1));
}

void flag() {
    Z = (ac == 0);
    N = (ac < 0);
}

int main() {
    AbrirArquivo();
    if (!verificarArquivo()) {
        printf("Arquivo inválido\n");
        return 1;
    }
    flag();

    printf("Iniciando execucao...\n");
    while (pc < 516) {
        int opcode = mem[pc];
        if (opcode == 0xF0) {
            printf("Instrucao de parada encontrada.\n");
            break;
        }

        switch (opcode) {
            case 0x10:
                mem[endereco(pc)] = ac;
                pc += 4;
                break;
            case 0x20:
                ac = endereco(pc);
                pc += 4;
                break;
            case 0x30:
                ac += mem[endereco(pc)];
                pc += 4;
                break;
            case 0x40:
                ac |= mem[endereco(pc)];
                pc += 4;
                break;
            case 0x50:
                ac &= mem[endereco(pc)];
                pc += 4;
                break;
            case 0x60:
                ac = ~ac;
                pc += 2;
                break;
            case 0x80:
                pc = endereco(pc);
                break;
            case 0x90:
                if (N) pc = endereco(pc);
                else pc += 4;
                break;
            case 0xA0:
                if (Z) pc = endereco(pc);
                else pc += 4;
                break;
            default:
                pc += 2;
        }
        flag();
    }

    printf("Ac: %d\nPc: %d\n", ac, pc);
    return 0;
}