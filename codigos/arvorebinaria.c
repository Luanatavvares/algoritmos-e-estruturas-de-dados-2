#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

typedef struct no {
    int numero;
    struct no *direita;
    struct no *esquerda; 
} No;

void criar(No **arvore, int n) {
    *arvore = (No*)malloc(sizeof(No));
    (*arvore)->numero = n;
    (*arvore)->esquerda = NULL;
    (*arvore)->direita = NULL;
}

void inserir(No **arvore, int numero) {
    if (*arvore == NULL) {
        criar(arvore, numero);
    } else {
        if (numero < (*arvore)->numero) {
            inserir(&((*arvore)->esquerda), numero);
        } else {
            inserir(&((*arvore)->direita), numero);
        }
    }
}

void preordem(No *arvore) {
    if (arvore != NULL) {
        printf("%d ", arvore->numero);
        preordem(arvore->esquerda);
        preordem(arvore->direita);
    }
}
    
void ordem(No *arvore) {
    if (arvore != NULL) {
        ordem(arvore->esquerda);
        printf("%d ", arvore->numero);
        ordem(arvore->direita);
    }
}

void posordem(No *arvore) {
    if (arvore != NULL) {
        posordem(arvore->esquerda);
        posordem(arvore->direita);
        printf("%d ", arvore->numero);
    }
}




int pesquisa(No *arvore, int n) {
    if (arvore != NULL) {
        if (arvore->numero == n) {
            return 1;
        } else if (n > arvore->numero) {
            return pesquisa(arvore->direita, n);
        } else {
            return pesquisa(arvore->esquerda, n);
        }
    }
    return 0;
}


int numeronos (No *arvore){
    if (arvore == NULL){
        return 0;
        }
    else {
        return 1 + numeronos(arvore-> direita) + numeronos(arvore-> esquerda);
    }
    }




int maior(No *arvore){
    if (arvore-> direita == NULL){
        return arvore-> numero;
    
    } else{
        return maior(arvore->direita);
    }

}

int main() {
    int n;
    scanf("%d", &n);
    No *arvore = NULL;
    
    for (int i = 0; i < n; i++) {
        int numero;
        scanf("%d", &numero);
        inserir(&arvore, numero);
    }
    
    printf("Preordem: ");
    preordem(&arvore);
    printf("\n");

    printf("Ordem: ");
    ordem(&arvore);
    printf("\n");

    printf("Posordem: ");
    posordem(&arvore);
    printf("\n");

    return 0;
} 
