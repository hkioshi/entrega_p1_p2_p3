CC = gcc
CFLAGS = -Wall -Wextra

all: bfc bfe

bfc: bfc.c
	$(CC) $(CFLAGS) bfc.c -o bfc

bfe: bfe.c
	$(CC) $(CFLAGS) bfe.c -o bfe

clean:
	rm -f bfc bfe

run:
	echo "teste: 6 * 7" | ./bfc | ./bfe

.PHONY: all clean run
