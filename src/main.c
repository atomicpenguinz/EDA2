#include <time.h>

#include "avl.h"
#include "arq.h"

extern No* raiz;
extern unsigned long long esforco;

void inserir(int valor);
void remover(int valor);
void liberar(No* nodo);

#define ARQUIVO1 "insercao.csv"
#define ARQUIVO2 "remocao.csv"
#define CONJUNTOS 25
#define REPETICOES 30
#define MAX_TAM 10000

void shuffle(int *vetor, int tam);

int main() {
    srand(time(NULL));

    #ifdef DEBUG
    FILE *fDebug = fopen("debug.log", "w");
    #endif
    int passo = MAX_TAM / CONJUNTOS;
    int chaves[MAX_TAM];
    for(int i = 0; i < MAX_TAM; i++)
        chaves[i] = i;

    long *comparacoesAVL_insercao = calloc(CONJUNTOS + 1, sizeof(long));
    long *comparacoesAVL_remocao = calloc(CONJUNTOS + 1, sizeof(long));

    long *comparacoesRN_insercao = calloc(CONJUNTOS + 1, sizeof(long));
    long *comparacoesRN_remocao = calloc(CONJUNTOS + 1, sizeof(long));

    for(int i = 0; i < REPETICOES; i++) {
        #ifdef DEBUG
        fprintf(fDebug, "repetição nº %d\n", i);
        #endif
        shuffle(chaves, MAX_TAM);
        ArvoreAVL *avl = criar_arvore_avl();

        raiz = NULL;
        esforco = 0;

        int index = 1;
        for(int n = 1; n <= MAX_TAM; n++) {
            inserir_no_avl(avl, chaves[n-1]);
            inserir(chaves[n-1]);

            if((n % passo) == 0) {
                #ifdef DEBUG
                fprintf(fDebug, "\ttamanho: %d\n", n);
                #endif
                comparacoesAVL_insercao[index] += avl->comparacoes;
                avl->comparacoes = 0;

                comparacoesRN_insercao[index] += esforco;
                esforco = 0;

                ArvoreAVL *avl_tmp = criar_arvore_avl();
                No* raiz_principal = raiz;
                raiz = NULL;

                for(int j = 0; j < n; j++) {
                    inserir_no_avl(avl_tmp, chaves[j]);
                    inserir(chaves[j]);
                }

                int *chaves_tmp = malloc(n * sizeof(int));
                for(int j = 0; j < n; j++)
                    chaves_tmp[j] = chaves[j];
                shuffle(chaves_tmp, n);

                esforco = 0;

                for(int j = 0; j < n; j++) {
                    remover_no_avl(avl_tmp, chaves_tmp[j]);
                    remover(chaves_tmp[j]);

                }


                comparacoesAVL_remocao[index] += avl_tmp->comparacoes;
                comparacoesRN_remocao[index] += esforco;

                free(chaves_tmp);
                free_avl(avl_tmp);

                raiz = raiz_principal;
                esforco = 0;

                index++;
            }

        }
        free_avl(avl);

        liberar(raiz);
        raiz = NULL;

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
                (double) comparacoesRN_insercao[i] / (REPETICOES * passo),
                0.0,
                0.0,
                0.0);
        fprintf(fDelete,
                "%d,%lf,%lf,%lf,%lf,%lf\n",
                tam,
                (double) comparacoesAVL_remocao[i] / (REPETICOES * tam),
                (double) comparacoesRN_remocao[i] / (REPETICOES * tam),
                0.0,
                0.0,
                0.0);
    }
    printf("arquivos '%s' e '%s' escritos\n", ARQUIVO1, ARQUIVO2);
    fclose(fInsert);
    fclose(fDelete);
    #ifdef DEBUG
    fclose(fDebug);
    #endif

    free(comparacoesAVL_insercao);
    free(comparacoesAVL_remocao);
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
