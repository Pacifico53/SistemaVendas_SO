// SERVIDOR DE VENDAS

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
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

char* show_stock_price(int code){
    Artigo a = seek_artigo(code);
    char result[128] = "";
    if(a){
        snprintf(result, 128, "Stock = %d\nPreço = %d\n", get_stock(a), get_preco(a));
    }
    return strdup(result);
}

char* update_stock(int code, int stock){
    Artigo a = seek_artigo(code);
    char result[128];
    if (a) {
        change_stock(a, get_stock(a)+stock);
        snprintf(result, 128, "Stock = %d\n", get_stock(a));
        free(a);
    }
    return strdup(result);
}

void save_venda(int code, int stock){
    int fd = open("database/VENDAS", O_WRONLY | O_APPEND);
    char venda[64] = "";
    stock = stock * -1;

    Artigo a = seek_artigo(code);
    if (a) {
        snprintf(venda, 64, "%d %d %d\n",
                code, stock, get_preco(a)*stock);
        write(fd, venda, 64);
    }
    close(fd);
}

char* exec_request(char* commands){
    //printf("String : %s\n",commands);
    char *token = strtok(commands, " ");
    char *cmds[2];
    cmds[0] = NULL;
    cmds[1] = NULL;
    cmds[2] = NULL;
    int i = 0;

    while (token && i<4) {
        cmds[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    //printf("%s %s %s\n",cmds[0],cmds[1],cmds[2]);

    //verificações checadas no cv
    if (cmds[2] == NULL) {
        return show_stock_price(atoi(cmds[0]));
    }
    else if (cmds[0] != NULL && cmds[1] != NULL) {
        if(atoi(cmds[1]) < 0){
            save_venda(atoi(cmds[0]), atoi(cmds[1]));
        }
        return update_stock(atoi(cmds[0]), atoi(cmds[1]));

    }else {
        printf("Fudeuu\n");
        return NULL;
    }
}

char* getFIFO(char* buffer){
    char *token = strtok(buffer, " ");
    char *cmds[2];
    cmds[0] = NULL;
    cmds[1] = NULL;
    cmds[2] = NULL;
    int i = 0;

    while (token && i<4) {
        cmds[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    if (cmds[2] == NULL) {
        return strdup(cmds[1]);
    }
    else{
        return strdup(cmds[2]);
    }
}

pid_t get_pid_fifo(char* file){
    int i, j = 0, len = strlen(file);
    char pid[5];
    for (i = len - 5; i < len; i++) {
        pid[j++] = file[i];
    }
    return (pid_t)(atoi(pid));
}

int main(){
    // FIFO file path
    char *serverFIFO = "database/serverFIFO";
    char clienteFIFO[128] = "";
    // Creating the named file(FIFO)
    int fd_serverFIFO, fd_clienteFIFO;
    char buf[LINE_BLOCK_SIZE];

    if( (mkfifo(serverFIFO, 0777)) == -1){
        perror("sv Creating serverFIFO");
    }

    while (1){
        char *reply;
        if ( (fd_serverFIFO = open(serverFIFO, O_RDONLY)) == -1){
          perror("sv Opening fd serverFIFO");
        }

        if( read(fd_serverFIFO, buf, LINE_BLOCK_SIZE) > 0 ){
            reply = exec_request(strdup(buf));
            /*
            if(reply == NULL){
              printf("fudeuuu\n");
            }
            */
            snprintf(clienteFIFO, 128, "database/clienteFIFO%s", getFIFO(strdup(buf)));
            fd_clienteFIFO = open(clienteFIFO, O_WRONLY);
            write(fd_clienteFIFO, reply, strlen(reply));
            free(reply);
            close(fd_clienteFIFO);
            printf("===Enviada Resposta===\n");
        }
        memset(buf,0,LINE_BLOCK_SIZE);
    }

	return 0;
}

