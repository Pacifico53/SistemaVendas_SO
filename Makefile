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
	$(CC) $(CFLAGS) -o ag src/ag.c

scripts:
	$(CC) $(CFLAGS) -o scriptMA test_scripts/script1.c
	$(CC) $(CFLAGS) -o scriptCV test_scripts/script2.c

automate:
	./scriptMA
	./ma < test_scripts/insertScript
	./scriptCV

clean:
	rm -f ma cv sv ag scriptMA scriptCV 
	rm -f database/serverFIFO database/clienteFIFO*
	rm -f test_scripts/insertScript test_scripts/vendasScript

resetDB:
	rm -f database/*
	touch database/ARTIGOS database/STOCKS database/STRINGS database/VENDAS

