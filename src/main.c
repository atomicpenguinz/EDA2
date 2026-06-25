#include <time.h>

#include "avl.h"

#define ARQUIVO1 "insercao.csv"
#define ARQUIVO2 "remocao.csv"
#define CONJUNTOS 25
#define REPETICOES 30
#define MAX_TAM 10000

void shuffle(int *vetor, int tam);

int main() {
    srand(time(NULL));

    int passo = MAX_TAM / CONJUNTOS;
    int chaves[MAX_TAM];
    for(int i = 0; i < MAX_TAM; i++)
        chaves[i] = i;

    long *comparacoesAVL_insercao = calloc(CONJUNTOS + 1, sizeof(long));
    long *comparacoesAVL_remocao = calloc(CONJUNTOS + 1, sizeof(long));
    for(int i = 0; i < REPETICOES; i++) {
        shuffle(chaves, MAX_TAM);
        ArvoreAVL *avl = criar_arvore_avl();

        int index = 1;
        for(int n = 1; n <= MAX_TAM; n++) {
            inserir_no_avl(avl, chaves[n-1]);

            if((n % passo) == 0) {
                comparacoesAVL_insercao[index] += avl->comparacoes;
                avl->comparacoes = 0;
                ArvoreAVL *avl_tmp = criar_arvore_avl();
                for(int j = 0; j < n; j++) {
                    inserir_no_avl(avl_tmp, chaves[j]);
                }

                int *chaves_tmp = malloc(n * sizeof(int));
                for(int j = 0; j < n; j++)
                    chaves_tmp[j] = chaves[j];
                shuffle(chaves_tmp, n);

                avl->comparacoes = 0;
                for(int j = 0; j < n; j++) {
                    remover_no_avl(avl_tmp, chaves[j]);
                }
                comparacoesAVL_remocao[index] += avl_tmp->comparacoes;
                free(chaves_tmp);
                free_avl(avl_tmp);
                index++;
            }

        }
        free_avl(avl);
    }
    FILE *fInsert = fopen(ARQUIVO1, "w");
    FILE *fDelete = fopen(ARQUIVO2, "w");

    fprintf(fInsert, "Tamanho,AVL,Rubro-negra,B1,B5,B10\n");
    fprintf(fDelete, "Tamanho,AVL,Rubro-negra,B1,B5,B10\n");

    for(int i = 1; i <= CONJUNTOS; i++) {
        int tam = passo * i;
        fprintf(fInsert,
                "%d,%lf,%lf,%lf,%lf,%lf\n",
                tam,
                (double) comparacoesAVL_insercao[i] / (REPETICOES * passo),
                0.0,
                0.0,
                0.0,
                0.0);
        fprintf(fDelete,
                "%d,%lf,%lf,%lf,%lf,%lf\n",
                tam,
                (double) comparacoesAVL_remocao[i] / (REPETICOES * tam),
                0.0,
                0.0,
                0.0,
                0.0);
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
