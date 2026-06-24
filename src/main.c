#include <time.h>

#include "avl.h"

#define ARQUIVO1 "insercao.csv"
#define ARQUIVO2 "remocao.csv"
#define CONJUNTOS 30
#define MAX_TAM 10000

void shuffle(int *vetor, int tam);

int main() {
    srand(time(NULL));

    int chaves[MAX_TAM];
    for(int i = 0; i < MAX_TAM; i++)
        chaves[i] = i;
    shuffle(chaves, MAX_TAM);

    FILE *fInsert = fopen(ARQUIVO1, "w");
    FILE *fDelete = fopen(ARQUIVO2, "w");

    fprintf(fInsert, "Tamanho,AVL,Rubro-negra,B1,B5,B10\n");
    fprintf(fDelete, "Tamanho,AVL,Rubro-negra,B1,B5,B10\n");

    for(int i = 1; i <= CONJUNTOS; i++) {
        ArvoreAVL *avl = criar_arvore_avl();

        int tam = (MAX_TAM / CONJUNTOS) * i;

        for(int j = 0; j < tam; j++) {
            inserir_no_avl(avl, chaves[j]);
            // inserir_no_rb...
        }
        fprintf(fInsert,
                "%d,%ld,%ld,%ld,%ld,%ld\n",
                tam,
                avl->comparacoes,
                0,
                0,
                0,
                0);

        shuffle(chaves, MAX_TAM);
        avl->comparacoes = 0;

        for(int j = 0; j < tam; j++) {
            remover_no_avl(avl, chaves[j]);
            // remover_no_rb...
        }
        fprintf(fDelete,
                "%d,%ld,%ld,%ld,%ld,%ld\n",
                tam,
                avl->comparacoes,
                0,
                0,
                0,
                0);
        free_avl(avl);
    }
}

void shuffle(int *vetor, int tam) {
    if(tam < 1) return;
    for(int i = 0; i < tam - 1; i++) {
        int j = i + rand() / (RAND_MAX / (tam - i) +1);
        int t = vetor[j];
        vetor[j] = vetor[i];
        vetor[i] = t;
    }
}
