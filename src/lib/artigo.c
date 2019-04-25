#include "../../include/artigo.h"

struct str_artigo{
    int nome;
    int preco;
    int codigo;
};

Artigo create_artigo(char* n, int p, int code){
    Artigo a = malloc(sizeof(struct str_artigo));
    
    a->nome = save_name(n);
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

int get_index_nome(Artigo a){
    return a->nome;
}

char* get_nome(Artigo a){
    int fd = open("../database/STRINGS", O_RDONLY);
    lseek(fd, a->nome, SEEK_SET);
    char n[64];

    read(fd, n, 64);
    int i = 0, flag = 1;
    for (i = 0; i < 64 && flag; i++) {
        if (n[i] == '\n') {
            n[i] = '\0';
            flag = 0;
        }
    }
    close(fd);

    return strdup(n);
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

    if (write(fd, str, 64) > 1) {
        printf("Success a escrever artigo no file ARTIGOS.\n");
    }
    else {
        printf("Error writing to file ARTIGOS.\n");
    }

    close(fd);
}

int save_name(char* name){
    int fd = open("../database/STRINGS", O_WRONLY | O_APPEND);
    
    int r = lseek(fd, 0, SEEK_END);
    if (write(fd, name, strlen(name)) > 1) {
        write(fd, "\n", 1);
        printf("Success a escrever o nome no file STRINGS.\n");
    }
    else {
        printf("Error writing to file STRINGS.\n");
    }

    close(fd);
    return r;
}

void print_artigo(Artigo a){
    printf("====\nNome = %s\n", get_nome(a));
    printf("Posicao do Nome = %d\n", get_index_nome(a));
    printf("Preço = %d\n", get_preco(a));
    printf("Codigo = %d\n====\n", get_code(a));
}

