#include <time.h>

#include "avl.h"
#include "arq.h"
#include "btree.h"

extern No* raiz;
extern unsigned long long esforco;
void inserir(int valor);
void remover(int valor);
void liberar(No* nodo);

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
        int tam = passo * i;

        long comparacoesAVL_insercao = 0;
        long comparacoesAVL_remocao = 0;

        long comparacoesRN_insercao = 0;
        long comparacoesRN_remocao = 0;

        long comparacoesB1_insercao = 0;
        long comparacoesB1_remocao = 0;

        long comparacoesB5_insercao = 0;
        long comparacoesB5_remocao = 0;

        long comparacoesB10_insercao = 0;
        long comparacoesB10_remocao = 0;

        for(int r = 0; r < REPETICOES; r++) {
            shuffle(chaves, MAX_TAM);

            ArvoreAVL *avl = criar_arvore_avl();
            ArvoreB *b1 = criar_arvore_b(1);
            ArvoreB *b5 = criar_arvore_b(5);
            ArvoreB *b10 = criar_arvore_b(10);

            raiz = NULL;
            esforco = 0;

            for(int j = 0; j < tam; j++) {
                inserir_no_avl(avl, chaves[j]);
                inserir(chaves[j]);
                inserir_no_b(b1, chaves[j]);
                inserir_no_b(b5, chaves[j]);
                inserir_no_b(b10, chaves[j]);
            }
            comparacoesAVL_insercao += avl->comparacoes;
            comparacoesB1_insercao += b1->comparacoes;
            comparacoesB5_insercao += b5->comparacoes;
            comparacoesB10_insercao += b10->comparacoes;
            comparacoesRN_insercao += esforco;

            shuffle(chaves, tam);

            avl->comparacoes = 0;
            b1->comparacoes = 0;
            b5->comparacoes = 0;
            b10->comparacoes = 0;
            esforco = 0;

            for(int j = 0; j < tam; j++) {
                remover_no_avl(avl, chaves[j]);
                remover_no_b(b1, chaves[j]);
                remover_no_b(b5, chaves[j]);
                remover_no_b(b10, chaves[j]);
                remover(chaves[j]);
            }

            comparacoesAVL_remocao += avl->comparacoes;
            comparacoesB1_remocao += b1->comparacoes;
            comparacoesB5_remocao += b5->comparacoes;
            comparacoesB10_remocao += b10->comparacoes;
            comparacoesRN_remocao += esforco;

            free_avl(avl);
            libera_arvore_b(b1);
            libera_arvore_b(b5);
            libera_arvore_b(b10);
            liberar(raiz);
            raiz = NULL;
        }
        fprintf(fInsert,
                "%d,%lf,%lf,%lf,%lf,%lf\n",
                tam,
                (double) comparacoesAVL_insercao / (REPETICOES * tam),
                (double) comparacoesRN_insercao / (REPETICOES * tam),
                (double) comparacoesB1_insercao / (REPETICOES * tam),
                (double) comparacoesB5_insercao / (REPETICOES * tam),
                (double) comparacoesB10_insercao / (REPETICOES * tam));

        fprintf(fDelete,
                "%d,%lf,%lf,%lf,%lf,%lf\n",
                tam,
                (double) comparacoesAVL_remocao / (REPETICOES * tam),
                (double) comparacoesRN_remocao / (REPETICOES * tam),
                (double) comparacoesB1_remocao / (REPETICOES * tam),
                (double) comparacoesB5_remocao / (REPETICOES * tam),
                (double) comparacoesB10_remocao / (REPETICOES * tam));
    }
    printf("arquivos '%s' e '%s' escritos\n", ARQUIVO1, ARQUIVO2);
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
