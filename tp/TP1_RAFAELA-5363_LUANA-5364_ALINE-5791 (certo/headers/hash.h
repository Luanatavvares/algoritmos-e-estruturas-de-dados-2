#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DOCS 100

typedef struct {
    int idDoc;
    int quantidade;
} Ocorrencia;

typedef struct NoEntrada {
    Ocorrencia *ocorrencias;
    char *palavra;
    int cont_ocorrencias;
    int capacidade_ocorrencias;
    struct NoEntrada *prox;
} EntradaIndiceInvertido;

typedef struct {
    EntradaIndiceInvertido **tabela;
    int tamanho;
    int capacidade;
} IndiceInvertidoHash;

typedef struct {
    int idDoc;
    double relevancia;
} ResultadoBusca;
void converter_para_minusculas(char *str) ;
unsigned int funcao_hash(const char *palavra, int capacidade);
EntradaIndiceInvertido *criar_nova_entrada(const char *palavra);
void adicionar_entrada_hash(IndiceInvertidoHash *hash, EntradaIndiceInvertido *entrada);
void atualizar_ocorrencias(EntradaIndiceInvertido *entrada, int idDoc, int quantidade);
void inserir_hash(IndiceInvertidoHash *hash, const char *palavra, int idDoc, int quantidade);
IndiceInvertidoHash *criar_tabela_hash(int capacidade);
void liberar_tabela_hash(IndiceInvertidoHash *tabela_hash);
void construir_indices_invertidos(ListaArquivos *lista_arquivos, IndiceInvertidoHash *indice_hash, int *id_doc);
int comparar_palavras(const void *a, const void *b);
void imprimir_indices_invertidos(IndiceInvertidoHash *indice_hash);
void buscar_documentos(IndiceInvertidoHash *hash, const char **termos, int num_termos, ListaArquivos *lista_arquivos);

#endif // HASH_H
