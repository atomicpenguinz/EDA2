#include "header.h"

ArvoreAVL *criar_arvore() {
	ArvoreAVL *arvore = malloc(sizeof(ArvoreAVL));
	arvore->raiz = NULL;
	return arvore;
}

NoAVL *criar_nodo(NoAVL *pai, int chave) {
	NoAVL *no = malloc(sizeof(NoAVL));
    no->chave = chave;
    no->pai = pai;
    no->esquerda = NULL;
    no->direita = NULL;

    return no;	
}

int altura(NoAVL* no){
	int esquerda, direita = 0;

	if(no->esquerda)
		esquerda = altura(no->esquerda) + 1;
	if(no->direita)
		direita = altura(no->direita) + 1;

	return (esquerda > direita) ? esquerda : direita;
}

int fb(NoAVL *no) {
	int esquerda = 0, direita = 0;
	
	if(no->esquerda)
		esquerda = altura(no->esquerda) + 1;
	if(no->direita)
		direita = altura(no->direita) + 1;	

	return (esquerda - direita);
}

NoAVL *rse(NoAVL *no) {
	NoAVL *pai = no->pai;
	NoAVL *direita = no->direita;

	no->direita = direita->esquerda;
	no->pai = direita;

	direita->esquerda = no;
	direita->pai = pai;

	return direita;
}

NoAVL *rsd(NoAVL *no) {
	NoAVL *pai = no->pai;
	NoAVL *esquerda = no->esquerda;

	no->direita = esquerda->direita;
	no->pai = esquerda;

	esquerda->direita = no;
	esquerda->pai = pai;

	return esquerda;
}

NoAVL *rde(NoAVL* no) {
	no->direita = rsd(no->direita);
	return rse(no);
}

NoAVL *rds(NoAVL* no) {
	no->esquerda = rsd(no->esquerda);
	return rsd(no);
}

NoAVL *adicionar_no(NoAVL *no, int chave, int *comparacoes) {
	(*comparacoes)++;
	if(chave > no->chave) {
		(*comparacoes)++;
		if(!no->direita) {
			NoAVL *novo = criar_nodo(no, chave);
			no->direita = novo;
			return novo;
		} else
			return adicionar_no(no->direita, chave, comparacoes);
	} else {
		(*comparacoes)++;
		if(!no->esquerda) {
			NoAVL *novo = criar_nodo(no, chave);
			no->esquerda= novo;
			return novo;
		} else
			return adicionar_no(no->esquerda, chave, comparacoes);
	}
}
