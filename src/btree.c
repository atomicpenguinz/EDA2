#include "btree.h"

NoB *criar_no_b(ArvoreB *arvore) {
	int max = arvore->ordem * 2;
	NoB *no = malloc(sizeof(NoB));
	no->pai = NULL;
	no->chaves = malloc(sizeof(int) * (max + 1));
	no->filhos = malloc(sizeof(NoB*) * (max + 2));
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
	return arvore;
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
	NoB *no = a->raiz;
	while(no) {
		int i = pesquisa_binaria(no, chave, a);
		// a->comparacoes++; // analisar se faz sentido contabilizar essas comparações
		if(i < no->total) {
			// a->comparacoes++;
			if(no->chaves[i] == chave)
				return 1;
		}
		no = no->filhos[i];
	}
	return 0;
}

NoB *localiza_no(ArvoreB *a, int chave) {
	NoB *no = a->raiz;
	while(no) {
		int i = pesquisa_binaria(no, chave, a);
		// a->comparacoes++; // analisar se faz sentido contabilizar essas comparações
		if(no->filhos[i] == NULL)
			return no;
		else
			no = no->filhos[i];
	}
	return NULL;
}
void adiciona_chave_em_no(NoB *no, NoB *direita, int chave, ArvoreB *a) {
	int i = pesquisa_binaria(no, chave, a);

	for(int j =  no->total - 1; j >= i; j--) {
		// a->comparacoes++;
		no->chaves[j+1] = no->chaves[j];
		no->filhos[j+2] = no->filhos[j + 1];
	}
	// a->comparacoes++; // última do for()
	no->chaves[i] = chave;
	no->filhos[i + 1] = direita;
	no->total++;
}

inline int transbordo(ArvoreB *arvore, NoB *no) {
	arvore->comparacoes++;
	return no->total > arvore->ordem * 2;
}

NoB *split(ArvoreB *arvore, NoB *no) {
	int meio = no->total / 2;
	NoB *novo = criar_no_b(arvore);
	novo->pai = no->pai;
	for(int i = meio + 1; i < no->total; i++) {
		novo->filhos[novo->total] = no->filhos[i];
		novo->chaves[novo->total] = no->chaves[i];
		if(novo->filhos[novo->total] != NULL)
			novo->filhos[novo->total]->pai = novo;

		novo->total++;
	}

	novo->filhos[novo->total] = no->filhos[no->total];
	if(novo->filhos[novo->total] != NULL)
		novo->filhos[novo->total]->pai = novo;
	no->total = meio;
	return novo;
}

void adiciona_chave_aux(ArvoreB *arvore, NoB *no, NoB *novo, int chave) {
	adiciona_chave_em_no(no, novo, chave, arvore);
	if(transbordo(arvore, no)) {
		int promovido = no->chaves[arvore->ordem];
		NoB *novo = split(arvore, no);

		if(no->pai == NULL) {
			NoB *raiz = criar_no_b(arvore);
			raiz->filhos[0] = no;
			adiciona_chave_em_no(raiz, novo, promovido, arvore);

			no->pai = raiz;
			novo->pai = raiz;
			arvore->raiz = raiz;
		} else
			adiciona_chave_aux(arvore, no->pai, novo, promovido);
	}
}

void inserir_no_b(ArvoreB *arvore, int chave) {
	NoB *no = localiza_no(arvore, chave);
	adiciona_chave_aux(arvore, no, NULL, chave);
}

