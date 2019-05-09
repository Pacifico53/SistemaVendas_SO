#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd = open("test_scripts/vendasScript", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    
    int fdArtigos = open("database/ARTIGOS", O_RDONLY);
    int posFinal = lseek(fdArtigos, 0, SEEK_END);
    int codeFinal = (posFinal / 65);
    if (codeFinal == 0) {
        printf("Erro, nao existem artigo!\nA sair...\n");
        return 1;
    }

    int i, j;

    printf("A fornecer stock inicial...\n");
    for (int i = 1; i <= codeFinal; i++) {
        char buf[64] = "";
        snprintf(buf, 64, "%d 100\n", i);
        write(fd, buf, strlen(buf));
    }
    printf("Done.\nA fazer as compras...\n");

    for(i = 0; i < 10; i++){
        for (j = 1; j <= codeFinal; j++) {
            char buf[64] = "";
            snprintf(buf, 64, "%d -10\n", j);
            write(fd, buf, strlen(buf));
        }
    }

    printf("All done\n");
    return 0;
}

