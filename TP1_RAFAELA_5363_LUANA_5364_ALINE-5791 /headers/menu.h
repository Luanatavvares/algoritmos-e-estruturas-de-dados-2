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
        menu.h
        Descrição do arquivo: Arquivo que contém a função do menu que será exibido no terminal. 
        

------------------------------------------------------------------------------------------------------------------------------------------------------------ */

#ifndef MENU_H
#define MENU_H
#include "patricia.h"  
#include "hash.h"

/*--------------------------------------Funções de visualização no main----------------------------------------------------------------*/
void mostrar_menu(ListaArquivos **lista_arquivos, IndiceInvertidoHash *indice_hash, Patricia *arvore_patricia, int *id_doc, int *idDocPat);
#endif