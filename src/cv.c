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
	while(1){
		if( (rl=readline(buffer,LINE_BLOCK_SIZE)) ){
			write(1,buffer,rl);

		}else{
			perror("deu merda a ler");
			return 1;
		}		
	}	
	return 0;
}
