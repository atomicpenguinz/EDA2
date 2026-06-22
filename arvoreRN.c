#include "arq.h"

No* raiz = NULL;

No* bst(No* arv, No* aux){
    if(arv == NULL){
        return aux;
    }

    if(aux->valor < arv->valor){
        arv->esquerda = bst(arv->esquerda, aux);
        arv->esquerda->pai = arv;
    }

    else if(aux->valor > arv->valor){
        arv->direita = bst(arv->direita, aux);
        arv->direita->pai = arv;
    }

    return arv;
}

void rodaDireita(No* aux){
    No* esq = aux->esquerda;
    aux->esquerda = esq->direita;
    if(aux->esquerda){
        aux->esquerda->pai = aux;
    }
    esq->pai = aux->pai;
    if(!aux->pai){
        raiz = esq;
    }
    else if(aux == aux->pai->esquerda){
        aux->pai->esquerda = esq;
    }
    else{
        aux->pai->direita = esq;
    }
    esq->direita = aux;
    aux->pai = esq;
}

void rodaEsquerda(No* aux){
    No* dir = aux->direita;
    aux->direita = dir->esquerda;
    if(aux->direita){
        aux->direita->pai = aux;
    }
    dir->pai = aux->pai;
    if(!aux->pai){
        raiz = dir;
    }
    else if(aux == aux->pai->esquerda){
        aux->pai->esquerda = dir;
    }
    else{
        aux->pai->direita = dir;
    }
    dir->esquerda = aux;
    aux->pai = dir;
}

void balancear(No* raiz, No* nodo){
    No* paiNodo = NULL;
    No* voNodo = NULL;

    while((nodo != raiz) && (nodo->cor != 0) && (nodo->pai->cor == 1)){
        paiNodo = nodo->pai;
        voNodo = nodo->pai->pai;

        if(paiNodo == voNodo->esquerda){
            No* tioNodo = voNodo->direita;

            if(tioNodo != NULL && tioNodo->cor == 1){
                voNodo->cor = 1;
                paiNodo->cor = 0;
                tioNodo->cor = 0;
                nodo = voNodo;
            }
            else{

                if(nodo == paiNodo->direita){
                    rodaEsquerda(paiNodo);
                    nodo = paiNodo;
                    paiNodo = nodo->pai;
                }

                rodaDireita(voNodo);
                int t = paiNodo->cor;
                paiNodo->cor = voNodo->cor;
                voNodo->cor = t;
                nodo = paiNodo;
            }
        }
        else{
            No* tioNodo = voNodo->esquerda;

            if(tioNodo != NULL && tioNodo->cor == 1){
                voNodo->cor = 1;
                paiNodo->cor = 0;
                tioNodo->cor = 0;
                nodo = voNodo;
            }
            else{
                if(nodo == paiNodo->esquerda){
                    rodaDireita(paiNodo);
                    nodo = paiNodo;
                    paiNodo = nodo->pai;
                }

                rodaEsquerda(voNodo);
                int t = paiNodo->cor;
                paiNodo->cor = voNodo->cor;
                voNodo->cor = t;
                nodo = paiNodo;
            }
        }
    }
}