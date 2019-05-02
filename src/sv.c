// SERVIDOR DE VENDAS

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include "../include/artigo.h"

#define LINE_BLOCK_SIZE 128


int isNumber(char* str){
   int i = 0, flag = 1;
   for (i = 0; str[i] && flag; i++) {
        if (i == 0){
           if(isdigit(str[i]) == 0 && str[i] != '-') {
                flag = 0;
            }
        }
        else {
            if (isdigit(str[i]) == 0) {
                flag = 0;
            }
        }
   }
   return flag;
}

void show_stock_price(int code){
    printf("Code = %d\n", code);
    Artigo a = seek_artigo(code);
    if(a){
        printf("Stock = %d\nPreço = %f\n", get_stock(a), get_preco(a));
    }
}

void update_stock(int code, int stock){
    printf("Code = %d\nStock = %d\n", code, stock);
    Artigo a = seek_artigo(code);
    if (a) {
        change_stock(a, get_stock(a)-stock);
        free(a);
    }
}

int check_command(char* commands){
    char *token = strtok(commands, " ");
    char *cmds[2];
    cmds[0] = NULL;
    cmds[1] = NULL;
    int i = 0;

    while (token && i<3) {
        cmds[i++] = strdup(token);
        token = strtok(NULL, " ");
    }

    if (cmds[1] == NULL) {
        if (isNumber(cmds[0]) && atoi(cmds[0]) > 0) {
            show_stock_price(atoi(cmds[0]));
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        if (isNumber(cmds[0]) && atoi(cmds[0]) > 0 && isNumber(cmds[1])) {
            update_stock(atoi(cmds[0]), atoi(cmds[1]));
            return 2;
        }
        else {
            return 0;
        }
    }
    return 0;
}

int main(){
    int fd;

    // FIFO file path
    char *serverFIFO = "database/fifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>,<permission>)
    mkfifo(serverFIFO, 0777);

    char buf[LINE_BLOCK_SIZE];
    while (1){
        // First open in read only and read
        fd = open(serverFIFO, O_RDONLY);
        printf("Entrou cliente\n");
        read(fd, buf, LINE_BLOCK_SIZE);
        printf("String: %s\n", buf);
        if(check_command(buf) == 0){
            perror("Invalid input.");
        }

        close(fd);
    }

	return 0;
}
