#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
	struct nodo *pai;	
	struct nodo *esquerda;
	struct nodo *direita;
	int chave;
} NoAVL;

typedef struct arvore {
	struct nodo *raiz;
} ArvoreAVL;

