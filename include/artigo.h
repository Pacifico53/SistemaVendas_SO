#ifndef __ARTIGO_H__
#define __ARTIGO_H__
typedef struct str_artigo* Artigo;

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

Artigo create_artigo(char* n, float p, int code);
Artigo seek_artigo(int codigo);
void set_nome(Artigo a, char* n);
void set_preco(Artigo a, float p);
int get_index_nome(Artigo a);
char* get_nome(Artigo a);
float get_preco(Artigo a);
int get_code(Artigo a);
void print_artigo(Artigo a);
void save_artigo(Artigo a);
int save_name(char* name);
#endif

