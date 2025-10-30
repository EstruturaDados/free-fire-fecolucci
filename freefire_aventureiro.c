// mochila.c
// Desafio Free Fire - Nível Aventureiro (Etapa 2)
// Modularizado em um único arquivo com seções (protótipos + implementações)

/* ======================================================================
   INCLUDES E DEFINIÇÕES BÁSICAS
   ====================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // (opcional) para medir tempo

#define TAM_MAX 10

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Lista encadeada
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

/* ======================================================================
   PROTÓTIPOS – UTILITÁRIOS
   ====================================================================== */
void limparBufferEntrada(void);
void pauseEnter(void);

/* ======================================================================
   PROTÓTIPOS – MOCHILA (VETOR / SEQUENCIAL)
   ====================================================================== */
void abrirInventarioVetor(Item *vetor);
int  contarItensVetor(const Item *vetor);
void inserirItemVetor(Item *vetor, const char* nome, const char* tipo, int qtd);
void removerItemVetor(Item *vetor, const char* nome);
void listarVetor(const Item *vetor);
int  buscarSequencialVetor(const Item *vetor, const char* nome, int *comparacoes);
void ordenarVetorPorNome(Item *vetor);
int  buscarBinariaVetor(const Item *vetor, const char* nome, int *comparacoes);

/* ======================================================================
   PROTÓTIPOS – MOCHILA (LISTA ENCADEADA / DINÂMICA)
   ====================================================================== */
No* criarNo(const char* nome, const char* tipo, int qtd);
void liberarLista(No **inicio);
int  contarItensLista(No *inicio);
void inserirItemLista(No **inicio, const char* nome, const char* tipo, int qtd);
void removerItemLista(No **inicio, const char* nome);
void listarLista(No *inicio);
No* buscarSequencialLista(No *inicio, const char* nome, int *comparacoes);

/* ======================================================================
   PROTÓTIPOS – MENUS
   ====================================================================== */
void menuVetor(void);
void menuLista(void);
void menuPrincipal(void);

/* ======================================================================
   IMPLEMENTAÇÕES – UTILITÁRIOS (I/O)
   ====================================================================== */
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

/* ======================================================================
   IMPLEMENTAÇÕES – MOCHILA (VETOR / SEQUENCIAL)
   ====================================================================== */
void abrirInventarioVetor(Item *vetor) {
    for (int i = 0; i < TAM_MAX; i++) {
        vetor[i].nome[0] = '\0';
        vetor[i].tipo[0] = '\0';
        vetor[i].quantidade = 0;
    }
}

int contarItensVetor(const Item *vetor) {
    int c = 0;
    for (int i = 0; i < TAM_MAX; i++) {
        if (vetor[i].nome[0] != '\0') c++;
    }
    return c;
}

// Insere em primeira posição livre, sem duplicar nome
void inserirItemVetor(Item *vetor, const char* nome, const char* tipo, int qtd) {
    // verificar duplicidade
    for (int i = 0; i < TAM_MAX; i++) {
        if (vetor[i].nome[0] != '\0' && strcmp(vetor[i].nome, nome) == 0) {
            printf("Erro: Ja existe um item chamado \"%s\" no vetor.\n", nome);
            return;
        }
    }
    // achar vaga
    int posLivre = -1;
    for (int i = 0; i < TAM_MAX; i++) {
        if (vetor[i].nome[0] == '\0') { posLivre = i; break; }
    }
    if (posLivre == -1) {
        printf("Erro: Vetor cheio! Nao e possivel inserir.\n");
        return;
    }
    strcpy(vetor[posLivre].nome, nome);
    strcpy(vetor[posLivre].tipo, tipo);
    vetor[posLivre].quantidade = qtd;
    printf("Item \"%s\" inserido no vetor.\n", nome);
}

