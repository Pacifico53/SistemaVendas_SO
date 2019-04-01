#include <stdio.h>
#include "artigo.h"

int main(int argc, const char *argv[])
{
    artigo a = create_artigo("nome", "descr", 200, 53);
    printf("Teste teste 123\n"); 
    print_artigo(a);
    return 0;
}
