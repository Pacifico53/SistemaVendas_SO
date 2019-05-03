// CLIENTE DE VENDAS

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LINE_BLOCK_SIZE 128

int readline(char* buffer, int size){   //se retornar-mos o i temos os numeros de byts lidos/até ao '\n'
    char c;
    int i=0;

    if(buffer == NULL || size == 0)
        return 0;

    while( read(0,&c,1) == 1 && i<size-1 ){
        if( c == '\n'){
            buffer[i]=0;
            return i;
        }
        buffer[i++]=c;
    }
    buffer[i]=0;
    return i;
}

int isValidComand(){

    return 0;
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
          //  show_stock_price(atoi(cmds[0]));
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        if (isNumber(cmds[0]) && atoi(cmds[0]) > 0 && isNumber(cmds[1])) {
          //  update_stock(atoi(cmds[0]), atoi(cmds[1]));
            return 2;
        }
        else {
            return 0;
        }
    }
    return 0;
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

int main(){
    char * serverFIFO = "database/serverFIFO";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    //   mkfifo(serverFIFO, 0777);	quem cria o fifo é o cv
    int fd;
    char buf[LINE_BLOCK_SIZE];

        // Open FIFO for write only
    if( (fd = open(serverFIFO, O_WRONLY)) == -1 ){
	     perror("cv Opening serverFIFO");
    }


     while(1){
    	  printf("pid :%i\n",getpid());
	// Take an input arr2ing from user.
        int bytesreaded = readline(buf, LINE_BLOCK_SIZE);

        // Write the input on FIFO and close it
        if(check_command(buf) == 0){
          printf("Invalid command\n");
        }else{
        write(fd, buf, bytesreaded);
        }
    }

    close(fd);
    printf("Done.\n");
    return 0;
}

//Cliente de Vendas
