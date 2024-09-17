/* ------------------------------------------------------------------------------------------------------------------------------------------------------------

    Universidade Federal De Viçosa - Campus Florestal
    Trabalho prático 1 de Algorítimos e estrutura de dados 2
    
    Professora responsável: 
        Glaucia Braga e Silva
        
    Integrantes do Gupo:
        Aline Cristina - 5791
        Luana Tavares  - 5364
        Rafaella Pinheiro - 5363

    Arquivo: 
        hash.h
        Descrição do arquivo: Arquivo que contém as funções para manipulação da tabela hash e da criação do seu indice invertido.  
        

------------------------------------------------------------------------------------------------------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/file_reader.h"
#include "../headers/hash.h"
#include "../headers/menu.h"
#include "../headers/patricia.h"

/*--------------------------------------Funções Hash----------------------------------------------------------------*/
// Função hash para calcular o índice
unsigned int funcao_hash(const char *palavra, int capacidade) {
    unsigned int hash = 0;
    while (*palavra) {
        hash = (hash * 31) + *palavra++;
    }
    return hash % capacidade;
}
// Função para adicionar uma entrada na tabela hash
void adicionar_entrada_hash(IndiceInvertidoHash *hash, EntradaIndiceInvertido *entrada) {
    unsigned int indice = funcao_hash(entrada->palavra, hash->capacidade);

    EntradaIndiceInvertido *novo_no = criar_nova_entrada(entrada->palavra);
    novo_no->ocorrencias = entrada->ocorrencias;
    novo_no->cont_ocorrencias = entrada->cont_ocorrencias;
    novo_no->capacidade_ocorrencias = entrada->capacidade_ocorrencias;

    novo_no->prox = hash->tabela[indice];
    hash->tabela[indice] = novo_no;

    hash->tamanho++;
}
// Função para liberar a tabela hash
void liberar_tabela_hash(IndiceInvertidoHash *tabela_hash) {
    if (tabela_hash == NULL) {
        return;
    }

    for (int i = 0; i < tabela_hash->capacidade; i++) {
        EntradaIndiceInvertido *atual = tabela_hash->tabela[i];
        while (atual != NULL) {
            EntradaIndiceInvertido *prox = atual->prox;
            free(atual->ocorrencias);
            free(atual->palavra);
            free(atual);
            atual = prox;
        }
    }
    free(tabela_hash->tabela);
    free(tabela_hash);
}
// Função para criar a tabela hash
IndiceInvertidoHash *criar_tabela_hash(int capacidade) {
    IndiceInvertidoHash *hash = (IndiceInvertidoHash *)malloc(sizeof(IndiceInvertidoHash));
    hash->tabela = (EntradaIndiceInvertido **)calloc(capacidade, sizeof(EntradaIndiceInvertido *));
    hash->tamanho = 0;
    hash->capacidade = capacidade;
    return hash;
}


/*--------------------------------------Funções Auxiliares----------------------------------------------------------------*/
// Função para remover o ponto final, se existir
void remover_ponto_final(char *palavra) {
    size_t len = strlen(palavra);
    if (len > 0 && palavra[len - 1] == '.') {
        palavra[len - 1] = '\0';
    }
}
// Função de comparação para qsort
int comparar_palavras(const void *a, const void *b) {
    EntradaIndiceInvertido *entradaA = *(EntradaIndiceInvertido **)a;
    EntradaIndiceInvertido *entradaB = *(EntradaIndiceInvertido **)b;
    return strcmp(entradaA->palavra, entradaB->palavra);
}


