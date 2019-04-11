#include <stdio.h>
#include "../include/artigo.h"

int main()
{
    Artigo a = create_artigo("nome", "descr", 200, 53);
    printf("Teste teste 123\n");
    print_artigo(a);
    return 0;
}