// Remove por nome e compacta (desloca à esquerda)
void removerItemVetor(Item *vetor, const char* nome) {
    int pos = -1;
    for (int i = 0; i < TAM_MAX; i++) {
        if (vetor[i].nome[0] != '\0' && strcmp(vetor[i].nome, nome) == 0) {
            pos = i; break;
        }
    }
    if (pos == -1) {
        printf("Erro: \"%s\" nao encontrado no vetor.\n", nome);
        return;
    }
    for (int i = pos; i < TAM_MAX - 1; i++) {
        vetor[i] = vetor[i + 1];
    }
    vetor[TAM_MAX - 1].nome[0] = '\0';
    vetor[TAM_MAX - 1].tipo[0] = '\0';
    vetor[TAM_MAX - 1].quantidade = 0;
    printf("Item \"%s\" removido do vetor.\n", nome);
}

void listarVetor(const Item *vetor) {
    int total = contarItensVetor(vetor);
    printf("\n--- MOCHILA (VETOR) %d/%d ---\n", total, TAM_MAX);
    printf("------------------------------------------------\n");
    printf("%-20s | %-12s | %-10s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("------------------------------------------------\n");
    int vazia = 1;
    for (int i = 0; i < TAM_MAX; i++) {
        if (vetor[i].nome[0] != '\0') {
            vazia = 0;
            printf("%-20s | %-12s | %-10d\n",
                   vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
        }
    }
    if (vazia) printf("(vazio)\n");
    printf("------------------------------------------------\n");
}

// Busca sequencial no vetor, retorna índice ou -1. Conta comparações via *comparacoes
int buscarSequencialVetor(const Item *vetor, const char* nome, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < TAM_MAX; i++) {
        if (vetor[i].nome[0] != '\0') {
            (*comparacoes)++;
            if (strcmp(vetor[i].nome, nome) == 0) return i;
        }
    }
    return -1;
}

// Bubble Sort por nome (ordena apenas posições ocupadas)
void ordenarVetorPorNome(Item *vetor) {
    int n = contarItensVetor(vetor);
    if (n <= 1) { printf("Nada a ordenar.\n"); return; }

    // Copia itens ocupados para buffer denso, ordena e devolve
    Item buf[TAM_MAX];
    int k = 0;
    for (int i = 0; i < TAM_MAX; i++) {
        if (vetor[i].nome[0] != '\0') buf[k++] = vetor[i];
    }
    // bubble
    for (int i = 0; i < k - 1; i++) {
        for (int j = 0; j < k - 1 - i; j++) {
            if (strcmp(buf[j].nome, buf[j + 1].nome) > 0) {
                Item tmp = buf[j];
                buf[j] = buf[j + 1];
                buf[j + 1] = tmp;
            }
        }
    }
    // limpa vetor e devolve ordenado no início
    abrirInventarioVetor(vetor);
    for (int i = 0; i < k; i++) vetor[i] = buf[i];
    printf("Vetor ordenado por nome (Bubble Sort).\n");
}

// Busca binária no vetor (somente após ordenar). Retorna índice ou -1. Conta comparações.
int buscarBinariaVetor(const Item *vetor, const char* nome, int *comparacoes) {
    *comparacoes = 0;
    int n = contarItensVetor(vetor);
    int ini = 0, fim = n - 1;

    while (ini <= fim) {
        int meio = ini + (fim - ini) / 2;
        (*comparacoes)++;
        int cmp = strcmp(vetor[meio].nome, nome);
        if (cmp == 0) return meio;
        else if (cmp > 0) fim = meio - 1;
        else ini = meio + 1;
    }
    return -1;
}

/* ======================================================================
   IMPLEMENTAÇÕES – MOCHILA (LISTA ENCADEADA / DINÂMICA)
   ====================================================================== */
No* criarNo(const char* nome, const char* tipo, int qtd) {
    No* novo = (No*) malloc(sizeof(No));
    if (!novo) {
        printf("Erro de memoria.\n");
        return NULL;
    }
    strcpy(novo->dados.nome, nome);
    strcpy(novo->dados.tipo, tipo);
    novo->dados.quantidade = qtd;
    novo->proximo = NULL;
    return novo;
}

void liberarLista(No **inicio) {
    No* p = *inicio;
    while (p) {
        No* aux = p->proximo;
        free(p);
        p = aux;
    }
    *inicio = NULL;
}

int contarItensLista(No *inicio) {
    int c = 0;
    for (No* p = inicio; p != NULL; p = p->proximo) c++;
    return c;
}

// Insere no final (sem duplicar nome)
void inserirItemLista(No **inicio, const char* nome, const char* tipo, int qtd) {
    // checar duplicidade
    for (No* p = *inicio; p != NULL; p = p->proximo) {
        if (strcmp(p->dados.nome, nome) == 0) {
            printf("Erro: Ja existe \"%s\" na lista.\n", nome);
            return;
        }
    }
    No* novo = criarNo(nome, tipo, qtd);
    if (!novo) return;

    if (*inicio == NULL) {
        *inicio = novo;
    } else {
        No* p = *inicio;
        while (p->proximo) p = p->proximo;
        p->proximo = novo;
    }
    printf("Item \"%s\" inserido na lista.\n", nome);
}

void removerItemLista(No **inicio, const char* nome) {
    if (*inicio == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    No* p = *inicio;
    No* ant = NULL;
    while (p && strcmp(p->dados.nome, nome) != 0) {
        ant = p;
        p = p->proximo;
    }
    if (!p) {
        printf("Erro: \"%s\" nao encontrado na lista.\n", nome);
        return;
    }
    if (ant == NULL) { // remove primeiro
        *inicio = p->proximo;
    } else {
        ant->proximo = p->proximo;
    }
    free(p);
    printf("Item \"%s\" removido da lista.\n", nome);
}

void listarLista(No *inicio) {
    int total = contarItensLista(inicio);
    printf("\n--- MOCHILA (LISTA) %d itens ---\n", total);
    printf("------------------------------------------------\n");
    printf("%-20s | %-12s | %-10s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("------------------------------------------------\n");
    if (!inicio) {
        printf("(vazio)\n");
    } else {
        for (No* p = inicio; p != NULL; p = p->proximo) {
            printf("%-20s | %-12s | %-10d\n",
                   p->dados.nome, p->dados.tipo, p->dados.quantidade);
        }
    }
    printf("------------------------------------------------\n");
}

// Busca sequencial na lista; retorna ponteiro para nó ou NULL; conta comparações
No* buscarSequencialLista(No *inicio, const char* nome, int *comparacoes) {
    *comparacoes = 0;
    for (No* p = inicio; p != NULL; p = p->proximo) {
        (*comparacoes)++;
        if (strcmp(p->dados.nome, nome) == 0) return p;
    }
    return NULL;
}

/* ======================================================================
   IMPLEMENTAÇÕES – MENUS
   ====================================================================== */
void menuVetor(void) {
    Item mochila[TAM_MAX];
    abrirInventarioVetor(mochila);

    int opc;
    do {
        printf("\n=============================\n");
        printf(" MOCHILA (VETOR) - %d/%d\n", contarItensVetor(mochila), TAM_MAX);
        printf("=============================\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar sequencial (por nome)\n");
        printf("5. Ordenar por nome (Bubble Sort)\n");
        printf("6. Buscar binaria (por nome)\n");
        printf("0. Voltar\n");
        printf("-----------------------------\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) { limparBufferEntrada(); opc = -1; }
        limparBufferEntrada();

        if (opc == 1) {
            char nome[30], tipo[20]; int qtd;
            printf("Nome: "); scanf(" %29[^\n]", nome); limparBufferEntrada();
            printf("Tipo: "); scanf(" %19[^\n]", tipo); limparBufferEntrada();
            printf("Quantidade: "); scanf("%d", &qtd); limparBufferEntrada();
            inserirItemVetor(mochila, nome, tipo, qtd);
            pauseEnter();
        } else if (opc == 2) {
            char nome[30];
            printf("Nome do item a remover: ");
            scanf(" %29[^\n]", nome); limparBufferEntrada();
            removerItemVetor(mochila, nome);
            pauseEnter();
        } else if (opc == 3) {
            listarVetor(mochila);
            pauseEnter();
        } else if (opc == 4) {
            char nome[30]; int comp = 0;
            printf("Nome para busca sequencial: ");
            scanf(" %29[^\n]", nome); limparBufferEntrada();
            int pos = buscarSequencialVetor(mochila, nome, &comp);
            if (pos != -1) {
                printf("Encontrado no indice %d. Comparacoes: %d\n", pos, comp);
                printf(" -> (%s | %s | %d)\n", mochila[pos].nome, mochila[pos].tipo, mochila[pos].quantidade);
            } else {
                printf("Nao encontrado. Comparacoes: %d\n", comp);
            }
            pauseEnter();
        } else if (opc == 5) {
            ordenarVetorPorNome(mochila);
            pauseEnter();
        } else if (opc == 6) {
            char nome[30]; int comp = 0;
            printf("Nome para busca binaria (requer vetor ordenado): ");
            scanf(" %29[^\n]", nome); limparBufferEntrada();
            int pos = buscarBinariaVetor(mochila, nome, &comp);
            if (pos != -1) {
                printf("Encontrado no indice %d. Comparacoes (binaria): %d\n", pos, comp);
                printf(" -> (%s | %s | %d)\n", mochila[pos].nome, mochila[pos].tipo, mochila[pos].quantidade);
            } else {
                printf("Nao encontrado. Comparacoes (binaria): %d\n", comp);
            }
            pauseEnter();
        } else if (opc == 0) {
            // voltar
        } else {
            printf("Opcao invalida!\n");
            pauseEnter();
        }
    } while (opc != 0);
}

void menuLista(void) {
    No* inicio = NULL;

    int opc;
    do {
        printf("\n=============================\n");
        printf(" MOCHILA (LISTA ENCADEADA) - %d itens\n", contarItensLista(inicio));
        printf("=============================\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar sequencial (por nome)\n");
        printf("0. Voltar\n");
        printf("-----------------------------\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) { limparBufferEntrada(); opc = -1; }
        limparBufferEntrada();

        if (opc == 1) {
            char nome[30], tipo[20]; int qtd;
            printf("Nome: "); scanf(" %29[^\n]", nome); limparBufferEntrada();
            printf("Tipo: "); scanf(" %19[^\n]", tipo); limparBufferEntrada();
            printf("Quantidade: "); scanf("%d", &qtd); limparBufferEntrada();
            inserirItemLista(&inicio, nome, tipo, qtd);
            pauseEnter();
        } else if (opc == 2) {
            char nome[30];
            printf("Nome do item a remover: ");
            scanf(" %29[^\n]", nome); limparBufferEntrada();
            removerItemLista(&inicio, nome);
            pauseEnter();
        } else if (opc == 3) {
            listarLista(inicio);
            pauseEnter();
        } else if (opc == 4) {
            char nome[30]; int comp = 0;
            printf("Nome para busca sequencial: ");
            scanf(" %29[^\n]", nome); limparBufferEntrada();
            No* achou = buscarSequencialLista(inicio, nome, &comp);
            if (achou) {
                printf("Encontrado. Comparacoes: %d\n", comp);
                printf(" -> (%s | %s | %d)\n",
                       achou->dados.nome, achou->dados.tipo, achou->dados.quantidade);
            } else {
                printf("Nao encontrado. Comparacoes: %d\n", comp);
            }
            pauseEnter();
        } else if (opc == 0) {
            // voltar
        } else {
            printf("Opcao invalida!\n");
            pauseEnter();
        }
    } while (opc != 0);

    liberarLista(&inicio);
}

void menuPrincipal(void) {
    int opc;
    do {
        printf("\n==========================================\n");
        printf("  MOCHILA DE SOBREVIVENCIA - NIVEL AVENTUREIRO\n");
        printf("==========================================\n");
        printf("1. Versao com Vetor (lista sequencial)\n");
        printf("2. Versao com Lista Encadeada (dinamica)\n");
        printf("0. Sair\n");
        printf("------------------------------------------\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) { limparBufferEntrada(); opc = -1; }
        limparBufferEntrada();

        if (opc == 1) {
            menuVetor();
        } else if (opc == 2) {
            menuLista();
        } else if (opc == 0) {
            // sair
        } else {
            printf("Opcao invalida!\n");
            pauseEnter();
        }
    } while (opc != 0);
}

/* ======================================================================
   MAIN
   ====================================================================== */
int main(void) {
    menuPrincipal();
    return 0;
}
