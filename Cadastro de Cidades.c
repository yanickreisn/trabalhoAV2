#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "cidades.txt"
#define TEMP "temp.txt"

typedef struct {
    int codigo;
    char nome[50];
    char estado[30];
    int populacao;
} Cidade;

// Declaração das funções
void incluirCidade(Cidade cidade);
void alterarCidade(int codigo);
void excluirCidade(int codigo);
void consultarCidade(int codigo);
void listarCidades();

int main() {
    int opcao, codigo;
    Cidade cidade;

    do {
        printf("\n--- Sistema de Cadastro de Cidades ---\n");
        printf("1. Incluir nova cidade\n");
        printf("2. Alterar dados de uma cidade\n");
        printf("3. Excluir uma cidade\n");
        printf("4. Consultar cidade pelo código\n");
        printf("5. Listar todas as cidades\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n--- Incluir Nova Cidade ---\n");
                printf("Código: ");
                scanf("%d", &cidade.codigo);
                getchar(); // Limpar o buffer
                printf("Nome da cidade: ");
                fgets(cidade.nome, 50, stdin);
                cidade.nome[strcspn(cidade.nome, "\n")] = '\0';
                printf("Estado: ");
                fgets(cidade.estado, 30, stdin);
                cidade.estado[strcspn(cidade.estado, "\n")] = '\0';
                printf("População: ");
                scanf("%d", &cidade.populacao);
                incluirCidade(cidade);
                break;

            case 2:
                printf("\n--- Alterar Dados da Cidade ---\n");
                printf("Informe o código: ");
                scanf("%d", &codigo);
                alterarCidade(codigo);
                break;

            case 3:
                printf("\n--- Excluir Cidade ---\n");
                printf("Informe o código: ");
                scanf("%d", &codigo);
                excluirCidade(codigo);
                break;

            case 4:
                printf("\n--- Consultar Cidade ---\n");
                printf("Informe o código: ");
                scanf("%d", &codigo);
                consultarCidade(codigo);
                break;

            case 5:
                printf("\n--- Lista de Todas as Cidades ---\n");
                listarCidades();
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// Implementação das funções
void incluirCidade(Cidade cidade) {
    FILE *arquivo = fopen(ARQUIVO, "a");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fprintf(arquivo, "%d,%s,%s,%d\n", cidade.codigo, cidade.nome, cidade.estado, cidade.populacao);
    fclose(arquivo);
    printf("Cidade adicionada com sucesso!\n");
}

void alterarCidade(int codigo) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    FILE *temp = fopen(TEMP, "w");
    if (arquivo == NULL || temp == NULL) {
        perror("Erro ao abrir os arquivos");
        return;
    }

    Cidade cidade;
    int encontrado = 0;
    char linha[200];

    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%49[^,],%29[^,],%d", &cidade.codigo, cidade.nome, cidade.estado, &cidade.populacao);
        if (cidade.codigo == codigo) {
            encontrado = 1;
            printf("Novo nome da cidade: ");
            getchar(); // Limpar o buffer
            fgets(cidade.nome, 50, stdin);
            cidade.nome[strcspn(cidade.nome, "\n")] = '\0';
            printf("Novo estado: ");
            fgets(cidade.estado, 30, stdin);
            cidade.estado[strcspn(cidade.estado, "\n")] = '\0';
            printf("Nova população: ");
            scanf("%d", &cidade.populacao);
        }
        fprintf(temp, "%d,%s,%s,%d\n", cidade.codigo, cidade.nome, cidade.estado, cidade.populacao);
    }

    fclose(arquivo);
    fclose(temp);

    remove(ARQUIVO);
    rename(TEMP, ARQUIVO);

    if (encontrado) {
        printf("Dados da cidade alterados com sucesso!\n");
    } else {
        printf("Cidade não encontrada!\n");
    }
}

void excluirCidade(int codigo) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    FILE *temp = fopen(TEMP, "w");
    if (arquivo == NULL || temp == NULL) {
        perror("Erro ao abrir os arquivos");
        return;
    }

    Cidade cidade;
    int encontrado = 0;
    char linha[200];

    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%49[^,],%29[^,],%d", &cidade.codigo, cidade.nome, cidade.estado, &cidade.populacao);
        if (cidade.codigo == codigo) {
            encontrado = 1;
        } else {
            fprintf(temp, "%d,%s,%s,%d\n", cidade.codigo, cidade.nome, cidade.estado, cidade.populacao);
        }
    }

    fclose(arquivo);
    fclose(temp);

    remove(ARQUIVO);
    rename(TEMP, ARQUIVO);

    if (encontrado) {
        printf("Cidade excluída com sucesso!\n");
    } else {
        printf("Cidade não encontrada!\n");
    }
}

void consultarCidade(int codigo) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Cidade cidade;
    int encontrado = 0;
    char linha[200];

    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%49[^,],%29[^,],%d", &cidade.codigo, cidade.nome, cidade.estado, &cidade.populacao);
        if (cidade.codigo == codigo) {
            encontrado = 1;
            printf("\nCódigo: %d\n", cidade.codigo);
            printf("Nome: %s\n", cidade.nome);
            printf("Estado: %s\n", cidade.estado);
            printf("População: %d\n", cidade.populacao);
            break;
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("Cidade não encontrada!\n");
    }
}

void listarCidades() {
    FILE *arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    Cidade cidade;
    char linha[200];

    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%49[^,],%29[^,],%d", &cidade.codigo, cidade.nome, cidade.estado, &cidade.populacao);
        printf("\nCódigo: %d\n", cidade.codigo);
        printf("Nome: %s\n", cidade.nome);
        printf("Estado: %s\n", cidade.estado);
        printf("População: %d\n", cidade.populacao);
    }

    fclose(arquivo);
}
