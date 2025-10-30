//Desafio Free Fire

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 10 // Define o número máximo de slots na mochila

// --------------------------------------------------
// ESTRUTURAS E PROTÓTIPOS
// --------------------------------------------------

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;  // Quantidade do próprio item
} Item;

void abrirInventario(Item *inventario); // Zera todas as posições (mochila vazia)

void inserirItem(Item *inventario, const char* item);
void removerItem(Item *inventario, const char* item);
void listarItem(const Item *inventario);
void buscarItem(const Item *inventario, const char* item);

// Contador simples (fora da struct) e cabeçalho estilizado
int  contarItens(const Item *inventario);
void mostrarStatus(int qtd);

// Utilitários de entrada/pausa
void limparBufferEntrada(void);
void pauseEnter(void);

// --------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES - INVENTÁRIO (VETOR DE STRUCTS)
// --------------------------------------------------

void abrirInventario(Item *inventario) {
    for (int i = 0; i < TAM_MAX; i++) {
        inventario[i].nome[0] = '\0';
        inventario[i].tipo[0] = '\0';
        inventario[i].quantidade = 0;
    }
}

// Insere um novo Item usando o nome recebido em 'item'.
void inserirItem(Item *inventario, const char* item) {
    printf("\n--- Adicionar Novo Item ---\n");

    // Verifica duplicidade de nome
    for (int i = 0; i < TAM_MAX; i++) {
        if (inventario[i].nome[0] != '\0' && strcmp(inventario[i].nome, item) == 0) {
            printf("Erro: Ja existe um item chamado \"%s\" no inventario.\n", item);
            return;
        }
    }

    // Encontra primeira posicao vazia
    int posLivre = -1;
    for (int i = 0; i < TAM_MAX; i++) {
        if (inventario[i].nome[0] == '\0') {
            posLivre = i;
            break;
        }
    }

    if (posLivre == -1) {
        printf("Erro: Inventario cheio! Nao e possivel inserir.\n");
        return;
    }

    char tipo[20];
    int qtd;

    printf("Nome do item: %s\n", item);
    printf("Tipo do item (arma, municao, cura, etc.): ");
    scanf(" %19[^\n]", tipo);
    printf("Quantidade: ");
    scanf("%d", &qtd);
    limparBufferEntrada();

    // Copia dados para a posicao livre
    strcpy(inventario[posLivre].nome, item);
    strcpy(inventario[posLivre].tipo, tipo);
    inventario[posLivre].quantidade = qtd;

    printf("\nItem '%s' adicionado com sucesso!\n", item);
}

// Remove um item pelo nome. Fecha a lacuna deslocando os seguintes para a esquerda.
void removerItem(Item *inventario, const char* item) {
    printf("\n--- Remover Item ---\n");
    printf("Digite o nome do item a ser removido: %s\n", item);

    int pos = -1;

    for (int i = 0; i < TAM_MAX; i++) {
        if (inventario[i].nome[0] != '\0' && strcmp(inventario[i].nome, item) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Erro: Item \"%s\" nao encontrado no inventario.\n", item);
        return;
    }

    // Desloca itens seguintes uma posicao para a esquerda
    for (int i = pos; i < TAM_MAX - 1; i++) {
        inventario[i] = inventario[i + 1];
    }

    // Zera a ultima posicao (agora vaga)
    inventario[TAM_MAX - 1].nome[0] = '\0';
    inventario[TAM_MAX - 1].tipo[0] = '\0';
    inventario[TAM_MAX - 1].quantidade = 0;

    printf("\nItem '%s' removido com sucesso!\n", item);
}

// Lista todos os itens em formato de tabela e mostra o total (X/10)
void listarItem(const Item *inventario) {
    int total = contarItens(inventario);

    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", total, TAM_MAX);
    printf("------------------------------------------------\n");
    printf("%-20s | %-12s | %-10s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("------------------------------------------------\n");

    int vazia = 1;
    for (int i = 0; i < TAM_MAX; i++) {
        if (inventario[i].nome[0] != '\0') {
            vazia = 0;
            printf("%-20s | %-12s | %-10d\n",
                   inventario[i].nome,
                   inventario[i].tipo,
                   inventario[i].quantidade);
        }
    }
    if (vazia) {
        printf("(vazio)\n");
    }
    printf("------------------------------------------------\n");
}

// Busca sequencial por nome e exibe os dados do item encontrado
void buscarItem(const Item *inventario, const char* item) {
    printf("\n--- Buscar Item ---\n");
    printf("Procurando por: %s\n", item);

    for (int i = 0; i < TAM_MAX; i++) {
        if (inventario[i].nome[0] != '\0' && strcmp(inventario[i].nome, item) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s\n", inventario[i].nome);
            printf("Tipo: %s\n", inventario[i].tipo);
            printf("Quantidade: %d\n", inventario[i].quantidade);
            return;
        }
    }
    printf("\nItem \"%s\" nao encontrado na mochila.\n", item);
}

// --------------------------------------------------
// FUNÇÕES AUXILIARES (contador, cabeçalho e entrada)
// --------------------------------------------------

int contarItens(const Item *inventario) {
    int c = 0;
    for (int i = 0; i < TAM_MAX; i++) {
        if (inventario[i].nome[0] != '\0') c++;
    }
    return c;
}

void mostrarStatus(int qtd) {
    printf("\n==========================================\n");
    printf(" MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
    printf("==========================================\n");
    printf("Itens na Mochila: %d/%d\n", qtd, TAM_MAX);
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseEnter(void) {
    printf("\nPressione Enter para continuar...");
    fflush(stdout);
    int c;
    // Garante esperar uma nova linha
    while ((c = getchar()) != '\n' && c != EOF);
}

// --------------------------------------------------
// MENUS E FUNÇÃO PRINCIPAL
// --------------------------------------------------

void menuInventario() {
    Item inventario[TAM_MAX];
    abrirInventario(inventario);

    int opcao;
    do {
        // Cabeçalho com contador (X/10) antes do menu
        mostrarStatus(contarItens(inventario));

        printf("\n1. Adicionar Item (Loot)\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens na Mochila\n");
        printf("4. Buscar Item\n");
        printf("0. Sair\n");
        printf("--------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1: {
                char nome[30];
                printf("\n--- Inserir Novo Item ---\n");
                printf("\nDigite o nome do item: ");
                scanf(" %29[^\n]", nome);
                limparBufferEntrada();

                inserirItem(inventario, nome);
                listarItem(inventario);
                pauseEnter(); // pausa como nas imagens
                break;
            }
            case 2: {
                char nome[30];
                printf("\n--- Remover Item ---\n");
                printf("\nDigite o nome do item a remover: ");
                scanf(" %29[^\n]", nome);
                limparBufferEntrada();

                removerItem(inventario, nome);
                listarItem(inventario);
                pauseEnter();
                break;
            }
            case 3:
                listarItem(inventario);
                pauseEnter();
                break;
            case 4: {
                char nome[30];
                printf("\n--- Busca de Item ---\n");
                printf("\nDigite o nome do item a buscar: ");
                scanf(" %29[^\n]", nome);
                limparBufferEntrada();

                buscarItem(inventario, nome);
                pauseEnter();
                break;
            }
            case 0:
                /* Saindo sem mensagem extra (como nas prints) */
                break;
            default:
                printf("Opcao invalida!\n");
                pauseEnter();
        }

    } while (opcao != 0);
}

int main() {
    menuInventario();
    return 0;
}
