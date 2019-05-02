#include <stdio.h>
#include "../include/artigo.h"
#include <stdlib.h>
#include <ctype.h>

int main(){
    int i = 1;
    Artigo a = create_artigo("Televisao Grande", 1259, i++);
    Artigo b = create_artigo("Playstation", 399, i++);
    Artigo c = create_artigo("XBox 360", 500.23, i++);
    print_artigo(a);
    print_artigo(b);
    print_artigo(c);
    save_artigo(a);
    save_artigo(b);
    save_artigo(c);
    change_stock(a, 10);
    printf("teste %d\n", get_stock(a));
    if (isdigit('1')!=0) {
       printf("ye\n");
    }


    return 0;
}

