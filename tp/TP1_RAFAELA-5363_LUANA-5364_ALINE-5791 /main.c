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
#include <string.h>
#include "headers/file_reader.h"
#include "headers/hash.h"
#include "headers/patricia.h"
#include "headers/menu.h"

int main() {


    ListaArquivos *lista_arquivos = NULL;
    // Contar o total de termos em cada arquivo
    IndiceInvertidoHash *indice_hash = criar_tabela_hash(100);
    Patricia arvore_patricia = criar_arvore_patricia();
    int id_doc = 0;
    int idDocPat = 0; 
    mostrar_menu(&lista_arquivos, indice_hash, &arvore_patricia, &id_doc, &idDocPat);

    liberar_lista_arquivos(lista_arquivos);

    liberar_tabela_hash(indice_hash);
    //liberar_arvore_patricia(arvore_patricia);
    free(arvore_patricia);

    return 0;
}