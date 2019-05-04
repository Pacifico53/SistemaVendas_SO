#include "../../include/artigo.h"

struct str_artigo{
    int nome;
    float preco;
    int codigo;
};

Artigo create_artigo(char* n, float p, int code){
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
        return 0;
    }
    else {
        printf("Artigo com codigo %d encontrado:\n", code);
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
    a->preco = strtof(info[1], NULL);
    a->codigo = code;

    return a;
}


void set_nome(Artigo a, char* n){
    int newCode = save_name(n);
    a->nome = newCode;
    save_artigo(a);
}

void set_preco(Artigo a, float p){
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

    if(read(fd, n, 64) != 64){
        perror("Name not found.");
    }
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

float get_preco(Artigo a){
    return a->preco;
}

int get_code(Artigo a){
    return a->codigo;
}

int get_stock(Artigo a){
    int fd = open("database/STOCKS", O_RDONLY);
    int pos = (a->codigo-1) * 65;
    lseek(fd, pos, SEEK_SET);
    char buf[64];

    if(read(fd, buf, 64) < 0){
        perror("Name not found.");
    }
    int i = 0, flag = 1;
    for (i = 0; i < 64 && flag; i++) {
        if (buf[i] == '\n') {
            buf[i] = '\0';
            flag = 0;
        }
    }
    close(fd);

    return atoi(buf);
}

void change_stock(Artigo a, int stock){
    int fd = open("database/STOCKS", O_WRONLY);
    char str[64] = "";
    int pos = (a->codigo-1) * 65;
    snprintf(str, 64, "%d", stock);

    lseek(fd, pos, SEEK_SET);

    if ((write(fd, str, 64) > 1) && (write(fd, "\n", 1) > 0)) {
        printf("Success writing to file STOCKS.\n");
    }
    else {
        printf("Error writing to file STOCKS.\n");
    }

    close(fd);
}

void save_artigo(Artigo a){
    int fd = open("database/ARTIGOS", O_WRONLY);
    char str[64] = "";
    int pos = (a->codigo-1) * 65;
    snprintf(str, 64, "%d %f", a->nome, a->preco);

    lseek(fd, pos, SEEK_SET);

    if ((write(fd, str, 64) > 1) && (write(fd, "\n", 1) > 0)) {
        printf("Success writing to file ARTIGOS.\n");
    }
    else {
        printf("Error writing to file ARTIGOS.\n");
    }

    close(fd);

    change_stock(a, 0);
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
    printf("===========\nNome = %s\n", get_nome(a));
    printf("Posicao do Nome = %d\n", get_index_nome(a));
    printf("Preço = %f\n", get_preco(a));
    printf("Codigo = %d\n===========\n", get_code(a));
}

