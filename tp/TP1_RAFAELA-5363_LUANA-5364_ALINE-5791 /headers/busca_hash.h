#ifndef BUSCA_HASH_H
#define BUSCA_HASH_H

static double *relevancias_global;

void buscar_e_calcular_relevancia(IndiceInvertidoHash *indice_hash, const char **termos, int num_termos, ListaArquivos *lista_arquivos);
double calcular_tf(int frequencia, int total_termos);
double calcular_idf(int num_documentos, int documentos_contendo_termo);
double calcular_tf_idf(double tf, double idf);
double calcular_relevancia(int total_termos_documento, double *tf_idfs, int num_termos) ;
int comparar_relevancia(const void *a, const void *b);

#endif // BUSCA_HASH_H