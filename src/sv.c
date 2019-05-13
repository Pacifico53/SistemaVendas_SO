// SERVIDOR DE VENDAS

#include <sys/stat.h>
#include <ctype.h>
#include "../include/artigo.h"

#define LINE_BLOCK_SIZE 32


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
    char result[LINE_BLOCK_SIZE] = "";
    if(a){
        snprintf(result, LINE_BLOCK_SIZE, "%d %d\n", get_stock(a), get_preco(a));
    }
    return strdup(result);
}

char* update_stock(int code, int stock){
    Artigo a = seek_artigo(code);
    char result[LINE_BLOCK_SIZE] = "";
    if (a) {
        int stockFinal = get_stock(a)+stock > 0 ? get_stock(a)+stock : 0;
        change_stock(a, stockFinal);
        snprintf(result, LINE_BLOCK_SIZE, "%d\n", get_stock(a));
        free(a);
    }
    return strdup(result);
}

void save_venda(int code, int stock){
    int fd = open("database/VENDAS", O_WRONLY | O_APPEND);
    char venda[LINE_BLOCK_SIZE] = "";
    stock = stock * -1;

    Artigo a = seek_artigo(code);
    if (a) {
        int stockA = get_stock(a);
        int stockFinal = stockA-stock >= 0 ? stock : stockA;
        snprintf(venda, LINE_BLOCK_SIZE, "%d %d %d",
                code, stockFinal, get_preco(a)*stockFinal);
        write(fd, venda, LINE_BLOCK_SIZE);
        write(fd, "\n", 1);
    }
    close(fd);
}

char* exec_request(char* commands){
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

    //verificações checadas no cv
    if (cmds[2] == NULL) {
        return show_stock_price(atoi(cmds[0]));
    }
    else{
        if(atoi(cmds[1]) < 0){
            save_venda(atoi(cmds[0]), atoi(cmds[1]));
        }
        return update_stock(atoi(cmds[0]), atoi(cmds[1]));
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

int main(){
    // FIFO file path
    char *serverFIFO = "database/serverFIFO";
    char clienteFIFO[128] = "";

    // Creating the named file(FIFO)
    int fd_serverFIFO, fd_clienteFIFO;
    char buf[LINE_BLOCK_SIZE];

    if((mkfifo(serverFIFO, 0777)) == -1){
        perror("sv creating serverFIFO");
    }

    if ((fd_serverFIFO = open(serverFIFO, O_RDWR)) == -1){
      perror("sv opening serverFIFO");
    }

    while (read(fd_serverFIFO, buf, LINE_BLOCK_SIZE) > 0){
        char *reply;
        snprintf(clienteFIFO, 128, "database/clienteFIFO%s", getFIFO(strdup(buf)));
        fd_clienteFIFO = open(clienteFIFO, O_WRONLY);
        reply = exec_request(buf);
        write(fd_clienteFIFO, reply, LINE_BLOCK_SIZE);
        close(fd_clienteFIFO);
        free(reply);
        memset(buf,0,LINE_BLOCK_SIZE);
    }

    close(fd_serverFIFO);
    return 0;
}

