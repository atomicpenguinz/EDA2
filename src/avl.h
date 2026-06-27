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

/* avl.c */
ArvoreAVL *criar_arvore_avl(void);
NoAVL *criar_nodo_avl(NoAVL *pai, int chave);
void free_avl(ArvoreAVL *arvore);
NoAVL *inserir_no_avl(ArvoreAVL *arvore, int valor);
NoAVL *remover_no_avl(ArvoreAVL *arvore, int chave);
