CC = gcc
CFLAGS=-Wall

all: manutencaoArtigos servidorVendas clienteVendas agregador scripts

manutencaoArtigos:
	$(CC) $(CFLAGS) -o ma src/ma.c src/lib/artigo.c

servidorVendas:
	$(CC) $(CFLAGS) -o sv src/sv.c src/lib/artigo.c

clienteVendas:
	$(CC) $(CFLAGS) -o cv src/cv.c src/lib/artigo.c

agregador:
	$(CC) $(CFLAGS) -o ag src/ag.c src/lib/artigo.c

scripts:
	$(CC) $(CFLAGS) -o scr test_scripts/script1.c

clean:
	rm -f ma cv sv ag scr database/serverFIFO database/clienteFIFO* database/20*

resetDB:
	rm -f database/*
	touch database/ARTIGOS database/STOCKS database/STRINGS database/VENDAS

