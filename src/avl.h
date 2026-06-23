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
ArvoreAVL *criar_arvore(void);
NoAVL *criar_nodo(NoAVL *pai, int chave);
int altura(NoAVL *no);
void att_altura(NoAVL *no);
int fb(NoAVL *no);
NoAVL *rse(ArvoreAVL *arvore, NoAVL *no);
NoAVL *rsd(ArvoreAVL *arvore, NoAVL *no);
NoAVL *rde(ArvoreAVL *arvore, NoAVL *no);
NoAVL *rdd(ArvoreAVL *arvore, NoAVL *no);
NoAVL *adicionar_no(ArvoreAVL *arvore, NoAVL *no, int chave);
void balanceamento(ArvoreAVL *arvore, NoAVL *no);
NoAVL *inserir(ArvoreAVL *arvore, int valor);
NoAVL *remover(ArvoreAVL *arvore, int valor);
