#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    int fd = open("test_scripts/insertScript", O_WRONLY | O_TRUNC | O_CREAT, 0666);
    printf("Numero de artigos a inserir:\n");
    int n = 0;
    scanf("%d",&n);
    for (int i = 1; i <= n; i++) {
        char buf[64] = "";
        snprintf(buf, 64, "i A%d %d\n", i, i);
        write(fd, buf, strlen(buf));
    }
    write(fd, "\n", 1); //aaa
    printf("All done\n");
    return 0;
}

