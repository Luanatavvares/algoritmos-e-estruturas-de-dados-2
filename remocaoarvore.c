#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int numero;
    struct no *esquerda, *direita;
} no;

no *criar(int numero) {
    no *novoNo = (no *)malloc(sizeof(no));
    novoNo->numero = numero;
    novoNo->esquerda = novoNo->direita = NULL;
    return novoNo;
}

no *inserir(no *arvore, int numero) {
    if (arvore == NULL)
        return criar(numero);

    if (numero < arvore->numero)
        arvore->esquerda = inserir(arvore->esquerda, numero);
    else if (numero > arvore->numero)
        arvore->direita = inserir(arvore->direita, numero);
    return arvore;
}

void imprimiordem(no *arvore) {
    if (arvore != NULL) {
        imprimiordem(arvore->esquerda);
        printf("%d ", arvore->numero);
        imprimiordem(arvore->direita);
    }
}

int main() {
    int N;
    scanf("%d", &N);

    no *arvore = NULL;

    for (int i = 0; i < N; i++) {
        int num;
        scanf("%d", &num);
        arvore = inserir(arvore, num);
    }

    imprimiordem(arvore);

    return 0;
}

