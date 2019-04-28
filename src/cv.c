// CLIENTE DE VENDAS

#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>

#define LINE_BLOCK_SIZE 128

int readline(char* buffer, int size){	//se retornar-mos o i temos os numeros de byts lidos/até ao '\n'
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


}

int main(int argc, char* argv[]){
	char buffer[LINE_BLOCK_SIZE];
	int rl;
	int cv_sv_fifo;
	char buf[LINE_BLOCK_SIZE];
	int sv_pipe;

	//create fifo
	if( (cv_sv_fifo == mkfifo("sv_sv_fifo",0666)) > 0){
		//open fifo
		if( (sv_pipe = open("sv_sv_fifo", O_WRONLY)) == 0){
			printf("pipe opened\n");
		}else{
			perror("opening pipe");
			return 1;
		}

		while(1){
			if( (rl=readline(buffer,LINE_BLOCK_SIZE)) ){
				write(sv_pipe,buffer,rl);

			}else{
				perror("deu merda a ler");
				return 1;
			}		
		}	
	}else{
		perror("fifo creation error");
		return 0;
	}
	return 0;
}
