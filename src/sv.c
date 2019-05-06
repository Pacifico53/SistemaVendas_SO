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
        snprintf(result, 128, "Stock = %d\nPreço = %f\n", get_stock(a), get_preco(a));
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

char* exec_request(char* commands){
    printf("string : %s\n",commands);
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
    printf("%s %s %s\n",cmds[0],cmds[1],cmds[2]);

    //verificações checadas no cv
    if (cmds[2] == NULL) {
      return show_stock_price(atoi(cmds[0]));

    }else if (cmds[0] != NULL && cmds[1] != NULL) {
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
    char *clienteFIFO = "database/clienteFIFO";
    // Creating the named file(FIFO)
    int fd_serverFIFO, fd_clienteFIFO;
    char buf[LINE_BLOCK_SIZE];

    if( (mkfifo(serverFIFO, 0777)) == -1){
      perror("sv Creating serverFIFO");
    }

    while (1){
        char *replie;

        if ( (fd_serverFIFO = open(serverFIFO, O_RDONLY)) == -1){
          perror("sv Opening fd serverFIFO");
        }

        if( read(fd_serverFIFO, buf, LINE_BLOCK_SIZE) > 0 ){
            printf("String:%s\n", buf);
            replie = exec_request(strdup(buf));
            if(replie == NULL){
              printf("fudeuuu\n");
            }
            //close(fdSERVER);
          //  char* pidFIFO = getFIFO(strdup(buf));
//printf("pidFIFO : %s\n",pidFIFO);
            fd_clienteFIFO = open(clienteFIFO, O_WRONLY);
            write(fd_clienteFIFO, replie, strlen(replie));

            close(fd_clienteFIFO);
          }
          /*  if(!strcmp(res, "")){
                perror("Invalid input.");
            }
            else {
                char* pidFIFO = getFIFO(strdup(buf));
                printf("pidFIFO = !%s!\n", pidFIFO);
                fdPIDFIFO = open(pidFIFO, O_WRONLY);
                write(fdPIDFIFO, res, strlen(res));
                printf("Respondi para %s.\n", pidFIFO);
                kill(get_pid_fifo(pidFIFO), SIGCONT);
                free(pidFIFO);
                close(fdPIDFIFO);
            }
        }
        snprintf(res, LINE_BLOCK_SIZE, " ");*/
        memset(buf,0,LINE_BLOCK_SIZE);//snprintf(buf, LINE_BLOCK_SIZE, "");

        //close(fd);
    }

	return 0;
}
