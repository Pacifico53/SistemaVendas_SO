CC = gcc
CFLAGS=-Wall

all: manutencaoArtigos testing servidorVendas clienteVendas

manutencaoArtigos:
	$(CC) $(CFLAGS) -o ma src/ma.c src/lib/artigo.c

servidorVendas:
	$(CC) $(CFLAGS) -o sv src/sv.c src/lib/artigo.c

clienteVendas:
	$(CC) $(CFLAGS) -o cv src/cv.c src/lib/artigo.c

testing:
	$(CC) $(CFLAGS) -o testing src/testing.c src/lib/artigo.c

clean:
	rm -f ma testing cv sv

resetDB:
	rm -f database/*
	touch database/ARTIGOS database/STOCKS database/STRINGS database/VENDAS
