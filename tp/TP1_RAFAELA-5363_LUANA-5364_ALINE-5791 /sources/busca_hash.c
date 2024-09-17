#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "../headers/file_reader.h"
#include "../headers/hash.h"
#include "../headers/busca_hash.h"


// Calcula o TF
double calcular_tf(int frequencia, int total_termos) {
    if (total_termos == 0) {
        return 0.0; // Evita divisão por zero
    }
    return (double)frequencia / total_termos;
}

// Calcula o IDF
double calcular_idf(int num_documentos, int documentos_contendo_termo) {
    if (documentos_contendo_termo == 0) {
        return log((double)num_documentos + 1); // Ajuste para evitar log(0)
    }
    return log((double)num_documentos / (documentos_contendo_termo + 1) + 1);
}

// Calcula o TF-IDF
double calcular_tf_idf(double tf, double idf) {
    return tf * idf;
}

// Calcula a relevância de um documento
double calcular_relevancia(int total_termos_documento, double *tf_idfs, int num_termos) {
    double soma = 0;
    for (int i = 0; i < num_termos; i++) {
        soma += tf_idfs[i];
    }
    if (total_termos_documento == 0) {
        return 0.0; // Evita divisão por zero
    }
    return soma / total_termos_documento;
}

// Função para comparar dois documentos com base na relevância
int comparar_relevancia(const void *a, const void *b) {
    int index_a = *(const int *)a;
    int index_b = *(const int *)b;
    
    if (relevancias_global[index_a] < relevancias_global[index_b]) return 1;
    if (relevancias_global[index_a] > relevancias_global[index_b]) return -1;
    return 0;
}

// Função principal para buscar e calcular relevância
void buscar_e_calcular_relevancia(IndiceInvertidoHash *indice_hash, const char **termos, int num_termos, ListaArquivos *lista_arquivos) {
    // Atualizar o total de termos antes de realizar a busca e cálculo
    contar_total_termos(lista_arquivos);

    int num_documentos = lista_arquivos->cont_arquivos;
    double *relevancias = (double *)calloc(num_documentos, sizeof(double));
    int *documentos_indices = (int *)malloc(num_documentos * sizeof(int));
    
    // Armazenar a referência global de relevâncias
    relevancias_global = relevancias;

    for (int i = 0; i < num_documentos; i++) {
        documentos_indices[i] = i; // Inicializar com o índice do documento
        double tf_idfs[num_termos];
        memset(tf_idfs, 0, sizeof(tf_idfs));

        for (int j = 0; j < num_termos; j++) {
            unsigned int indice = funcao_hash(termos[j], indice_hash->capacidade);
            EntradaIndiceInvertido *entrada = indice_hash->tabela[indice];
            int frequencias = 0; // Frequência no documento específico

            while (entrada != NULL) {
                if (strcmp(entrada->palavra, termos[j]) == 0) {
                    for (int k = 0; k < entrada->cont_ocorrencias; k++) {
                        if (entrada->ocorrencias[k].idDoc == i) {
                            frequencias++;
                        }
                    }
                    break;
                }
                entrada = entrada->prox;
            }

            double tf = calcular_tf(frequencias, lista_arquivos->total_termos[i]);
            double idf = calcular_idf(num_documentos, entrada ? entrada->cont_ocorrencias : 0);
            tf_idfs[j] = calcular_tf_idf(tf, idf);
        }

        relevancias[i] = calcular_relevancia(lista_arquivos->total_termos[i], tf_idfs, num_termos);
    }

    // Ordenar os documentos com base na relevância
    qsort(documentos_indices, num_documentos, sizeof(int), comparar_relevancia);

    // Exibir os documentos em ordem decrescente de relevância
    for (int i = 0; i < num_documentos; i++) {
        int index = documentos_indices[i];
        printf("Documento %d - Relevância: %.3f\n", index + 1, relevancias[index]);
    }

    free(relevancias);
    free(documentos_indices);
}
