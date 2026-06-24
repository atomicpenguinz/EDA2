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
	esquerda->direita = no;
	esquerda->pai = pai;

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
	if(!no) return NULL;

	arvore->comparacoes++;
	if(chave > no->chave) {
		// arvore->comparacoes++;
		if(!no->direita) {
			no->direita = criar_nodo(no, chave);
			att_altura(no);
			return no->direita;
		} else {
			NoAVL *novo = adicionar_no(arvore, no->direita, chave);
			att_altura(no);
			return novo;
		}
	} else {
		if(!no->esquerda) {
			no->esquerda = criar_nodo(no, chave);
			att_altura(no);
			return no->esquerda;
		} else {
			NoAVL *novo = adicionar_no(arvore, no->esquerda, chave);
			att_altura(no);
			return novo;
			}
	}
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
	if(no)
		balanceamento(arvore, no);
	return no;
}

NoAVL* localizar(ArvoreAVL *arvore, NoAVL* no, int valor) {
	if(!no) return NULL;

	arvore->comparacoes++;
    if (no->chave == valor) {
        return no;
    } else {
		arvore->comparacoes++;
        if(valor < no->chave)
            return localizar(arvore, no->esquerda, valor);
        else
            return localizar(arvore, no->direita, valor);
    }
    return NULL;
}

static NoAVL *menor_no(NoAVL *no) {
	if(!no) return NULL;
	NoAVL *aux = no;
	while(aux->esquerda)
		aux = aux->esquerda;
	return aux;
}

static NoAVL *remover_aux(NoAVL *raiz, int chave, ArvoreAVL *arvore) {
	if(!raiz) return NULL;

	arvore->comparacoes++;
	if(chave < raiz->chave)
		raiz->esquerda= remover_aux(raiz->esquerda, chave, arvore);
	else if (chave > raiz->chave) {
		arvore->comparacoes++;
		raiz->direita = remover_aux(raiz->direita, chave, arvore);
	} else {
		arvore->comparacoes++;
		if(!raiz->esquerda || !raiz->direita) {
			NoAVL *aux = raiz->esquerda ? raiz->esquerda : raiz->direita;
			if(!aux) {
				aux = raiz;
				raiz = NULL;
			} else {
				NoAVL *pai = raiz->pai;
				*raiz = *aux;
				raiz->pai = pai;
				if(raiz->esquerda)
					raiz->esquerda->pai = raiz;
				if(raiz->direita)
					raiz->direita->pai = raiz;
			}
			free(aux);
		} else {
			NoAVL *aux = menor_no(raiz->direita);
			raiz->chave = aux->chave;
			raiz->direita = remover_aux(raiz->direita, aux->chave, arvore);
		}
	}
	if(!raiz) return NULL;

	raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));
	int fb_raiz = fb(raiz);
	int fb_esq = fb(raiz->esquerda);

	arvore->comparacoes++;
	if(fb_raiz > 1 ) {
		arvore->comparacoes++;
		if (fb_esq >= 0)
			return rsd(arvore, raiz);
		else {
			raiz->esquerda = rse(arvore, raiz->esquerda);
			return rsd(arvore, raiz);
		}
	}

	arvore->comparacoes++;
	int fb_dir = fb(raiz->direita); 
	if(fb_raiz < -1 ) {
		arvore->comparacoes++;
		if(fb_dir <= 0)
			return rse(arvore, raiz);
		else {
			raiz->direita = rsd(arvore, raiz->direita);
			return rse(arvore, raiz);
		}
	}
	return raiz;
}

NoAVL *remover(ArvoreAVL *arvore, int chave) {
	return (!arvore || !arvore->raiz) ?
		NULL :
		remover_aux(arvore->raiz, chave, arvore);
}
