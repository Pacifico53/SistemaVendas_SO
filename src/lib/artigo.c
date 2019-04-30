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

Artigo seek_artigo(int code){
    int fd = open("database/ARTIGOS", O_RDONLY);
    int pos = (code-1) * 65;
    lseek(fd, pos, SEEK_SET);

    char buf[64] = "";
    if (read(fd, buf, 64) != 64) {
        printf("Erro a ler\n");
    }
    else {
        printf("Li isto: \'%s\'\n", buf);
    }

    char* token;
    char* info[2];
    int i = 0;

    token = strtok(buf, " ");
    while (token) {
        info[i] = strdup(token);
        i++;
        token = strtok(NULL, " ");
    }

    Artigo a = malloc(sizeof(struct str_artigo));

    a->nome = atoi(info[0]);
    a->preco = atoi(info[1]);
    a->codigo = code;

    print_artigo(a);
    return a;
}


void set_nome(Artigo a, char* n){
    int newCode = save_name(n);
    a->nome = newCode;
    save_artigo(a);
}

void set_preco(Artigo a, int p){
    a->preco = p;
    save_artigo(a);
}

int get_index_nome(Artigo a){
    return a->nome;
}

char* get_nome(Artigo a){
    int fd = open("database/STRINGS", O_RDONLY);
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
    int fd = open("database/ARTIGOS", O_WRONLY);
    char str[64] = "";
    int pos = (a->codigo-1) * 65;
    snprintf(str, 64, "%d %d", a->nome, a->preco);

    lseek(fd, pos, SEEK_SET);

    if ((write(fd, str, 64) > 1) && (write(fd, "\n", 1) > 0)) {

        printf("Success writing to file ARTIGOS.\n");
    }
    else {
        printf("Error writing to file ARTIGOS.\n");
    }

    close(fd);
}

int save_name(char* name){
    int fd = open("database/STRINGS", O_WRONLY | O_APPEND);

    int r = lseek(fd, 0, SEEK_END);
    if ((write(fd, name, strlen(name)) > 0) && (write(fd, "\n", 1) > 0)) {
        printf("Success writing to file STRINGS.\n");
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
