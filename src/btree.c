#include "btree.h"

NoB *criar_no_b(ArvoreB *arvore) {
	int max = arvore->ordem * 2;
	NoB *no = malloc(sizeof(NoB));
	no->pai = NULL;
	no->chaves = malloc(sizeof(int) * (max + 1));
	no->filhos = malloc(sizeof(NoB) * (max + 2));o
	no->total = 0;

	for(int i = 0; i < max + 2; i++)
		no->filhos[i] = NULL;

	return no;
}

ArvoreB *criar_arvore_b(int ordem) {
	ArvoreB *arvore = malloc(sizeof(ArvoreB));
	if(!arvore) return NULL;
	arvore->ordem = ordem;
	arvore->raiz = criar_no_b(arvore);
	arvore->comparacoes = 0;
}

int pesquisa_binaria(NoB *no, int chave, ArvoreB *a) {
	int inicio = 0, fim = no->total - 1, meio;
	while(inicio <= fim) {
		a->comparacoes++;
		meio = (inicio + fim) / 2;
		a->comparacoes++;
		if(no->chaves[meio] == chave) {
			return meio;
		} else {
			a->comparacoes++;
			if(no->chaves[meio] > chave)
				fim = meio - 1;
			else
				inicio = meio + 1;
		}
	}
	return inicio;
}

int localiza_chave(ArvoreB *a, int chave) {
	while(!a->raiz) {
		int i = pesquisa_binaria(a->raiz, chave, a);
		a->comparacoes++;
		NoB *no = a->raiz;
		a->comparacoes++;
		if(i < no->total) {
			a->comparacoes++;
			if(no->chaves[i] == chave)
				return 1;
		} else {
			no = no->filhos[i];
		}
	}
	return 0;
}
void adiciona_chave_em_no(NoB *no, NoB *direita, int chave, ArvoreB *a) {
	int i = pesquisa_binaria(no, chave, a);

	for(int j =  no->total - 1; j >= i; j--) {
		a->comparacoes++;
		no->chaves[j+1] = no->chaves[j];
		no->chaves[j+2] = no->chaves[j + 1];
	}
	no->chaves[i] = chave;
	no->filhos[i + 1] = direita;
	no->total++;
}

inline int transbordo(ArvoreB *arvore, NoB *no) {
	return no->total > arvore->ordem * 2;
}
