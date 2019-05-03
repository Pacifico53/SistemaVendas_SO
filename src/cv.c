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
    
	// Take an input arr2ing from user.
        readline(buf, LINE_BLOCK_SIZE);
        // Write the input on FIFO and close it
        write(fd, buf, LINE_BLOCK_SIZE);
    }
   
    close(fd);
    printf("Done.\n");
    return 0;
}

//Cliente de Vendas

