#include "header.h"

ArvoreAVL *criar_arvore() {
	ArvoreAVL *arvore = malloc(sizeof(ArvoreAVL));
	arvore->raiz = NULL;
	return arvore;
}

Nodo *criar_nodo(Nodo *pai, int chave) {
	No* no = malloc(sizeof(No));
    no->chave = chave;
    no->pai = pai;
    no->esquerda = NULL;
    no->direita = NULL;

    return no;	
}

int altura(Nodo* no){
	int esquerda, direita = 0;

	if(no->esquerda)
		esquerda = altura(no->esquerda) + 1;
	if(no->direita)
		direita = altura(no->direita) + 1;

	return (esquerda > direita) ? esquerda : direita;
}

int fb(Nodo *no) {
	int esquerda = 0, direita = 0;
	
	if(no->esquerda)
		esquerda = altura(no->esquerda) + 1;
	if(no->direita)
		direita = altura(no->direita) + 1;	

	return (esquerda - direita);
}

Nodo *rse(Nodo *no) {
	No *pai = no->pai;
	No *direita = no->direita;

	no->direita = direita->esquerda;
	no->pai = direita;

	direita->esquerda = no;
	direita->pai = pai;

	return direita;
}

Nodo *rsd(Nodo *no) {
	No *pai = no->pai;
	No *esquerda = no->esquerda;

	no->direita = esquerda->direita;
	no->pai = esquerda;

	esquerda->direita = no;
	esquerda->pai = pai;

	return esquerda;
}

Nodo *rde(Nodo* no) {
	no->direita = rsd(no->direita);
	return rse(no);
}

Nodo *rds(Nodo* no) {
	no->esquerda = rsd(no->esquerda);
	return rsd(no);
}

Nodo *adicionar_no(Nodo *no, int chave) {
	if(chave > no->chave) {
		if(!no->direita) {
			
		}
	}
}
