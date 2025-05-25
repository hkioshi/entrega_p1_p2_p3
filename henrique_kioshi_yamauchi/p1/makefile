# Makefile para o projeto Neander

CC = gcc
CFLAGS = -Wall -O2

# Nomes dos arquivos
PARSER = parcer
INTERPRETER = interpretador
EXECUTOR = executor

# Regras principais
all: $(PARSER) $(INTERPRETER) $(EXECUTOR)

$(PARSER): parcer.c
	$(CC) $(CFLAGS) -o $(PARSER) parcer.c

$(INTERPRETER): interpretador.c
	$(CC) $(CFLAGS) -o $(INTERPRETER) interpretador.c

$(EXECUTOR): executor.c
	$(CC) $(CFLAGS) -o $(EXECUTOR) executor.c

run: all
	@echo "Executando pipeline: .lpn → .asm → .bin → execução"
	./$(PARSER)
	./$(INTERPRETER)
	./$(EXECUTOR)

clean:
	rm -f $(PARSER) $(INTERPRETER) $(EXECUTOR) *.asm *.bin

.PHONY: all run clean
