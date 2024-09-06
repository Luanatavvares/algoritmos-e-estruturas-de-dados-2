#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define TAMANHOTABELA 7
#define N 13   // tamanho máximo do nome
#define VAZIO "!!!!!!!!!!"

typedef unsigned TipoPesos[N];
typedef unsigned int TipoIndice;

typedef struct {
    char nome[N];
    char codigo[N];
    int seekings; 
} Tripulante;

typedef Tripulante TABELA[TAMANHOTABELA];
TABELA TabelaHash;

void GeraPesos(TipoPesos p) {
    int i;
    struct timeval semente;
    gettimeofday(&semente, NULL);
    srand((int)(semente.tv_sec + 1000000 * semente.tv_usec));
    for (i = 0; i < N; i++)
        p[i] = 1 + (int)(10000.0 * rand() / (RAND_MAX + 1.0));
}

// hashing universal
TipoIndice h(char *nomeTripulante, TipoPesos p) {
    int i;
    unsigned int Soma = 0;
    int numerocaracteres = strlen(nomeTripulante);
    for (i = 0; i < numerocaracteres; i++)
        Soma += (unsigned int)nomeTripulante[i] * p[i];
    return (Soma % TAMANHOTABELA);
}

void Inicializa(TABELA T) {
    int i;
    for (i = 0; i < TAMANHOTABELA; i++) {
        memcpy(T[i].nome, VAZIO, N);
        T[i].seekings = 0; 
    }
}

int Insere(Tripulante *Tripulante, TABELA T, TipoPesos p) {
    TipoIndice Posicao;
    TipoIndice Inicio;
    int seekings = 0;
    Posicao = Inicio = h(Tripulante->nome, p);

    do {
        seekings++;
        if (strcmp(T[Posicao].nome, VAZIO) == 0) {
            memcpy(T[Posicao].nome, Tripulante->nome, N);
            memcpy(T[Posicao].codigo, Tripulante->codigo, N);
            T[Posicao].seekings = seekings; 
            return seekings; 
        }
        Posicao = (Posicao + 1) % TAMANHOTABELA;
    } while (Posicao != Inicio);

    return -1; 
}

int Busca(char *nomeTripulante, TABELA T, TipoPesos p) {
    TipoIndice Posicao;
    TipoIndice Inicio;
    int seekings = 0; 
    Posicao = Inicio = h(nomeTripulante, p);

    do {
        seekings++;
        if (strcmp(T[Posicao].nome, nomeTripulante) == 0) {
            T[Posicao].seekings = seekings; 
            return Posicao; 
        }
        Posicao = (Posicao + 1) % TAMANHOTABELA;
    } while (Posicao != Inicio);

    return -1; 
}

void ExibeTabela(TABELA T) {
    printf("Dados da Tabela Hash:\n");
    for (int i = 0; i < TAMANHOTABELA; i++) {
        if (strcmp(T[i].nome, VAZIO) != 0) {
            printf("[%d] Nome: %s, Código: %s, Seekings: %d\n", i, T[i].nome, T[i].codigo, T[i].seekings);
        } else {
            printf("[%d] VAZIO\n", i);
        }
    }
}

void CarregaDados(char *nomeArquivo, TipoPesos pesos) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    Tripulante novoTripulante;
    Inicializa(TabelaHash);

    while (fscanf(arquivo, "%s %s", novoTripulante.nome, novoTripulante.codigo) == 2) {
        int posicao = Insere(&novoTripulante, TabelaHash, pesos);
        if (posicao == -1) {
            printf("Tabela cheia. Não foi possível inserir todos os tripulantes.\n");
            break;
        }
    }

    fclose(arquivo);
}

int main() {
    char nomeTripulante[N];
    char nomeArquivo[] = "tripulacao.txt";
    TipoPesos pesos; 

    GeraPesos(pesos); 
    CarregaDados(nomeArquivo, pesos);
    ExibeTabela(TabelaHash);

    printf("Digite o nome do tripulante para buscar: ");
    scanf("%s", nomeTripulante);

    int posicao = Busca(nomeTripulante, TabelaHash, pesos);
    if (posicao != -1) {
        printf("Tripulante encontrado:\n");
        printf("Nome: %s\n", TabelaHash[posicao].nome);
        printf("Código: %s\n", TabelaHash[posicao].codigo);
        printf("Seekings realizados: %d\n", TabelaHash[posicao].seekings);
    } else {
        printf("Tripulante não encontrado.\n");
    }

    return 0;
}
