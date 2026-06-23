#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo {
	struct nodo *pai;	
	struct nodo *esquerda;
	struct nodo *direita;
	int chave;
	int altura;
} NoAVL;

typedef struct arvore {
	long comparacoes;
	struct nodo *raiz;
} ArvoreAVL;

