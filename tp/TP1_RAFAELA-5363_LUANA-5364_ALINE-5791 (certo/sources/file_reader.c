#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include "../headers/file_reader.h"

ListaArquivos* ler_lista_arquivos(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(EXIT_FAILURE);
    }

    int cont_arquivos;
    fscanf(arquivo, "%d\n", &cont_arquivos);

    ListaArquivos *lista_arquivos = (ListaArquivos*)malloc(sizeof(ListaArquivos));
    lista_arquivos->cont_arquivos = cont_arquivos;
    lista_arquivos->nomes_arquivos = (char**)malloc(cont_arquivos * sizeof(char*));
    lista_arquivos->total_termos = (int*)malloc(cont_arquivos * sizeof(int)); // Inicializa o array para total de termos

    for (int i = 0; i < cont_arquivos; i++) {
        lista_arquivos->nomes_arquivos[i] = (char*)malloc(256 * sizeof(char));
        fscanf(arquivo, "%s\n", lista_arquivos->nomes_arquivos[i]);

        // Adiciona o caminho da subpasta 'pocoes' ao nome do arquivo
        char caminho_completo[512];
        snprintf(caminho_completo, sizeof(caminho_completo), "pocoes/%s", lista_arquivos->nomes_arquivos[i]);
        strcpy(lista_arquivos->nomes_arquivos[i], caminho_completo);

        // Verifica se o arquivo pode ser aberto
        FILE *arquivo_teste = fopen(caminho_completo, "r");
        if (arquivo_teste) {
            fclose(arquivo_teste);
        } else {
            printf("Erro ao abrir o arquivo %d: %s.\n", i + 1, caminho_completo);
        }
    }

    fclose(arquivo);
    return lista_arquivos;
}


void liberar_lista_arquivos(ListaArquivos *lista_arquivos) {
    for (int i = 0; i < lista_arquivos->cont_arquivos; i++) {
        free(lista_arquivos->nomes_arquivos[i]);
    }
    free(lista_arquivos->nomes_arquivos);
    free(lista_arquivos);
}

char** ler_arquivo_receita(const char *nome_arquivo, int *num_ingredientes, int *num_passos) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(EXIT_FAILURE);
    }

    char **partes_receita = (char**)malloc(3 * sizeof(char*));

    for (int i = 0; i < 3; i++) {
        partes_receita[i] = (char*)malloc(1024 * sizeof(char));
        fgets(partes_receita[i], 1024, arquivo);
    }

    fclose(arquivo);

    *num_ingredientes = 1;
    *num_passos = 1;

    for (char *c = partes_receita[1]; *c != '\0'; c++) {
        if (*c == ';') (*num_ingredientes)++;
    }

    for (char *c = partes_receita[2]; *c != '\0'; c++) {
        if (*c == ';') (*num_passos)++;
    }

    return partes_receita;
}


void contar_total_termos(ListaArquivos *lista_arquivos) {
    for (int i = 0; i < lista_arquivos->cont_arquivos; i++) {
        char *nome_arquivo = lista_arquivos->nomes_arquivos[i];
        FILE *arquivo = fopen(nome_arquivo, "r");
        if (!arquivo) {
            fprintf(stderr, "Erro ao abrir o arquivo %s\n", nome_arquivo);
            continue;
        }

        char linha[2048];
        int total_termos = 0;

        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            lista_arquivos->total_termos[i] = 0;
            continue;
        }

        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            fclose(arquivo);
            lista_arquivos->total_termos[i] = 0;
            continue;
        }

        char *termo = strtok(linha, ";");
        while (termo) {
            while (isspace((unsigned char)*termo)) termo++;
            char *end = termo + strlen(termo) - 1;
            while (end > termo && isspace((unsigned char)*end)) end--;
            *(end + 1) = '\0';

            if (strlen(termo) > 0) {
                total_termos++;
            }

            termo = strtok(NULL, ";");
        }

        fclose(arquivo);

        lista_arquivos->total_termos[i] = total_termos;
    }
}