/*--------------------------------------Funções Indice Invertido Hash----------------------------------------------------------------*/
// Função para atualizar ocorrências de uma palavra
void atualizar_ocorrencias(EntradaIndiceInvertido *entrada, int idDoc, int quantidade) {
    for (int i = 0; i < entrada->cont_ocorrencias; i++) {
        if (entrada->ocorrencias[i].idDoc == idDoc) {
            entrada->ocorrencias[i].quantidade += quantidade;
            return;
        }
    }

    if (entrada->cont_ocorrencias == entrada->capacidade_ocorrencias) {
        entrada->capacidade_ocorrencias *= 2;
        entrada->ocorrencias = (Ocorrencia *)realloc(entrada->ocorrencias, entrada->capacidade_ocorrencias * sizeof(Ocorrencia));
    }
    entrada->ocorrencias[entrada->cont_ocorrencias].idDoc = idDoc;
    entrada->ocorrencias[entrada->cont_ocorrencias].quantidade = quantidade;
    entrada->cont_ocorrencias++;
}
// Função para inserir uma palavra na tabela hash do índice invertido
void inserir_hash(IndiceInvertidoHash *hash, const char *palavra, int idDoc, int quantidade) {
    unsigned int indice = funcao_hash(palavra, hash->capacidade);

    EntradaIndiceInvertido *atual = hash->tabela[indice];
    while (atual!= NULL) {
        if (strcmp(atual->palavra, palavra) == 0) {
            atualizar_ocorrencias(atual, idDoc, quantidade);
            return;
        }
        atual = atual->prox;
    }

    EntradaIndiceInvertido *entrada = criar_nova_entrada(palavra);
    atualizar_ocorrencias(entrada, idDoc, quantidade);
    adicionar_entrada_hash(hash, entrada);
}
// Função para criar uma nova entrada no índice invertido
EntradaIndiceInvertido *criar_nova_entrada(const char *palavra) {
    EntradaIndiceInvertido *entrada = (EntradaIndiceInvertido *)malloc(sizeof(EntradaIndiceInvertido));
    entrada->palavra = strdup(palavra);
    entrada->cont_ocorrencias = 0;
    entrada->capacidade_ocorrencias = 10;
    entrada->ocorrencias = (Ocorrencia *)malloc(entrada->capacidade_ocorrencias * sizeof(Ocorrencia));
    entrada->prox = NULL;
    return entrada;
}
// Função para construir os índices invertidos
void construir_indices_invertidos(ListaArquivos *lista_arquivos, IndiceInvertidoHash *indice_hash, int *id_doc) {
    for (int i = 0; i < lista_arquivos->cont_arquivos; i++) {
        char *nome_arquivo = lista_arquivos->nomes_arquivos[i];
        FILE *arquivo = fopen(nome_arquivo, "r");
        if (!arquivo) {
            fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome_arquivo);
            continue;
        }

        char linha[2048];
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            continue;
        }

        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            continue;
        }

        char *ingrediente = strtok(linha, ";");
        while (ingrediente) {
            while (isspace((unsigned char)*ingrediente)) ingrediente++;
            char *end = ingrediente + strlen(ingrediente) - 1;
            while (end > ingrediente && isspace((unsigned char)*end)) end--;
            *(end + 1) = '\0';

            remover_ponto_final(ingrediente);
            inserir_hash(indice_hash, ingrediente, *id_doc, 1);
            
            ingrediente = strtok(NULL, ";");
        }

        // Reabre o arquivo para contagem detalhada
        rewind(arquivo);

        // Ignora a primeira linha
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            continue;
        }

        // Processa a segunda linha novamente para contar as ocorrências de cada ingrediente
        if (fgets(linha, sizeof(linha), arquivo) != NULL) {
            char *ingrediente = strtok(linha, ";");
            while (ingrediente) {
                while (isspace((unsigned char)*ingrediente)) ingrediente++;
                char *end = ingrediente + strlen(ingrediente) - 1;
                while (end > ingrediente && isspace((unsigned char)*end)) end--;
                *(end + 1) = '\0';
                remover_ponto_final(ingrediente);
                // Conta as ocorrências do ingrediente no arquivo
                int quantidade = 0;
                char temp_linha[2048];
                FILE *temp_arquivo = fopen(nome_arquivo, "r");
                if (temp_arquivo) {
                    while (fgets(temp_linha, sizeof(temp_linha), temp_arquivo)) {
                        char *p = temp_linha;
                        while ((p = strstr(p, ingrediente)) != NULL) {
                            quantidade++;
                            p += strlen(ingrediente);
                        }
                    }
                    fclose(temp_arquivo);

                    // Atualiza a contagem de ocorrências
                    unsigned int indice = funcao_hash(ingrediente, indice_hash->capacidade);
                    EntradaIndiceInvertido *entrada = indice_hash->tabela[indice];
                    while (entrada != NULL) {
                        if (strcmp(entrada->palavra, ingrediente) == 0) {
                            atualizar_ocorrencias(entrada, *id_doc, quantidade);
                            break;
                        }
                        entrada = entrada->prox;
                    }
                }

                ingrediente = strtok(NULL, ";");
            }
        }

        fclose(arquivo);
        (*id_doc)++;
    }
}
// Função para imprimir os índices invertidos
void imprimir_indices_invertidos(IndiceInvertidoHash *indice_hash) {
    int cont_entradas = 0;
    for (int i = 0; i < indice_hash->capacidade; i++) {
        EntradaIndiceInvertido *entrada = indice_hash->tabela[i];
        while (entrada != NULL) {
            cont_entradas++;
            entrada = entrada->prox;
        }
    }

    EntradaIndiceInvertido **entradas = (EntradaIndiceInvertido **)malloc(cont_entradas * sizeof(EntradaIndiceInvertido *));
    int index = 0;
    for (int i = 0; i < indice_hash->capacidade; i++) {
        EntradaIndiceInvertido *entrada = indice_hash->tabela[i];
        while (entrada != NULL) {
            entradas[index++] = entrada;
            entrada = entrada->prox;
        }
    }

    qsort(entradas, cont_entradas, sizeof(EntradaIndiceInvertido *), comparar_palavras);

    for (int i = 0; i < cont_entradas; i++) {
        EntradaIndiceInvertido *entrada = entradas[i];
        printf("%s: ", entrada->palavra);
        for (int j = 0; j < entrada->cont_ocorrencias; j++) {
            printf("<%d, %d>", entrada->ocorrencias[j].quantidade, entrada->ocorrencias[j].idDoc + 1);
            if (j < entrada->cont_ocorrencias - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }

    free(entradas);
}
// Função para buscar se uma entrada já existe na tabela hash
EntradaIndiceInvertido *buscar_entrada_hash(IndiceInvertidoHash *hash, const char *palavra) {
    unsigned int indice = funcao_hash(palavra, hash->capacidade);

    EntradaIndiceInvertido *atual = hash->tabela[indice];
    while (atual != NULL) {
        if (strcmp(atual->palavra, palavra) == 0) {
            return atual;
        }
        atual = atual->prox;
    }

    return NULL; // Não encontrado
}