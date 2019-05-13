// CLIENTE DE VENDAS

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>

#define LINE_BLOCK_SIZE 32

int readline(char *buffer, int size){
    int i = 0;
    char c;
    int bytes = 0;
    if(buffer == NULL || size == 0)
        return 0;
    while((bytes = read(0, &c, 1)) != 0 && i < size - 1){
        if(c == '\n'){
            buffer[i] = 0;
            return i;
        }
        buffer[i++] = c;
    }
    if (bytes == 0) {
        return -1;
    }
    buffer[i] = '\0';
    return i;
}

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

char* build_request_msg(char* cmds0, char* cmds1, int type){
    if(type == 1){
        char* request = malloc(LINE_BLOCK_SIZE);
        snprintf(request, LINE_BLOCK_SIZE, "%s %d", cmds0, getpid());
        return request;
    }

    if(type == 2){
        char* request = malloc(LINE_BLOCK_SIZE);
        snprintf(request, LINE_BLOCK_SIZE, "%s %s %d", cmds0, cmds1, getpid());
        return request;
    }

    return NULL;
}

//checa se o comando é válido. ser for retorna char* com o pid+msg, caso contrário NULL
char* check_command(char* commands){
    if(!strcmp(commands,"\0")){
        return NULL;
    }
    char *token = strtok(commands, " ");
    char *cmds[2];
    cmds[0] = NULL;
    cmds[1] = NULL;
    cmds[2] = NULL;
    int i = 0;

    while (token && i<4) {
        cmds[i]=token;
        i++;
        token=strtok(NULL," ");
    }

    if(cmds[2] != NULL){ //comando inválido, argumentos a mais
        return NULL;
    }

    //comando tipo 1
    if (cmds[1] == NULL) {
        if (isNumber(cmds[0]) && atoi(cmds[0]) > 0) {
            char* request = build_request_msg(cmds[0], cmds[1], 1);
            return request;
        }
        else {
            return NULL;
        }
    }

    //comando tipo 2
    if (cmds[0] != NULL && cmds[1] != NULL) {
        if(isNumber(cmds[0]) && atoi(cmds[0]) > 0 && isNumber(cmds[1])){
            char* request = build_request_msg(cmds[0], cmds[1], 2);
            return request;
        }
        else {
            return NULL;
        }
    }
    return NULL;
}

int main(){
    char *serverFIFO = "database/serverFIFO";
    char clienteFIFO[128] = "";
    snprintf(clienteFIFO, 128, "database/clienteFIFO%d", getpid());
    int fd_serverFIFO, fd_clienteFIFO;
    int rl = 0;

    if( (mkfifo(clienteFIFO, 0777)) == -1){
      perror("cv Creating clienteFIFO");
    }

   if( (fd_serverFIFO = open(serverFIFO, O_WRONLY)) == -1){
      perror("cv Opening fd serverFIFO");
    }


    char *request;
    char buf[LINE_BLOCK_SIZE];
    char reply[LINE_BLOCK_SIZE];

    while(1){
        int br=0;

        rl = readline(buf, LINE_BLOCK_SIZE);
        if (rl == -1) {
            break;
        }        

        if((request = check_command(buf)) != NULL ){
            write(fd_serverFIFO, request, LINE_BLOCK_SIZE);
            fd_clienteFIFO = open(clienteFIFO, O_RDONLY);

            while((br = read(fd_clienteFIFO, reply, LINE_BLOCK_SIZE))<=0);
            write(1,reply, strlen(reply));
                
            close(fd_clienteFIFO);
        }
        else{
            write(1, "Invalid comand\n", 15);
        }

        memset(reply, 0, LINE_BLOCK_SIZE);
        memset(buf,0, LINE_BLOCK_SIZE);
        free(request);
    }
    
    close(fd_serverFIFO);
    return 0;
}

