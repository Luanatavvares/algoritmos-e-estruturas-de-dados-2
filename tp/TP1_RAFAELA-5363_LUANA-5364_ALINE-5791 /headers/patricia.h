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

#ifndef PATRICIA_H
#define PATRICIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_OCORRENCIAS 100
#define MAX_PALAVRA 100
typedef enum { interno, externo } tipoLoc;

typedef struct NoPatricia* Patricia;

typedef struct OcorrenciaPat{
    char palavra[MAX_PALAVRA];
    int idDoc_pat;
    int qtde_pat;
} OcorrenciaPat;

typedef struct {
    char valPalavra[50];
    int idDocPat;
    int qtde;
    int num_ocorrencias;
    struct {
        char palavra[50];
        int idDoc_pat;
        int qtde_pat;
    } cont_ocorrencias[50]; 
} tipoPalavra;

typedef struct NoPatricia {
    tipoLoc nt;
    union {
        struct {
            int index;
            char indexLetra;
            Patricia esq, dir;
        } nInterno;
        tipoPalavra palavra;
    } no;
} tipoNo;


Patricia criar_arvore_patricia();
char letra(tipoPalavra k, int i);
short eExterno(Patricia p);
Patricia criaNoInt(int i, Patricia *esq, Patricia *dir);
Patricia criaNoExt(tipoPalavra k);
Patricia insereEntre(tipoPalavra k, Patricia *t, int i);
Patricia inserePatricia(char* valPalavra, int idDocPat, Patricia *t);
tipoPalavra* pesquisaPatricia(char palavra[50], Patricia t);
int quantasPalavrasPatricia(Patricia t, int idDocPat);
void construir_indices_invertidos_patricia(ListaArquivos *lista_arquivos, Patricia *arvore, int *id_docPat) ;
void adicionar_ocorrencia(char *palavra, int idDocPat, tipoPalavra *palavra_indexada) ;
tipoPalavra *criar_tipo_palavra() ;
void extrairPalavras(Patricia t, tipoPalavra **vetor, int *index, int *tamanho) ;
void imprimirPatriciaOrdenada(Patricia t) ;
int comparar_palavrasPat(const void *a, const void *b) ;
#endif