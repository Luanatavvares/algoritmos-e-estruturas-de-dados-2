#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    char letra;
    struct no *esquerda;
    struct no *direita;
} no;

void criar(no **arvore, char n) {
    *arvore = (no*)malloc(sizeof(no));
    (*arvore)->letra = n;
    (*arvore)->esquerda = NULL;
    (*arvore)->direita = NULL;
}

void montararvore(no **arvore, char string[]) {
    int i;
    for (i = 0; i < strlen(string); i++) {
        if (*arvore == NULL) {
            criar(arvore, string[i]);
        } else {
            no *atual = *arvore;
            while (1) {
                if (string[i] < atual->letra) {
                    if (atual->esquerda == NULL) {
                        criar(&(atual->esquerda), string[i]);
                        break;
                    } else {
                        atual = atual->esquerda;
                    }
                } else {
                    if (atual->direita == NULL) {
                        criar(&(atual->direita), string[i]);
                        break;
                    } else {
                        atual = atual->direita;
                    }
                }
            }
        }
    }
}



void posordem(no *arvore) {
    if (arvore != NULL) {
        posordem(arvore->esquerda);
        posordem(arvore->direita);
        printf("%c", arvore->letra);
    }
}

int main() {
    no *arvore = NULL;
    char string[100]; 
    
    scanf("%s", string);
    
    montararvore(&arvore, string);
    posordem(arvore);
    
    return 0;
}
