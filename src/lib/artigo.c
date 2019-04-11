#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../include/artigo.h"

struct str_artigo{
    char* nome;         //Isto tem de ser um numero para apontar para um nome no ficheiro STRINGS
    char* descricao;    //Igual a cena do nome
    int preco;
    int codigo;
};

Artigo create_artigo(char* n, char* d, int p, int code){
    Artigo a = malloc(sizeof(struct str_artigo));
    a->nome = strdup(n);
    a->descricao = strdup(d);
    a->preco = p;
    a->codigo = code;

    return a;
}

void set_nome(Artigo a, char* n){
    free(a->nome);
    a->nome = strdup(n);
}

void set_descr(Artigo a, char* d){
    free(a->descricao);
    a->descricao = strdup(d);
}

void set_preco(Artigo a, int p){
    a->preco = p;
}

char* get_nome(Artigo a){
    return strdup(a->nome);
}

char* get_descr(Artigo a){
    return strdup(a->descricao);
}

int get_preco(Artigo a){
    return a->preco;
}

int get_code(Artigo a){
    return a->codigo;
}

void print_artigo(Artigo a){
    printf("Nome = %s\n", get_nome(a));
    printf("Descriçao = %s\n", get_descr(a));
    printf("Preço = %d\n", get_preco(a));
    printf("Codigo = %d\n", get_code(a));
}

