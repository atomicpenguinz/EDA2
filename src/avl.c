#include "avl.h"

ArvoreAVL *criar_arvore_avl() {
    ArvoreAVL *arvore = malloc(sizeof(ArvoreAVL));
    if(!arvore) return NULL;

    arvore->raiz = NULL;
    arvore->comparacoes = 0;
    return arvore;
}

NoAVL *criar_nodo_avl(NoAVL *pai, int chave) {
    NoAVL *no = malloc(sizeof(NoAVL));
    if(!no) return NULL;
    no->chave = chave;
    no->pai = pai;
    no->esquerda = NULL;
    no->direita = NULL;
    no->altura = 1;
    return no;
}

static void percorrer(NoAVL* no, void (*callback)(void*)) {
    if (no != NULL) {
        percorrer(no->esquerda,callback);
        percorrer(no->direita,callback);
        callback(no);
    }
}

void free_avl(ArvoreAVL *arvore) {
    if(!arvore) return;
    percorrer(arvore->raiz, free);
    free(arvore);
}
static inline int max(int x, int y) {
    return x > y ? x : y;
}

static int altura(NoAVL* no) {
    return no != NULL ? no->altura : 0;
}

static void att_altura(ArvoreAVL *arvore, NoAVL *no) {
    no->altura = 1 + max(altura(no->esquerda), altura(no->direita));
}

static int fb(NoAVL *no) {
    if(!no) return 0;
    return altura(no->esquerda) - altura(no->direita);
}

static NoAVL *rse(ArvoreAVL *arvore, NoAVL *no) {
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
    att_altura(arvore, no);
    att_altura(arvore, direita);
    return direita;
}

static NoAVL *rsd(ArvoreAVL *arvore, NoAVL *no) {
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
    att_altura(arvore, no);
    att_altura(arvore, esquerda);
    return esquerda;
}

static NoAVL *rde(ArvoreAVL *arvore, NoAVL* no) {
    no->direita = rsd(arvore, no->direita);
    return rse(arvore, no);
}

static NoAVL *rdd(ArvoreAVL *arvore, NoAVL* no) {
    no->esquerda = rse(arvore, no->esquerda);
    return rsd(arvore, no);
}

static NoAVL *adicionar_no_avl(ArvoreAVL *arvore, NoAVL *no, int chave) {
    if(!no) return NULL;

    arvore->comparacoes++;
    if(chave > no->chave) {
        // arvore->comparacoes++;
        if(!no->direita) {
            no->direita = criar_nodo_avl(no, chave);
            att_altura(arvore, no);
            return no->direita;
        } else {
            NoAVL *novo = adicionar_no_avl(arvore, no->direita, chave);
            att_altura(arvore, no);
            return novo;
        }
    } else {
        if(!no->esquerda) {
            no->esquerda = criar_nodo_avl(no, chave);
            att_altura(arvore, no);
            return no->esquerda;
        } else {
            NoAVL *novo = adicionar_no_avl(arvore, no->esquerda, chave);
            att_altura(arvore, no);
            return novo;
        }
    }
}

static void balanceamento_avl(ArvoreAVL *arvore, NoAVL *no) {
    for(; no != NULL; no = no->pai) {
        int fator = fb(no);

        if(fator > 1) {
            if(fb(no->esquerda) > 0) {
                rsd(arvore, no);
            } else {
                rdd(arvore, no);
            }
        } else {
            if(fator < -1) {
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

NoAVL *inserir_no_avl(ArvoreAVL *arvore, int valor) {
    if(VAZIA(arvore)) {
        NoAVL *novo = criar_nodo_avl(NULL, valor);
        arvore->raiz = novo;
        return novo;
    }
    NoAVL *no = adicionar_no_avl(arvore, arvore->raiz, valor);
    if(no)
        balanceamento_avl(arvore, no);
    return no;
}

static NoAVL* localizar_no_avl(ArvoreAVL *arvore, NoAVL* no, int valor) {
    if(!no) return NULL;

    arvore->comparacoes++;
    if (no->chave == valor) {
        return no;
    } else {
        arvore->comparacoes++;
        if(valor < no->chave)
            return localizar_no_avl(arvore, no->esquerda, valor);
        else
            return localizar_no_avl(arvore, no->direita, valor);
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
    else {
        arvore->comparacoes++;
        if (chave > raiz->chave) {
            raiz->direita = remover_aux(raiz->direita, chave, arvore);
        } else {
            if(!raiz->esquerda || !raiz->direita) {
                NoAVL *aux = raiz->esquerda ? raiz->esquerda : raiz->direita;
                if(!aux) {
                    if(raiz->pai) {
                        if(raiz->pai->esquerda == raiz)
                            raiz->pai->esquerda = aux;
                        else
                            raiz->pai->direita = aux;
                    }
                    free(raiz);
                    raiz = NULL;
                } else {
                    aux->pai = raiz->pai;
                    if(raiz->pai) {
                        if(raiz->pai->esquerda == raiz)
                            raiz->pai->esquerda = aux;
                        else
                            raiz->pai->direita = aux;
                    }
                free(raiz);
                raiz = aux;
                }
            } else {
                NoAVL *aux = menor_no(raiz->direita);
                raiz->chave = aux->chave;
                raiz->direita = remover_aux(raiz->direita, aux->chave, arvore);
            }
        }
    }
    if(!raiz) return NULL;

    att_altura(arvore, raiz); 
    if(raiz->esquerda) raiz->esquerda->pai = raiz;
    if(raiz->direita) raiz->direita->pai = raiz;

    int fb_raiz = fb(raiz);
    int fb_esq = raiz->esquerda ? fb(raiz->esquerda) : 0;
    int fb_dir = raiz->direita ? fb(raiz->direita) : 0;

    if(fb_raiz > 1) {
        if (fb_esq >= 0)
            return rsd(arvore, raiz);
        else {
            raiz->esquerda = rse(arvore, raiz->esquerda);
            return rsd(arvore, raiz);
        }
    }

    if(fb_raiz < -1 ) {
        if(fb_dir <= 0)
            return rse(arvore, raiz);
        else {
            raiz->direita = rsd(arvore, raiz->direita);
            return rse(arvore, raiz);
        }
    }

    return raiz;
}

NoAVL *remover_no_avl(ArvoreAVL *arvore, int chave) {
    if (!arvore || !arvore->raiz) return NULL;
    arvore->raiz = remover_aux(arvore->raiz, chave, arvore);
    return arvore->raiz;
}
