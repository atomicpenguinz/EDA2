#include <time.h>

#include "avl.h"

#define ARQUIVO1 "insercao.csv"
#define ARQUIVO2 "remocao.csv"
#define CONJUNTOS 50
#define REPETICOES 30
#define MAX_TAM 10000

void shuffle(int *vetor, int tam);

int main() {
    srand(time(NULL));

    int passo = MAX_TAM / CONJUNTOS;
    int chaves[MAX_TAM];
    for(int i = 0; i < MAX_TAM; i++)
        chaves[i] = i;

    FILE *fInsert = fopen(ARQUIVO1, "w");
    FILE *fDelete = fopen(ARQUIVO2, "w");

    fprintf(fInsert, "Tamanho,AVL,Rubro-negra,B1,B5,B10\n");
    fprintf(fDelete, "Tamanho,AVL,Rubro-negra,B1,B5,B10\n");

    for(int i = 1; i <= CONJUNTOS; i++) {
        int tam = (MAX_TAM / CONJUNTOS) * i;
        shuffle(chaves, MAX_TAM);
        long comparacoesAVL_insercao = 0;
        long comparacoesAVL_remocao = 0;
        ArvoreAVL *avl = criar_arvore_avl();

        for(int i = 0; i < MAX_TAM; i++) {

            for(int j = 0; j < tam; j++) {
                inserir_no_avl(avl, chaves[j]);
                // inserir_no_rb...
            }

            comparacoesAVL_insercao += avl->comparacoes;

            shuffle(chaves, tam);

            avl->comparacoes = 0L;
            for(int j = 0L; j < tam; j++) {
                remover_no_avl(avl, chaves[j]);
                // remover_no_rb...
            }

            comparacoesAVL_remocao += avl->comparacoes;
            free_avl(avl);
        }
        fprintf(fInsert,
                "%d,%.4lf,%ld,%ld,%ld,%ld\n",
                tam,
                (double) comparacoesAVL_insercao / (REPETICOES * tam),
                0L,
                0L,
                0L,
                0L);
        fprintf(fDelete,
                "%d,%.4lf,%ld,%ld,%ld,%ld\n",
                tam,
                (double) comparacoesAVL_remocao / (REPETICOES * tam),
                0L,
                0L,
                0L,
                0L);
    }
    fclose(fInsert);
    fclose(fDelete);
    return 0;
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
