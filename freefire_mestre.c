// mestre.c
// PLANO DE FUGA – CÓDIGO DA ILHA (NÍVEL MESTRE)
// Versão modularizada em um único arquivo, sem uso de static

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =====================================================================
   DEFINIÇÕES BÁSICAS
   ===================================================================== */
#define MAX_COMP 10

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade; // 1–5
} Componente;

/* =====================================================================
   PROTÓTIPOS
   ===================================================================== */
void limparBufferEntrada(void);
void pauseEnter(void);
void tirarQuebra(char *s);
void imprimirCabecalho(int qtd, int ordenadoPorNome);
void imprimirTabela(const Componente *v, int n);
int cadastrar(Componente *arr, int n);
int descartar(Componente *arr, int n);
void listar(const Componente *arr, int n);
long bubbleSortPorNome(Componente *v, int n);
long insertionPorTipo(Componente *v, int n);
long selectionPorPrioridade(Componente *v, int n);
void organizar(Componente *base, int n, int *ordenadoPorNome);
void buscaBinariaNome(const Componente *base, int n, int ordenadoPorNome);

/* =====================================================================
   IMPLEMENTAÇÕES – UTILITÁRIOS
   ===================================================================== */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseEnter(void) {
    printf("\nPressione Enter para continuar...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void tirarQuebra(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

/* =====================================================================
   INTERFACE – CABEÇALHO E TABELAS
   ===================================================================== */
void imprimirCabecalho(int qtd, int ordenadoPorNome) {
    printf("====================================================\n");
    printf("  PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
    printf("====================================================\n");
    printf("Itens na Mochila: %d/%d\n", qtd, MAX_COMP);
    printf("Status da Ordenacao por Nome: %s\n\n",
           ordenadoPorNome ? "ORDENADO" : "NAO ORDENADO");
    printf("1. Adicionar Componente\n");
    printf("2. Descartar Componente\n");
    printf("3. Listar Componentes (Inventario)\n");
    printf("4. Organizar Mochila (Ordenar Componentes)\n");
    printf("5. Busca Binaria por Componente-Chave (por nome)\n");
    printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
    printf("----------------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

void imprimirTabela(const Componente *v, int n) {
    printf("\n--- INVENTARIO ATUAL (%d/%d) ---\n", n, MAX_COMP);
    printf("----------------------------------------------------\n");
    printf("%-20s | %-10s | %-10s | %-10s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("----------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-20s | %-10s | %-10d | %-10d\n",
               v[i].nome, v[i].tipo, v[i].quantidade, v[i].prioridade);
    }

    printf("----------------------------------------------------\n");
}

/* =====================================================================
   FUNÇÕES DE MOCHILA (CRUD)
   ===================================================================== */
int cadastrar(Componente *arr, int n) {
    if (n >= MAX_COMP) {
        printf("\nMochila cheia. Nao e possivel adicionar.\n");
        return n;
    }

    char buf[64];
    printf("\n--- Coletando Novo Componente ---\n");

    printf("Nome: ");
    fgets(arr[n].nome, sizeof(arr[n].nome), stdin);
    tirarQuebra(arr[n].nome);

    printf("Tipo (Estrutural, Eletronico, Energia): ");
    fgets(arr[n].tipo, sizeof(arr[n].tipo), stdin);
    tirarQuebra(arr[n].tipo);

    printf("Quantidade: ");
    fgets(buf, sizeof(buf), stdin);
    tirarQuebra(buf);
    arr[n].quantidade = atoi(buf);

    do {
        printf("Prioridade de Montagem (1-5): ");
        fgets(buf, sizeof(buf), stdin);
        tirarQuebra(buf);
        arr[n].prioridade = atoi(buf);
    } while (arr[n].prioridade < 1 || arr[n].prioridade > 5);

    printf("\nComponente '%s' adicionado!\n", arr[n].nome);
    n++;
    imprimirTabela(arr, n);
    pauseEnter();
    return n;
}

int descartar(Componente *arr, int n) {
    if (n == 0) {
        printf("\nMochila vazia.\n");
        pauseEnter();
        return n;
    }

    char alvo[30];
    printf("\nNome do componente a descartar: ");
    fgets(alvo, sizeof(alvo), stdin);
    tirarQuebra(alvo);

    int pos = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].nome, alvo) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Componente '%s' nao encontrado.\n", alvo);
    } else {
        for (int i = pos; i < n - 1; i++) arr[i] = arr[i + 1];
        n--;
        printf("Componente '%s' descartado!\n", alvo);
    }

    pauseEnter();
    return n;
}

void listar(const Componente *arr, int n) {
    imprimirTabela(arr, n);
    pauseEnter();
}

/* =====================================================================
   ORDENAÇÕES
   ===================================================================== */
long bubbleSortPorNome(Componente *v, int n) {
    long comps = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            comps++;
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                Componente t = v[j];
                v[j] = v[j + 1];
                v[j + 1] = t;
            }
        }
    }
    return comps;
}

