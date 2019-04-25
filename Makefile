CC = gcc
CFLAGS=-Wall

all: manutencaoArtigos testing

manutencaoArtigos:
	$(CC) $(CFLAGS) -o ma src/ma.c src/lib/artigo.c

testing:
	$(CC) $(CFLAGS) -o testing src/testing.c src/lib/artigo.c

clean:
	rm -f ma testing

resetDB:
	rm -f database/*
	touch database/ARTIGOS database/STOCKS database/STRINGS database/VENDAS
