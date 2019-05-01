// MANUTENÇÃO DE ARTIGOS
// ao compilar (makefile) este ficheiro deverá gerar o ficheiro ma executável
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/artigo.h"

#define LINE_BLOCK_SIZE 128


int readline(char *buffer, int size){   //retorna os bytes lidos
    int i = 0;
    char c;
    if(buffer == NULL || size == 0)
        return 0;
    while(read(1, &c, 1) == 1 && i < size - 1){
        if(c == '\n'){
            buffer[i] = 0;
            return i;
        }
        buffer[i++] = c;
    }
    buffer[i] = 0; // making sure it's 0-terminated
    return i;
}

int isValidComandcmd(char* cmd0, char* cmd1, char* cmd2){
    //check if i string int é valido    atoi string retorna 0     atoi inteiro  retorna >0
    if( strcmp(cmd0,"i") == 0){
        if(cmd0 == NULL || cmd1 == NULL || cmd2 == NULL){
          return 0;
        }
        int cmd1i = atoi(cmd1);
        int cmd2i = atoi(cmd2);
        if( cmd1i == 0 && cmd2i > 0){
            return 1;
        }
        else{
            return 0;
        }
    }
    //check if n int string é valido
    if( strcmp(cmd0,"n") == 0){
        if(cmd0 == NULL || cmd1 == NULL || cmd2 == NULL){
            return 0;
          }
        int cmd1n = atoi(cmd1);
        int cmd2n = atoi(cmd2);
        if( cmd1n > 0 && cmd2n == 0){
            return 2;
        }
        else{
            return 0;
        }
    }
    // check if p int int é valido
    if( strcmp(cmd0,"p") == 0){
        if(cmd0 == NULL || cmd1 == NULL || cmd2 == NULL){
            return 0;
          }
        int cmd1p = atoi(cmd1);
        int cmd2p = atoi(cmd2);
        if( cmd1p > 0 && cmd2p > 0){
            return 3;
        }
        else{
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
    return 0;
}


void menuShow(){
    printf("------------------MANUTENÇÃO DE ARTIGOS-----------------------\n");
    printf(" i <nome> preço           --> insere novo artigo, mostra código\n");
    printf(" n <código> <novo nome>   --> altera nome do artigo\n");
    printf(" p <código> <novo preço>  --> altera preço do artigo\n" );
    printf(" c <código>               --> mostra nome + preço do artigo com esse código\n");
    printf(" l                        --> lista artigos código + nome + preço\n");
    printf(" m                        --> mostra menu\n");
    printf("--------------------------------------------------------------\n");
    printf("\n");
}

void register_new_artigo(char* n, int p, int code){
    Artigo a = create_artigo(n, p, code);
    save_artigo(a);
    printf("codigo = %d\n",code);
    free(a);
}

void change_nome_artigo(int code, char* n){
    Artigo a = seek_artigo(code);
    if(a){
        set_nome(a, n);
        free(a);
    }
}

void change_preco_artigo(int code, float p){
    Artigo a = seek_artigo(code);
    if(a){
        set_preco(a, p);
        free(a);
    }
}

void show_artigo(int code){
    Artigo a = seek_artigo(code);
    //print_artigo(a); ----> seek_artigo já dá print(para testar)
    free(a);
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
    menuShow();
    int rl;


    while(1){
        char buf[LINE_BLOCK_SIZE];
        char *commands[3];
        char *token = NULL;
        commands[0]=NULL;
        commands[1]=NULL;
        commands[2]=NULL;
        if((rl=readline(buf,LINE_BLOCK_SIZE)) > 0 ) {
            //char *all = strtok(buf,"\n");
            //printf("Your string2: %s\n", all);
            //printf("%d\n",strlen(all));
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
                printf("COMANDO VALIDO\n");
                switch(validcmd){
                    case 1: register_new_artigo(commands[1], strtof(commands[2], NULL), currCod++);
                            break;
                    case 2: change_nome_artigo(atoi(commands[1]), commands[2]);
                            break;
                    case 3: change_preco_artigo(atoi(commands[1]), strtof(commands[2], NULL));
                            break;
                    case 4: show_artigo(atoi(commands[1]));
                            break;
                    case 5: list_artigos(currCod);
                            break;
                    case 6: menuShow();
                            break;
                    default: printf("-----------------------\n");
                }
            }
            else{
                perror("Invalid command type/input, please insert one of the commands listed     use m for MENU");
            }
        }
        else{
            perror("No line readed");
        }
    }
    return 0;
}

