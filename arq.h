#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum coloracao
{
    Vermelho,
    Preto
};
typedef enum coloracao Cor;

typedef struct no
{
    struct no *pai;
    struct no *esquerda;
    struct no *direita;
    Cor cor;
    int valor;
} No;

