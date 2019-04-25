#ifndef __ARTIGO_H__
#define __ARTIGO_H__
typedef struct str_artigo* Artigo;

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

Artigo create_artigo(char* n, int p, int code);
Artigo fetch_artigo(int n, int p, int code);
void set_nome(Artigo a, int n);
void set_preco(Artigo a, int p);
int get_index_nome(Artigo a);
char* get_nome(Artigo a);
int get_preco(Artigo a);
int get_code(Artigo a);
void print_artigo(Artigo a);
void save_artigo(Artigo a);
int save_name(char* name);
void seek_artigo(int codigo);
#endif

