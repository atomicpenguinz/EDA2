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
		if(no->chaves[meio] == chave) {
			return meio;
		} else {
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
		if(i < no->total) {
			a->comparacoes++;
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
		no->chaves[j+1] = no->chaves[j];
		no->filhos[j+2] = no->filhos[j + 1];
	}
	no->chaves[i] = chave;
	no->filhos[i + 1] = direita;
	no->total++;
}

int transbordo(ArvoreB *arvore, NoB *no) {
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

void remove_chave_de_no(NoB *no, int index){
    for(int i=index; i < no->total - 1; i++){
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }
    no->total--;
}

NoB *busca_no_chave(NoB *no, int chave, int *index, ArvoreB *a){
    if(no==NULL) return NULL;
    int i = pesquisa_binaria(no, chave, a);

    a->comparacoes++;
    if(i < no->total && no->chaves[i] == chave){
        *index = i;
        return no;
    }
    if(no->filhos[0] == NULL) return NULL; 
    return busca_no_chave(no->filhos[i], chave, index, a);
}

void trata_subfluxo(ArvoreB *arvore, NoB *no){
    if(no == arvore->raiz || no->total >= arvore->ordem) return;

    NoB *pai = no->pai;
    int idx_filho = 0;
    while (pai->filhos[idx_filho] != no) idx_filho++;

    NoB *irmao_esq = (idx_filho > 0) ? pai->filhos[idx_filho - 1] : NULL;
    NoB *irmao_dir = (idx_filho < pai->total) ? pai->filhos[idx_filho + 1] : NULL;

    if(irmao_esq && irmao_esq->total > arvore->ordem){
        for(int i = no->total; i>0; i--) {
            no->chaves[i] = no->chaves[i - 1];
            no->filhos[i + 1] = no->filhos[i];
        }
        no->filhos[1] = no->filhos[0];

        no->chaves[0] = pai->chaves[idx_filho - 1];
        no->filhos[0] = irmao_esq->filhos[irmao_esq->total];
        if(no->filhos[0]) no->filhos[0]->pai = no;
        no->total++;

        pai->chaves[idx_filho - 1] = irmao_esq->chaves[irmao_esq->total - 1];
        irmao_esq->total--;
        return;
    }

    if(irmao_dir && irmao_dir->total > arvore->ordem){
        no->chaves[no->total] = pai->chaves[idx_filho];
        no->filhos[no->total + 1] = irmao_dir->filhos[0];
        if(no->filhos[no->total + 1]) no->filhos[no->total + 1]->pai = no;
        no->total++;

        pai->chaves[idx_filho] = irmao_dir->chaves[0];
        
        for(int i=0; i < irmao_dir->total - 1; i++) {
            irmao_dir->chaves[i] = irmao_dir->chaves[i + 1];
            irmao_dir->filhos[i] = irmao_dir->filhos[i + 1];
        }
        irmao_dir->filhos[irmao_dir->total - 1] = irmao_dir->filhos[irmao_dir->total];
        irmao_dir->total--;
        return;
    }

    if(irmao_esq){ 
        irmao_esq->chaves[irmao_esq->total] = pai->chaves[idx_filho - 1];
        irmao_esq->total++;

        for(int i=0; i < no->total; i++) {
            irmao_esq->chaves[irmao_esq->total] = no->chaves[i];
            irmao_esq->filhos[irmao_esq->total] = no->filhos[i];
            if(irmao_esq->filhos[irmao_esq->total]) irmao_esq->filhos[irmao_esq->total]->pai = irmao_esq;
            irmao_esq->total++;
        }
        irmao_esq->filhos[irmao_esq->total] = no->filhos[no->total];
        if(irmao_esq->filhos[irmao_esq->total]) irmao_esq->filhos[irmao_esq->total]->pai = irmao_esq;

        remove_chave_de_no(pai, idx_filho - 1);
        free(no->chaves); free(no->filhos); free(no);

        if(pai == arvore->raiz && pai->total == 0) {
            arvore->raiz = irmao_esq;
            irmao_esq->pai = NULL;
            free(pai->chaves); free(pai->filhos); free(pai);
        }else{
            trata_subfluxo(arvore, pai);
        }
    }else if(irmao_dir){
        no->chaves[no->total] = pai->chaves[idx_filho];
        no->total++;

        for(int i=0; i < irmao_dir->total; i++) {
            no->chaves[no->total] = irmao_dir->chaves[i];
            no->filhos[no->total] = irmao_dir->filhos[i];
            if(no->filhos[no->total]) no->filhos[no->total]->pai = no;
            no->total++;
        }
        no->filhos[no->total] = irmao_dir->filhos[irmao_dir->total];
        if(no->filhos[no->total]) no->filhos[no->total]->pai = no;

        remove_chave_de_no(pai, idx_filho);
        free(irmao_dir->chaves); free(irmao_dir->filhos); free(irmao_dir);

        if(pai == arvore->raiz && pai->total == 0) {
            arvore->raiz = no;
            no->pai = NULL;
            free(pai->chaves); free(pai->filhos); free(pai);
        }else{
            trata_subfluxo(arvore, pai);
        }
    }
}

void remover_no_b(ArvoreB *arvore, int chave){
    int index;
    NoB *no = busca_no_chave(arvore->raiz, chave, &index, arvore);
    if (!no) return; 

    if(no->filhos[0] != NULL) {
        NoB *sucessor = no->filhos[index + 1];
        while (sucessor->filhos[0] != NULL) sucessor = sucessor->filhos[0];
        
        no->chaves[index] = sucessor->chaves[0];
        no = sucessor;
        index = 0;
    }

    remove_chave_de_no(no, index);

    trata_subfluxo(arvore, no);
}

void libera_no_b(NoB *no){
    if(no){
        if(no->filhos[0] != NULL){
            for(int i=0; i <= no->total; i++){
                libera_no_b(no->filhos[i]);
            }
        }
        free(no->chaves);
        free(no->filhos);
        free(no);
    }
}

void libera_arvore_b(ArvoreB *arvore){
    if(arvore){
        libera_no_b(arvore->raiz);
        free(arvore);
    }
}
