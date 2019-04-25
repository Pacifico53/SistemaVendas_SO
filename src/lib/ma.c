// MANUTENÇÃO DE ARTIGOS

// ao compilar (makefile) este ficheiro deverá gerar o ficheiro ma executável
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LINE_BLOCK_SIZE 128


int readline(char *buffer, size_t size){

    size_t cnt = 0;
    char c;
    if(buffer == NULL || size == 0)
        return 0;

    while(read(STDIN_FILENO, &c, 1) == 1 && cnt < size - 1){
        if(c == '\n'){
            buffer[cnt] = 0;
            return 1;
        }

        buffer[cnt++] = c;
    }

    buffer[cnt] = 0; // making sure it's 0-terminated
    return 1;
}

int isValidComand(char* cmd0, char* cmd1, char*cmd2){
    if(strcmp(cmd0,"i") == 0 || strcmp(cmd0,"n") == 0 || strcmp(cmd0,"p") == 0){
      int f1=1,f2=1;
      //check if cmd1 is a string
      for(int i=0; cmd1[i] != '\0'; i++){
        if(!isalpha(cmd1[i])){
          f1 = 0;
        }
      }
      //check if cmd2 is a number
      for(int j=0; cmd2[j] != '\0'; j++){
        if(!isdigit(cmd2[j])){
          f2 = 0;
        }
      }
      return (f1&&f2);
    }else{
      return 0;
    }
}


/*
  char** readln(char *buf,char* front){
	char** buff=malloc(LINE_BLOCK_SIZE*sizeof(char*));
	int i=0;
	char* token;
	token = strtok(buf, front);

	while( token != NULL ) {
	  buff[i]=malloc(strlen(token)*sizeof(char));
	  strcpy(buff[i],token);
	  buff[i][strlen(token)]=0;
	  i++;
	  token = strtok(NULL, front);
   }
	buff[i]=NULL;
	return buff;
}
*/


void menuShow(){
	printf("-----------------MANUTENÇÃO DE ARTIGOS-----------------------\n");
	printf(" i <nome> preço	          --> insere novo artigo, mostra código\n");
	printf(" n <código> <novo nome>   --> altera nome do artigo\n");
  printf(" p <código> <novo preço>  --> altera preço do artigo\n" );
	printf(" outros comandos a adicionar\n");
  printf("\n");
}

int main(){

	int fdArtigos;
	int fdStrings;
	//abrir ficheiros artigos, strings e controlo e erros;
	if( (fdArtigos=open("artigos.txt", O_RDWR|O_CREAT,0666)) == -1){
		perror(" ERROR : opening artigos.txt");
		return 1;
	}
	if( (fdStrings=open("strings.txt", O_RDWR|O_CREAT,0666)) == -1){
		perror(" ERROR : opening strings.txt");
		return 1;
  }

	menuShow();
	int rr;
  int rl;
  char buf[LINE_BLOCK_SIZE];

	while(1){

	   if( (rl=readline(buf,LINE_BLOCK_SIZE)) > 0 ) {
       printf("Linha lida : %s  Tamanho linha : %i bytes\n", buf, strlen(buf));
		       //char *all = strtok(buf,"\n");
		      //printf("Your string2: %s\n", all);
		       //printf("%d\n",strlen(all));


      //partir linha em token(3 tokens)	pode-se fazer numa função auxiliar
      int i=0;
      char *token;
      char *comands[3];
      token=strtok(buf," ");
		    while(token != NULL && i < 3){
          comands[i] = token;
			    i++;
          token=strtok(NULL," ");
		    }
      //imprimir 3 tokens para ver se estão corretos
			printf("Token0: %s  Token1: %s  Token2: %s\n", comands[0], comands[1], comands[2]);
      printf("\n");


		//swich options menu
    if(isValidComand( comands[0], comands[1], comands[2]) ){
      printf("Comando válido\n");

    }else{
      perror("Invalid comand type/input, please insert one of the commands listed");
      menuShow();
    }





    }else{
			perror("No line readed");
		}

	}

	return 0;
}
