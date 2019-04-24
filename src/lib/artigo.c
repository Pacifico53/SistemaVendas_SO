#include "../../include/artigo.h"
struct str_artigo{
    int nome;
    int preco;
    int codigo;
};

Artigo create_artigo(int n, int p, int code){
    Artigo a = malloc(sizeof(struct str_artigo));
    a->nome = n;
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
    int fd = open("../database/ARTIGOS", O_WRONLY);
    char str[64] = " ";
    sprintf(str, "%d %d", a->nome, a->preco);
    printf("String: \"%s\"\n", str);

    if (write(fd, str, 64) > 1) {
        printf("Success.\n");
    }
    else {
        printf("Error writing to file.\n");
    }
}

void print_artigo(Artigo a){
    printf("Nome = %d\n", get_nome(a));
    printf("Preço = %d\n", get_preco(a));
    printf("Codigo = %d\n", get_code(a));
}

