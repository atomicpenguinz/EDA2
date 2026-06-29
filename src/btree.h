#include <stdlib.h>

typedef struct no_b {
    int total;
    int *chaves;
    struct no_b **filhos;
    struct no_b *pai;
} NoB;

typedef struct arvore_b {
    NoB *raiz;
    int ordem;
    long comparacoes;
} ArvoreB;

ArvoreB *criar_arvore_b(int ordem);
void inserir_no_b(ArvoreB *arvore, int chave);
void remover_no_b(ArvoreB *arvore, int chave);
void libera_arvore_b(ArvoreB *arvore);