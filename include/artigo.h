#ifndef __ARTIGO_H__
#define __ARTIGO_H__
typedef struct str_artigo* artigo;

artigo create_artigo(char* n, char* d, int p, int code); 
void set_nome(artigo a, char* n);
void set_descr(artigo a, char* d);
void set_preco(artigo a, int p);
char* get_nome(artigo a);
char* get_descr(artigo a);
int get_preco(artigo a);
int get_code(artigo a);
void print_artigo(artigo a);
#endif
