/* ------------------------------------------------------------------------------------------------------------------------------------------------------------

    Universidade Federal De Viçosa - Campus Florestal
    Trabalho prático 1 de Algorítimos e estrutura de dados 2
    
    Professora responsável: 
        Glaucia Braga e Silva
        
    Integrantes do Gupo:
        Aline Cristina - 5791
        Luana Tavares  - 5364
        Rafaella Pinheiro - 5363

------------------------------------------------------------------------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "../headers/file_reader.h"
#include "../headers/hash.h"
#include "../headers/patricia.h"
#include "../headers/menu.h"

char letra(tipoPalavra k, int i) {
    if (strlen(k.valPalavra) < i)
        return ' ';
    return k.valPalavra[i - 1];
}

short eExterno(Patricia p) {
    return (p->nt == externo);
}

Patricia criaNoInt(int i, Patricia *esq, Patricia *dir) {
    Patricia p = (Patricia)malloc(sizeof(tipoNo));
    p->nt = interno;
    p->no.nInterno.esq = *esq;
    p->no.nInterno.dir = *dir;
    p->no.nInterno.index = i;

    Patricia aux = p;
    while (!eExterno(aux)) {
        aux = aux->no.nInterno.dir;
    }
    p->no.nInterno.indexLetra = letra(aux->no.palavra, i);
    return p;
}

Patricia criaNoExt(tipoPalavra k) {
    Patricia p = (Patricia)malloc(sizeof(tipoNo));
    p->nt = externo;
    p->no.palavra = k;
    return p;
}

Patricia insereEntre(tipoPalavra k, Patricia *t, int i) {
    Patricia p;
    if (eExterno(*t) || i < (*t)->no.nInterno.index) {
        p = criaNoExt(k);
        if (letra(k, i) == (*t)->no.nInterno.indexLetra)
            return criaNoInt(i, t, &p);
        else
            return criaNoInt(i, &p, t);
    } else {
        if (letra(k, (*t)->no.nInterno.index) == (*t)->no.nInterno.indexLetra)
            (*t)->no.nInterno.dir = insereEntre(k, &(*t)->no.nInterno.dir, i);
        else
            (*t)->no.nInterno.esq = insereEntre(k, &(*t)->no.nInterno.esq, i);
        return *t;
    }
}

Patricia inserePatricia(char* valPalavra, int idDocPat, Patricia *t) {
    tipoPalavra palavra;
    strcpy(palavra.valPalavra, valPalavra);
    palavra.idDocPat = idDocPat;
    palavra.qtde = 1;
    palavra.num_ocorrencias = 0; // Inicializa o contador de ocorrências

    if (*t == NULL) {
        return criaNoExt(palavra);
    } else {
        Patricia p = *t;
        int i = 1;
        while (!eExterno(p)) {
            if (letra(palavra, p->no.nInterno.index) != p->no.nInterno.indexLetra) {
                p = p->no.nInterno.esq;
            } else {
                p = p->no.nInterno.dir;
            }
            i++;
        }

        if (strcmp(p->no.palavra.valPalavra, palavra.valPalavra) == 0) {
            // Atualiza a contagem do nó existente
            adicionar_ocorrencia(palavra.valPalavra, palavra.idDocPat, &p->no.palavra);
            return *t;
        } else {
            return insereEntre(palavra, t, i);
        }
    }
}


Patricia criar_arvore_patricia() {
    return NULL; 
}

tipoPalavra* pesquisaPatricia(char palavra[50], Patricia t) {
    if (t == NULL)
        return NULL;
    if (eExterno(t)) {
        if (strcmp(palavra, t->no.palavra.valPalavra) == 0)
            return &t->no.palavra;
        else
            return NULL;
    }
    tipoPalavra aux;
    strcpy(aux.valPalavra, palavra);
    if (letra(aux, t->no.nInterno.index) == t->no.nInterno.indexLetra)
        return pesquisaPatricia(palavra, t->no.nInterno.dir);
    else
        return pesquisaPatricia(palavra, t->no.nInterno.esq);
}


void construir_indices_invertidos_patricia(ListaArquivos *lista_arquivos, Patricia *arvore, int *id_docPat) {
    for (int i = 0; i < lista_arquivos->cont_arquivos; i++) {
        char *nome_arquivo = lista_arquivos->nomes_arquivos[i];
        FILE *arquivo = fopen(nome_arquivo, "r");
        if (!arquivo) {
            fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome_arquivo);
            continue;
        }

        char linha[2048];
        // Ler a primeira linha (será ignorada)
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            continue;
        }

        // Ler a segunda linha (ingredientes)
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            continue;
        }

        fclose(arquivo);

        // Processar a segunda linha
        char *ingrediente = strtok(linha, ";");
        while (ingrediente) {
            // Remover espaços em branco no início e no fim
            while (isspace((unsigned char)*ingrediente)) ingrediente++;
            char *end = ingrediente + strlen(ingrediente) - 1;
            while (end > ingrediente && isspace((unsigned char)*end)) end--;
            *(end + 1) = '\0';

            // Inserir o ingrediente no índice invertido
            tipoPalavra palavra;
            strcpy(palavra.valPalavra, ingrediente);
            palavra.idDocPat = *id_docPat;
            palavra.qtde = 1;
            palavra.num_ocorrencias = 0;
            *arvore = inserePatricia(palavra.valPalavra, *id_docPat, arvore);

            // Adicionar ocorrência ao índice invertido
            tipoPalavra *palavra_indexada = pesquisaPatricia(palavra.valPalavra, *arvore);
            if (palavra_indexada) {
                adicionar_ocorrencia(palavra.valPalavra, *id_docPat, palavra_indexada);
            }

            ingrediente = strtok(NULL, ";");
        }
        (*id_docPat)++;
    }
}


tipoPalavra *criar_tipo_palavra() {
    tipoPalavra *nova_palavra = (tipoPalavra *)malloc(sizeof(tipoPalavra));
    if (nova_palavra != NULL) {
        nova_palavra->valPalavra[0] = '\0'; // Inicializa a string vazia
        nova_palavra->idDocPat = 0;         // Inicializa o ID do documento
        nova_palavra->qtde = 0;             // Inicializa a quantidade de ocorrências
        // Inicializa outros campos conforme necessário
    }
    return nova_palavra;
}


void adicionar_ocorrencia(char *palavra, int idDoc, tipoPalavra *palavraTipo) {
    int found = 0;
    for (int i = 0; i < palavraTipo->num_ocorrencias; i++) {
        if (palavraTipo->cont_ocorrencias[i].idDoc_pat == idDoc) {
            palavraTipo->cont_ocorrencias[i].qtde_pat++;
            found = 1;
            break;
        }
    }
    if (!found) {
        if (palavraTipo->num_ocorrencias < MAX_OCORRENCIAS) {
            strcpy(palavraTipo->cont_ocorrencias[palavraTipo->num_ocorrencias].palavra, palavra);
            palavraTipo->cont_ocorrencias[palavraTipo->num_ocorrencias].idDoc_pat = idDoc;
            palavraTipo->cont_ocorrencias[palavraTipo->num_ocorrencias].qtde_pat = 1;
            palavraTipo->num_ocorrencias++;
        }
    }
}

// Função para extrair as palavras da Patricia para um vetor
void extrairPalavras(Patricia t, tipoPalavra **vetor, int *index, int *tamanho) {
    if (t == NULL)
        return;
    if (eExterno(t)) {
        if (*index >= *tamanho) {
            *tamanho *= 2;
            *vetor = realloc(*vetor, (*tamanho) * sizeof(tipoPalavra));
        }
        (*vetor)[*index] = t->no.palavra;
        (*index)++;
    } else {
        extrairPalavras(t->no.nInterno.esq, vetor, index, tamanho);
        extrairPalavras(t->no.nInterno.dir, vetor, index, tamanho);
    }
}

int comparar_palavrasPat(const void *a, const void *b) {
    const tipoPalavra *palavraA = a;
    const tipoPalavra *palavraB = b;
    return strcmp(palavraA->valPalavra, palavraB->valPalavra);
}

// Função principal para imprimir a Patricia em ordem alfabética
void imprimirPatriciaOrdenada(Patricia t) {
    int tamanho = 10;
    int index = 0;
    tipoPalavra *vetor = malloc(tamanho * sizeof(tipoPalavra));
    // Extrair palavras da árvore para o vetor
    extrairPalavras(t, &vetor, &index, &tamanho);
    // Ordenar o vetor
    qsort(vetor, index, sizeof(tipoPalavra), comparar_palavrasPat);
    // Imprimir o vetor ordenado
    for (int i = 0; i < index; i++) {
        printf("%s: ", vetor[i].valPalavra);
        for (int j = 0; j < vetor[i].num_ocorrencias; j++) {
            printf("<%d, %d>", vetor[i].cont_ocorrencias[j].qtde_pat,vetor[i].cont_ocorrencias[j].idDoc_pat+1);
            if (j < vetor[i].num_ocorrencias - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }

    free(vetor);
}