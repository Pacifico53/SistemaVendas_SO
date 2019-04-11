#ifndef __ARTIGO_H__
#define __ARTIGO_H__
typedef struct str_artigo* Artigo;

Artigo create_artigo(char* n, char* d, int p, int code);
void set_nome(Artigo a, char* n);
void set_descr(Artigo a, char* d);
void set_preco(Artigo a, int p);
char* get_nome(Artigo a);
char* get_descr(Artigo a);
int get_preco(Artigo a);
int get_code(Artigo a);
void print_artigo(Artigo a);
#endif

