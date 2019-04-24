#include <stdio.h>
#include "../include/artigo.h"

int main(){
    Artigo a = create_artigo(113, 500, 2);
    printf("Teste teste 123\n");
    save_artigo(a);
    print_artigo(a);
    return 0;
}

