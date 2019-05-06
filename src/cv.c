// CLIENTE DE VENDAS

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define LINE_BLOCK_SIZE 128

int readline(char* buffer, int size){   //se retornar-mos o i temos os numeros de byts lidos/até ao '\n'
    int i = 0;
    char c;

    if(buffer == NULL || size == 0)
        return 0;

    while( read(0,&c,1) == 1 && i<size-1 ){
        if( c == '\n'){
          buffer[i] = 0;

          //  char pidFIFO[32] = "";
          //  snprintf(pidFIFO, 32, " database/fifo%d", getpid());
          //  strcat(buffer, pidFIFO);
            return i;
        }
        buffer[i++]=c;
    }
    buffer[i]=0;
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
    char* request = malloc(sizeof(LINE_BLOCK_SIZE));
    snprintf(request, sizeof(request)+20, "%s %d", cmds0, getpid());
    //strcat(request,pid_string);
    //strcat(request, " ");
    //strcat(request, cmds[0]);
    return request;
  }

  if(type == 2){
    char* request = malloc(sizeof(LINE_BLOCK_SIZE));
    snprintf(request, sizeof(request)+20, "%s %s %d", cmds0, cmds1, getpid());
    return request;
  }

  printf("cv Invalid command created\n");
  return NULL;
}

//checa se o comando é válido. ser for retorna char* com o pid+msg, caso contrário NULL
char* check_command(char* commands){
    printf("check command buf(readed) : %s\n",commands);
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
          printf("1 -> cmds[0] : %s    cmds[1] : %s    cmds[2] : %s\n", cmds[0], cmds[1], cmds[2]);
          char* request = build_request_msg(cmds[0], cmds[1], 1);
          return request;
        }else {
          return NULL;
        }
    }
    //comando tipo 2
    if (cmds[0] != NULL && cmds[1] != NULL) {
        if(isNumber(cmds[0]) && atoi(cmds[0]) > 0 && isNumber(cmds[1])){
          printf("2 -> cmds[0] : %s    cmds[1] : %s    cmds[2] : %s\n", cmds[0], cmds[1], cmds[2]);
          char* request = build_request_msg(cmds[0], cmds[1], 2);
          return request;
        } else {
          return NULL;
        }
    }
    return NULL;
}


void sigcont_handler(int sig){
    printf("Acordei %d\n", sig);
}



int main(){
    char *serverFIFO = "database/serverFIFO";
    char *clienteFIFO = "database/clienteFIFO";
    char  replie[128];
    int fd_serverFIFO, fd_clienteFIFO;
  //  char pidFIFO[64] = "";
  //  snprintf(pidFIFO, 64, "database/fifo%d", getpid());

    //signal(SIGCONT, sigcont_handler);

    if( (mkfifo(clienteFIFO, 0777)) == -1){
      perror("cv Creating clienteFIFO");
    }

    while (1){
        char *request;
        char buf[LINE_BLOCK_SIZE];
        // char res[LINE_BLOCK_SIZE];
        if( (fd_serverFIFO = open(serverFIFO, O_WRONLY)) == -1){
          perror("cv Opening fd serverFIFO");
        }

        // Take an input from user.
        int bytesreaded = readline(buf, LINE_BLOCK_SIZE);

        if( (request = check_command(buf)) != NULL ){
          printf("valid command\n");

          printf("Request command: %s  size: %ld  lenght : %ld\n",request, sizeof(request),strlen(request));
          //Write the input on FIFO and close it
          write(fd_serverFIFO, request, strlen(request));
          free(request);
          close(fd_serverFIFO);
          fd_clienteFIFO = open(clienteFIFO, O_RDONLY);

          // dup2(fdCLIENTE,1); ?? acho que não funciona com fifos


          read(fd_clienteFIFO, replie, LINE_BLOCK_SIZE);
          printf("replie : %s\n",replie);
          write(1,"yoo\n",4);
          close(fd_clienteFIFO);
          memset(replie, 0, LINE_BLOCK_SIZE);
          memset(buf,0,LINE_BLOCK_SIZE);
          /*close(fdSERVER);1
          fdPIDFIFO = open(pidFIFO, O_RDONLY);
          pause();


        read(fdPIDFIFO, res, LINE_BLOCK_SIZE);
        printf("%s", res);
        close(fdPIDFIFO);
        snprintf(buf, LINE_BLOCK_SIZE, " ");
        snprintf(res, LINE_BLOCK_SIZE, " ");*/
      } else{
        printf("Invalid comand\n");
      }
    }

    printf("Done.\n");
    return 0;
}
