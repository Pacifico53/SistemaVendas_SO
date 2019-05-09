//AGREGADOR

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

char* agregate(int code){
    int fd = open("database/VENDAS", O_RDONLY);
    char result[32] = "";
    char buf[64] = "";
    int stock = 0, money = 0;

    while((read(fd, buf, 64)) > 0){
        char *str = strdup(buf);
        char *token = strtok(str, " ");
        char *info[3];
        info[0] = NULL;
        info[1] = NULL;
        info[2] = NULL;
        int i = 0;

        while (token && i<4) {
            info[i++] = strdup(token);
            token = strtok(NULL, " ");
        }

        if (atoi(info[0]) == code) {
            stock += atoi(info[1]);
            money += atoi(info[2]);
        }
    }
    snprintf(result, 32, "%d %d %d\n", code, stock, money);
    close(fd);
    return strdup(result);
}

int main(){
    time_t currtime;
    char strTime[21] = "";
    char file[64] = "";
    currtime = time(NULL);
    struct tm *localTime;

    localTime = localtime(&currtime);
    strftime (strTime, 21, "%Y-%m-%dT%H:%M:%S", localTime);
    snprintf(file, 64, "database/%s", strTime);

    int fdFinal = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);

    int fdArtigos = open("database/ARTIGOS", O_RDONLY);
    int posFinal = lseek(fdArtigos, 0, SEEK_END);
    int codeFinal = (posFinal / 65);
    int i = 0, status = 0;
    pid_t pid;
    for(i = 1; i <= codeFinal; i++){
        if ((pid = fork()) == 0) {
            char* res = agregate(i);
            write(fdFinal, res, strlen(res));
            _exit(0);
        }
    }

    for(i = 1; i <= codeFinal; i++){
        wait(&status);
    }

    return 0;
}

