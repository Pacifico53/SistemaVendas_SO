// SERVIDOR DE VENDAS

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#define LINE_BLOCK_SIZE 128

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
        read(fd, buf, LINE_BLOCK_SIZE);

        // Print the read string and close
        printf("String: %s\n", buf);
        close(fd);
    }



	return 0;
}
