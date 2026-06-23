#include "avl.h"

ArvoreAVL *criar_arvore() {
	ArvoreAVL *arvore = malloc(sizeof(ArvoreAVL));
	if(!arvore) return NULL;

	arvore->raiz = NULL;
	arvore->comparacoes = 0;
	return arvore;
}

NoAVL *criar_nodo(NoAVL *pai, int chave) {
	NoAVL *no = malloc(sizeof(NoAVL));
	if(!no) return NULL;
    no->chave = chave;
    no->pai = pai;
    no->esquerda = NULL;
    no->direita = NULL;
	no->altura = 1;
    return no;	
}
static inline int max(int x, int y) { return x > y ? x : y;}

int altura(NoAVL* no){
	return no != NULL ? no->altura : 0;
}

void att_altura(NoAVL *no) {
	no->altura = 1 + max(altura(no->esquerda), altura(no->direita));
}

int fb(NoAVL *no) {
	return altura(no->esquerda) - altura(no->direita);
}

NoAVL *rse(ArvoreAVL *arvore, NoAVL *no) {
	NoAVL *pai = no->pai;
	NoAVL *direita = no->direita;
	
	// arvore->comparacoes++;
	if(direita->esquerda)
		direita->esquerda->pai = no;

	no->direita = direita->esquerda;
	no->pai = direita;
	direita->esquerda = no;
	direita->pai = pai;

	// arvore->comparacoes++;
	if(!pai)
		arvore->raiz = direita;
	else {
		arvore->comparacoes++;
		if(pai->esquerda == no)
			pai->esquerda = direita;
		else
			pai->direita = direita;
	}
	att_altura(no);
	att_altura(direita);
	return direita;
}

NoAVL *rsd(ArvoreAVL *arvore, NoAVL *no) {
	NoAVL *pai = no->pai;
	NoAVL *esquerda = no->esquerda;

	// arvore->comparacoes++;
	if(esquerda->direita != NULL)
		esquerda->direita->pai = no;

	no->esquerda = esquerda->direita;
	no->pai = esquerda;

	// arvore->comparacoes++;
	if(!pai)
		arvore->raiz = esquerda;
	else {
		arvore->comparacoes++;
		if(pai->esquerda == no)
			pai->esquerda = esquerda;
		else
			pai->direita = esquerda;
	}
	esquerda->direita = no;
	esquerda->pai = pai;
	att_altura(no);
	att_altura(esquerda);
	return esquerda;
}

NoAVL *rde(ArvoreAVL *arvore, NoAVL* no) {
	no->direita = rsd(arvore, no->direita);
	return rse(arvore, no);
}

NoAVL *rdd(ArvoreAVL *arvore, NoAVL* no) {
	no->esquerda = rse(arvore, no->esquerda);
	return rsd(arvore, no);
}

NoAVL *adicionar_no(ArvoreAVL *arvore, NoAVL *no, int chave) {
	arvore->comparacoes++;
	if(chave > no->chave) {
		// arvore->comparacoes++;
		if(!no->direita) {
			no->direita = criar_nodo(no, chave);
		} else {
			no->direita = adicionar_no(arvore, no->direita, chave);
		}
	} else {
		// arvore->comparacoes++;
		if(!no->esquerda) {
			no->esquerda = criar_nodo(no, chave);
		} else {
			no->esquerda = adicionar_no(arvore, no->esquerda, chave);
		}
	}
	att_altura(no);
	return no;
}

void balanceamento(ArvoreAVL *arvore, NoAVL *no) {
	for(; no != NULL; no = no->pai) {
		int fator = fb(no);

		arvore->comparacoes++;
		if(fator > 1) {
			arvore->comparacoes++;
			if(fb(no->esquerda) > 0) {
				rsd(arvore, no);
			} else {
				rdd(arvore, no);
			}
		} else {
			arvore->comparacoes++;
			if(fator < -1) {
				arvore->comparacoes++;
				if(fb(no->direita) < 0) {
					rse(arvore, no);
				} else {
					rde(arvore, no);
				}
			}
		}
	}
}
#define VAZIA(arvore) (arvore->raiz == NULL)

NoAVL *inserir(ArvoreAVL *arvore, int valor) {
	arvore->comparacoes++;
	if(VAZIA(arvore)) {
		NoAVL *novo = criar_nodo(NULL, valor);
		arvore->raiz = novo;
		return novo;
	}
	NoAVL *no = adicionar_no(arvore, arvore->raiz, valor);
	balanceamento(arvore, no);
	NoAVL *aux = no;
	return no;
}

NoAVL *remover(ArvoreAVL *arvore, int valor) {
	
}
