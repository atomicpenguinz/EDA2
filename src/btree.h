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

