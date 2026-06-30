#include "arq.h"

No* raiz = NULL;
unsigned long long esforco = 0;

No* bst(No* arv, No* aux){
    
    if(arv == NULL){
        return aux;
    }

    esforco++;
    if(aux->valor < arv->valor){
        arv->esquerda = bst(arv->esquerda, aux);
        arv->esquerda->pai = arv;
    }

    else if(aux->valor >= arv->valor){
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

void balancearAdd(No* nodo){
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

    raiz->cor = Preto;
}

void troca(No* nodo, No* aux){

    if(nodo->pai == NULL){
        raiz = aux;
    }
    else if(nodo == nodo->pai->esquerda){
        nodo->pai->esquerda = aux;
    }
    else{
        nodo->pai->direita = aux;
    }

    if(aux != NULL){
        aux->pai = nodo->pai;
    }
}

void balancearRem(No* u, No* uPai){
    No* v = NULL;

    while(u != raiz && (u == NULL || u->cor == 0)){

        if(u == uPai->esquerda){
            v = uPai->direita;

            if(v != NULL && v->cor == 1){
                v->cor = 0;
                uPai->cor = 1;
                rodaEsquerda(uPai);
                v = uPai->direita;
            }

            if((v == NULL) || ((v->esquerda == NULL || v->esquerda->cor == 0) && (v->direita == NULL || v->direita->cor == 0))){
                if(v != NULL) v->cor = 1;
                u = uPai;
                uPai = u->pai;
            }
            else{
                if(v->direita == NULL || v->direita->cor == 0){
                    if(v->esquerda != NULL) v->esquerda->cor = 0;
                    v->cor = 1;
                    rodaDireita(v);
                    v = uPai->direita;
                }

                if(v != NULL){
                    v->cor = uPai->cor;
                    if(v->direita != NULL) v->direita->cor = 0;
                }
                uPai->cor = 0;
                rodaEsquerda(uPai);
                u = raiz;
            }
        }
        else{
            v = uPai->esquerda;

            if(v != NULL && v->cor == 1){
                v->cor = 0;
                uPai->cor = 1;
                rodaDireita(uPai);
                v = uPai->esquerda;
            }

            if((v == NULL) || ((v->direita == NULL || v->direita->cor == 0) && (v->esquerda == NULL || v->esquerda->cor == 0))){
                if(v != NULL) v->cor = 1;
                u = uPai;
                uPai = u->pai;
            }
            else{
                if(v->esquerda == NULL || v->esquerda->cor == 0){
                    if(v->direita != NULL) v->direita->cor = 0;
                    v->cor = 1;
                    rodaEsquerda(v);
                    v = uPai->esquerda;
                }

                if(v != NULL){
                    v->cor = uPai->cor;
                    if(v->esquerda != NULL) v->esquerda->cor = 0;
                }
                uPai->cor = 0;
                rodaDireita(uPai);
                u = raiz;
            }
        }
    }

    if(u != NULL){
        u->cor = 0;
    }
}

void removerNodo(No* z){
    if(z == NULL) return;

    No* y = z;
    No* x = NULL;
    No* xPai = NULL;

    int corOg = y->cor;

    if(z->esquerda == NULL){
        x = z->direita;
        xPai = z->pai;
        troca(z, z->direita);

    }
    else if(z->direita == NULL){
        x = z->esquerda;
        xPai = z->pai;
        troca(z, z->esquerda);
    }
    else{
        y = z->direita;
        while(y->esquerda != NULL){
            y = y->esquerda;
        }

        corOg = y->cor;
        x = y->direita;

        if(y->pai == z){
            xPai = y;
        }
        else{
            xPai = y->pai;
            troca(y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }

        troca(z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }

    if(corOg == 0){
        balancearRem(x, xPai);
    }

    free(z);
}

void remover(int valor){
    No* atual = raiz;

    while(atual != NULL && atual->valor != valor){
        esforco++;

        if(valor < atual->valor){
            atual = atual->esquerda;
        }
        else{
            atual = atual->direita;
        }
    }

    if(atual == NULL){
        //printf("Valor não encontrado\n");
        return;
    }

    removerNodo(atual);
}

No* criarNo(int valor){
    No* novo = (No*)malloc(sizeof(No));
    if(novo == NULL){
        printf("Erro de alocação de memória\n");
        exit(1);
    }
    novo->valor = valor;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->pai = NULL;
    novo->cor = Vermelho;
    return novo;
}

void inserir(int valor){
    No* novo = criarNo(valor);
    raiz = bst(raiz, novo);

    balancearAdd(novo);
}

void imprimirArvore(No* no, int espaco){
    if(no == NULL) return;

    espaco += 7;

    imprimirArvore(no->direita, espaco);

    printf("\n");
    for(int i = 7; i < espaco; i++){
        printf(" ");
    }

    printf("%d(%s)\n", no->valor, no->cor == Vermelho ? "V" : "P");

    imprimirArvore(no->esquerda, espaco);
}

void liberar(No* nodo){
    if (nodo == NULL) return;
    liberar(nodo->esquerda);
    liberar(nodo->direita);
    free(nodo);
}

/*int main(){
    srand(time(NULL));
    int num = 10;
    int maxn = 10000;
    int passo = 100;;

    printf("TAMANHO_N, ESFORCO_ADICAO_RN, ESFORCO_REMOCAO_RN\n");
    
    for(int n = passo; n <= maxn; n += passo){
        unsigned long long totalAdd = 0;
        unsigned long long totalRem = 0;

        for(int amostra = 0; amostra < num; amostra++){
            raiz = NULL;

            int* vetor = (int*) malloc(n*sizeof(int));
            for(int i = 0; i < n; i++){
                vetor[i] = rand() % 100000;
            }

            esforco = 0;

            for(int i = 0; i < n; i++){
                inserir(vetor[i]);
            }
            totalAdd += esforco;

            esforco = 0;

            for(int i = 0; i < n; i++){
                remover(vetor[i]);
            }
            totalRem += esforco;

            free(vetor);
        }

        printf("%d, %llu, %llu\n", n, (totalAdd / num) / n, (totalRem / num) / n);
    }

    return 0;
}*/