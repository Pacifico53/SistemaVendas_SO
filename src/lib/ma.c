// MANUTENÇÃO DE ARTIGOS
// ao compilar (makefile) este ficheiro deverá gerar o ficheiro ma executável
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>

void menuShow(){
    printf("-----------------MANUTENÇÃO DE ARTIGOS-----------------------\n");
    printf(" i <nome> preço           --> insere novo artigo, mostra código\n");
    printf(" n <código> <novo nome>   --> altera nome do artigo\n");
    printf(" p <código> <novo preço>  --> altera preço do artigo\n" );
    printf(" outros comandos a adicionar\n");
}

int cenas(){
    int fdArtigos;
    int fdStrings;
    //abrir ficheiros artigos, strings e controlo e erros;
    if( (fdArtigos=open("artigos.txt", O_RDWR)) == -1){
        perror(" ERROR : opening artigos.txt");
        return 1;
    }
    if( (fdStrings=open("strings.txt", O_RDWR)) == -1){
        perror(" ERROR : opening strings.txt");
        return 1;
    }

    menuShow();
    while(1){
    }
}

