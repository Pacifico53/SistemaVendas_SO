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
    char c;
    int i=0;

    if(buffer == NULL || size == 0)
        return 0;

    while( read(0,&c,1) == 1 && i<size-1 ){
        if( c == '\n'){
            char pidFIFO[32] = "";
            snprintf(pidFIFO, 32, " database/fifo%d", getpid());
            strcat(buffer, pidFIFO);
            return i;
        }
        buffer[i++]=c;
    }
    buffer[i]=0;
    return i;
}

void sigcont_handler(int sig){
    printf("Acordei %d\n", sig);
}

int main(){
    // FIFO file path
    char *serverFIFO = "database/serverFIFO";
    int fdSERVER, fdPIDFIFO;
    char pidFIFO[64] = "";
    snprintf(pidFIFO, 64, "database/fifo%d", getpid());

    signal(SIGCONT, sigcont_handler);
    
    mkfifo(pidFIFO, 0777);
    mkfifo(serverFIFO, 0777);

    while (1){
        char buf[LINE_BLOCK_SIZE];
        char res[LINE_BLOCK_SIZE];
        fdSERVER = open(serverFIFO, O_WRONLY);

        // Take an input from user.
        readline(buf, LINE_BLOCK_SIZE);

        // Write the input on FIFO and close it
        write(fdSERVER, buf, LINE_BLOCK_SIZE);
        close(fdSERVER);
        fdPIDFIFO = open(pidFIFO, O_RDONLY);
        pause();


        read(fdPIDFIFO, res, LINE_BLOCK_SIZE);
        printf("%s", res);
        close(fdPIDFIFO);
        snprintf(buf, LINE_BLOCK_SIZE, " ");
        snprintf(res, LINE_BLOCK_SIZE, " ");
    }

    printf("Done.\n");
    return 0;
}

