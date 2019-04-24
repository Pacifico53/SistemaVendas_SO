#include "../../include/artigo.h"

struct str_artigo{
    int nome;
    int preco;
    int codigo;
};

Artigo create_artigo(char* n, int p, int code){
    Artigo a = malloc(sizeof(struct str_artigo));
    save_name(n);
    a->nome = 53;
    a->preco = p;
    a->codigo = code;

    return a;
}

void set_nome(Artigo a, int n){
    a->nome = n;
}

void set_preco(Artigo a, int p){
    a->preco = p;
}

int get_nome(Artigo a){
    return a->nome;
}

int get_preco(Artigo a){
    return a->preco;
}

int get_code(Artigo a){
    return a->codigo;
}

void save_artigo(Artigo a){
    int fd = open("../database/ARTIGOS", O_WRONLY | O_APPEND);
    char str[64] = " ";
    sprintf(str, "%d %d\n", a->nome, a->preco);
    printf("String: \"%s\"\n", str);

    if (write(fd, str, 64) > 1) {
        printf("Success ARTIGOS.\n");
    }
    else {
        printf("Error writing to file ARTIGOS.\n");
    }
}

void save_name(char* name){
    int fd = open("../database/STRINGS", O_WRONLY | O_APPEND);
    if (write(fd, name, sizeof(name)+1) > 1) {
        write(fd, "\n", 1);
        printf("Success STRINGS.\n");
    }
    else {
        printf("Error writing to file STRINGS.\n");
    }
}

void print_artigo(Artigo a){
    printf("Nome = %d\n", get_nome(a));
    printf("Preço = %d\n", get_preco(a));
    printf("Codigo = %d\n", get_code(a));
}

