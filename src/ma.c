// MANUTENÇÃO DE ARTIGOS
// ao compilar (makefile) este ficheiro deverá gerar o ficheiro ma executável
#include <sys/wait.h>
#include <ctype.h>
#include "../include/artigo.h"

#define LINE_BLOCK_SIZE 128

void run_agredador(){
    char* args[] = {"./ag", NULL};
    int cpid, status = 0;
    if( (cpid = fork()) == 0){
        write(1, "A agregar vendas...\n", 20);
        execv(args[0], args);
    }
    else if( cpid == -1 ){
        perror("Agregador FORK ERROR");
    }
    else {
        wait(&status);
        write(1, "Done.\n", 6);
    }
}

int readline(char *buffer, int size){   //retorna os bytes lidos
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
    buffer[i] = '\0'; // making sure it's 0-terminated
    return i;
}

int isNumber(char* str){
   int i = 0, flag = 0;
   for (i = 0; str[i] && flag == 0; i++) {
       if (isdigit(str[i]) == 0) {
            flag = 1;
       }
   }

   return flag;
}

int isValidComandcmd(char* cmd0, char* cmd1, char* cmd2){
    //check if i string int é valido    atoi string retorna 0     atoi inteiro  retorna >0
    if( strcmp(cmd0,"i") == 0){
        if(cmd0 == NULL || cmd1 == NULL || cmd2 == NULL){
          return 0;
        }
        if (isNumber(cmd1) == 1 && isNumber(cmd2) == 0 && atoi(cmd2) >= 0) {
            return 1;
        }
        else {
            return 0;
        }
    }

    //check if n int string é valido
    if( strcmp(cmd0,"n") == 0){
        if(cmd0 == NULL || cmd1 == NULL || cmd2 == NULL){
            return 0;
        }
        if (isNumber(cmd1) == 0 && isNumber(cmd2) == 1 && atoi(cmd1) > 0) {
            return 2;
        }
        else {
            return 0;
        }
    }

    // check if p int int é valido
    if( strcmp(cmd0,"p") == 0){
        if(cmd0 == NULL || cmd1 == NULL || cmd2 == NULL){
            return 0;
          }
        if (isNumber(cmd1) == 0 && isNumber(cmd2) == 0 && atoi(cmd1) > 0 && atoi(cmd2) >= 0) {
            return 3;
        }
        else {
            return 0;
        }
    }

    // check if c int é válido
    if( strcmp(cmd0,"c") == 0){
        if(cmd0 == NULL || cmd1 == NULL){
            return 0;
          }
        int cmd1p = atoi(cmd1);
        if( cmd1p > 0){
            return 4;
        }
        else{
            return 0;
        }
    }

    //check if l é válido
    if(strcmp(cmd0,"l") == 0){
        if(cmd0 == NULL){
            return 0;
          }
        return 5;
    }

    // check if m é valido
    if(strcmp(cmd0,"m") == 0){
        if(cmd0 == NULL){
            return 0;
          }
        return 6;
    }

    if(strcmp(cmd0,"a") == 0){
      if(cmd0 == NULL){
        return 0;
      }
      return 7;
    }
    return 0;
}


void menuShow(){
    write(1 , "------------------MANUTENÇÃO DE ARTIGOS-----------------------\n", 65);
    write(1, " i <nome> preço           --> insere novo artigo, mostra código\n", 66);
    write(1, " n <código> <novo nome>   --> altera nome do artigo\n", 53);
    write(1, " p <código> <novo preço>  --> altera preço do artigo\n", 56);
    write(1, " c <código>               --> mostra nome + preço do artigo com esse código\n", 79);
    write(1, " l                        --> lista artigos código + nome + preço\n", 68);
    write(1, " m                        --> mostra menu\n", 42);
    write(1, " a                        --> corre agregador\n", 46);
    write(1, "--------------------------------------------------------------\n\n", 64);
}

void register_new_artigo(char* n, int p, int code){
    Artigo a = create_artigo(n, p, code);
    save_artigo(a);
    char str[64] = "";
    snprintf(str, 64, "%d\n", code);
    write(1, str, strlen(str));
    free(a);
}

void change_nome_artigo(int code, char* n){
    Artigo a = seek_artigo(code);
    if(a){
        set_nome(a, n);
        free(a);
    }
}

void change_preco_artigo(int code, int p){
    Artigo a = seek_artigo(code);
    if(a){
        set_preco(a, p);
        free(a);
    }
}

void show_artigo(int code){
    Artigo a = seek_artigo(code);
    if(a){
        print_artigo(a);
        free(a);
    }
}

void list_artigos(int currCod){
    for(int i=1; i<currCod; i++){
      show_artigo(i);
    }
}

int main(){
    int fdArtigos = open("database/ARTIGOS", O_RDONLY);
    int currCod = (lseek(fdArtigos, 0, SEEK_END) / 65) + 1;
    close(fdArtigos);
    int rl;

    while(1){
        char buf[LINE_BLOCK_SIZE];
        char *commands[3];
        char *token = NULL;
        commands[0]=NULL;
        commands[1]=NULL;
        commands[2]=NULL;
        if((rl=readline(buf,LINE_BLOCK_SIZE)) > 0 ) {
            //partir linha em token(3 tokens) pode-se fazer numa função auxiliar
            int i=0;
            token=strtok(buf," ");
            while(token != NULL && i<3){
                commands[i]=token;
                i++;
                token=strtok(NULL," ");
            }
            //imprimir 3 tokens para ver se estão corretos
            int validcmd;
            //swich options menu
            if((validcmd = isValidComandcmd(commands[0], commands[1], commands[2])) > 0 ){
                switch(validcmd){
                    case 1: register_new_artigo(commands[1], atoi(commands[2]), currCod++);
                            break;
                    case 2: change_nome_artigo(atoi(commands[1]), commands[2]);
                            break;
                    case 3: change_preco_artigo(atoi(commands[1]), atoi(commands[2]));
                            break;
                    case 4: show_artigo(atoi(commands[1]));
                            break;
                    case 5: list_artigos(currCod);
                            break;
                    case 6: menuShow();
                            break;
                    case 7: run_agredador();
                            break;
                }
            }
            else{
                perror("Invalid command type/input, please type \"m\" for all available commands.");
            }
        }
        else if (rl == -1) {
            break;
        }
        else{
            perror("No line read.");
        }
    }
    return 0;
}