long insertionPorTipo(Componente *v, int n) {
    long comps = 0;
    for (int i = 1; i < n; i++) {
        Componente k = v[i];
        int j = i - 1;
        while (j >= 0) {
            comps++;
            if (strcmp(v[j].tipo, k.tipo) > 0) {
                v[j + 1] = v[j];
                j--;
            } else break;
        }
        v[j + 1] = k;
    }
    return comps;
}

long selectionPorPrioridade(Componente *v, int n) {
    long comps = 0;
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            comps++;
            if (v[j].prioridade < v[min].prioridade) min = j;
        }
        if (min != i) {
            Componente t = v[i];
            v[i] = v[min];
            v[min] = t;
        }
    }
    return comps;
}

void organizar(Componente *base, int n, int *ordenadoPorNome) {
    if (n == 0) {
        printf("\nMochila vazia.\n");
        pauseEnter();
        return;
    }

    printf("\n--- Estrategia de Organizacao ---\n");
    printf("Como deseja ordenar os componentes?\n");
    printf("1. Por Nome (Ordem Alfabetica)\n");
    printf("2. Por Tipo\n");
    printf("3. Por Prioridade de Montagem\n");
    printf("0. Cancelar\n");
    printf("Escolha o criterio: ");

    char buf[32];
    fgets(buf, sizeof(buf), stdin);
    tirarQuebra(buf);
    int op = atoi(buf);

    long comps = 0;
    if (op == 1) {
        comps = bubbleSortPorNome(base, n);
        printf("\nMochila organizada por NOME.\n");
        printf("Analise de Desempenho: Foram necessarias %ld comparacoes.\n", comps);
        *ordenadoPorNome = 1;
        imprimirTabela(base, n);
    } else if (op == 2) {
        comps = insertionPorTipo(base, n);
        printf("\nMochila organizada por TIPO.\n");
        printf("Analise de Desempenho: Foram necessarias %ld comparacoes.\n", comps);
        *ordenadoPorNome = 0;
        imprimirTabela(base, n);
    } else if (op == 3) {
        comps = selectionPorPrioridade(base, n);
        printf("\nMochila organizada por PRIORIDADE.\n");
        printf("Analise de Desempenho: Foram necessarias %ld comparacoes.\n", comps);
        *ordenadoPorNome = 0;
        imprimirTabela(base, n);
    }

    pauseEnter();
}

/* =====================================================================
   BUSCA BINÁRIA
   ===================================================================== */
void buscaBinariaNome(const Componente *base, int n, int ordenadoPorNome) {
    if (n == 0) {
        printf("\nMochila vazia.\n");
        pauseEnter();
        return;
    }
    if (!ordenadoPorNome) {
        printf("\nALERTA: A busca binaria requer que a mochila esteja ordenada por NOME.\n");
        printf("Use a Opcao 4 para organizar a mochila primeiro.\n");
        pauseEnter();
        return;
    }

    char chave[30];
    printf("\n--- Busca Binaria por Componente-Chave ---\n");
    printf("Nome do componente a buscar: ");
    fgets(chave, sizeof(chave), stdin);
    tirarQuebra(chave);

    int ini = 0, fim = n - 1;
    long comps = 0;
    int pos = -1;

    while (ini <= fim) {
        int meio = ini + (fim - ini) / 2;
        comps++;
        int cmp = strcmp(base[meio].nome, chave);
        if (cmp == 0) {
            pos = meio;
            break;
        } else if (cmp > 0)
            fim = meio - 1;
        else
            ini = meio + 1;
    }

    if (pos != -1) {
        printf("\n--- Componente-Chave Encontrado! ---\n");
        printf("Nome: %s, Tipo: %s, Qtd: %d, Prio: %d\n",
               base[pos].nome, base[pos].tipo, base[pos].quantidade, base[pos].prioridade);
    } else {
        printf("\nComponente nao encontrado.\n");
    }
    pauseEnter();
}

/* =====================================================================
   MAIN
   ===================================================================== */
int main(void) {
    Componente mochila[MAX_COMP];
    int n = 0;
    int ordenadoPorNome = 0;
    char buf[32];
    int opc;

    do {
        imprimirCabecalho(n, ordenadoPorNome);
        fgets(buf, sizeof(buf), stdin);
        tirarQuebra(buf);
        opc = atoi(buf);

        if (opc == 1) {
            int antes = n;
            n = cadastrar(mochila, n);
            if (n != antes) ordenadoPorNome = 0;
        } else if (opc == 2) {
            int antes = n;
            n = descartar(mochila, n);
            if (n != antes) ordenadoPorNome = 0;
        } else if (opc == 3) {
            listar(mochila, n);
        } else if (opc == 4) {
            organizar(mochila, n, &ordenadoPorNome);
        } else if (opc == 5) {
            buscaBinariaNome(mochila, n, ordenadoPorNome);
        } else if (opc == 0) {
            // sair
        } else {
            printf("\nOpcao invalida!\n");
            pauseEnter();
        }
    } while (opc != 0);

    return 0;
}
