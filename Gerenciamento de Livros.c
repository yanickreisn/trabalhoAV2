#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "livros.dat"

// Definição da struct do livro
typedef struct {
    int codigo;
    char titulo[50];
    char autor[50];
    int ano;
} Livro;

// Declaração das funções
void adicionarLivro(Livro livro);
void alterarLivro(int codigo);
void excluirLivro(int codigo);
Livro consultarLivro(int codigo);
void listarLivros();

// Função principal
int main() {
    int opcao, codigo;
    Livro livro;

    do {
        printf("\n--- Sistema de Gerenciamento de Livros ---\n");
        printf("1. Adicionar novo livro\n");
        printf("2. Alterar dados de um livro\n");
        printf("3. Excluir livro\n");
        printf("4. Consultar informações de um livro\n");
        printf("5. Listar todos os livros\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n--- Adicionar Novo Livro ---\n");
                printf("Código: ");
                scanf("%d", &livro.codigo);
                printf("Título: ");
                getchar(); // Limpar buffer do teclado
                fgets(livro.titulo, 50, stdin);
                livro.titulo[strcspn(livro.titulo, "\n")] = '\0'; // Remover '\n'
                printf("Autor: ");
                fgets(livro.autor, 50, stdin);
                livro.autor[strcspn(livro.autor, "\n")] = '\0';
                printf("Ano de publicação: ");
                scanf("%d", &livro.ano);
                adicionarLivro(livro);
                break;

            case 2:
                printf("\n--- Alterar Dados do Livro ---\n");
                printf("Informe o código: ");
                scanf("%d", &codigo);
                alterarLivro(codigo);
                break;

            case 3:
                printf("\n--- Excluir Livro ---\n");
                printf("Informe o código: ");
                scanf("%d", &codigo);
                excluirLivro(codigo);
                break;

            case 4:
                printf("\n--- Consultar Livro ---\n");
                printf("Informe o código: ");
                scanf("%d", &codigo);
                livro = consultarLivro(codigo);
                if (livro.codigo != -1) {
                    printf("\nCódigo: %d\n", livro.codigo);
                    printf("Título: %s\n", livro.titulo);
                    printf("Autor: %s\n", livro.autor);
                    printf("Ano de publicação: %d\n", livro.ano);
                } else {
                    printf("Livro não encontrado!\n");
                }
                break;

            case 5:
                printf("\n--- Lista de Todos os Livros ---\n");
                listarLivros();
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
void adicionarLivro(Livro livro) {
    FILE *arquivo = fopen(ARQUIVO, "ab");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fwrite(&livro, sizeof(Livro), 1, arquivo);
    fclose(arquivo);
    printf("Livro adicionado com sucesso!\n");
}

void alterarLivro(int codigo) {
    FILE *arquivo = fopen(ARQUIVO, "rb+");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Livro livro;
    int encontrado = 0;

    while (fread(&livro, sizeof(Livro), 1, arquivo)) {
        if (livro.codigo == codigo) {
            encontrado = 1;
            printf("Novo Título: ");
            getchar(); // Limpar buffer
            fgets(livro.titulo, 50, stdin);
            livro.titulo[strcspn(livro.titulo, "\n")] = '\0';
            printf("Novo Autor: ");
            fgets(livro.autor, 50, stdin);
            livro.autor[strcspn(livro.autor, "\n")] = '\0';
            printf("Novo Ano de Publicação: ");
            scanf("%d", &livro.ano);

            fseek(arquivo, -sizeof(Livro), SEEK_CUR);
            fwrite(&livro, sizeof(Livro), 1, arquivo);
            break;
        }
    }
    fclose(arquivo);
    if (encontrado) {
        printf("Dados do livro alterados com sucesso!\n");
    } else {
        printf("Livro não encontrado!\n");
    }
}

void excluirLivro(int codigo) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (arquivo == NULL || temp == NULL) {
        perror("Erro ao abrir os arquivos");
        return;
    }
    Livro livro;
    int encontrado = 0;

    while (fread(&livro, sizeof(Livro), 1, arquivo)) {
        if (livro.codigo == codigo) {
            encontrado = 1;
        } else {
            fwrite(&livro, sizeof(Livro), 1, temp);
        }
    }
    fclose(arquivo);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Livro excluído com sucesso!\n");
    } else {
        printf("Livro não encontrado!\n");
    }
}

Livro consultarLivro(int codigo) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    Livro livro;
    livro.codigo = -1; // Indicador de não encontrado

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return livro;
    }

    while (fread(&livro, sizeof(Livro), 1, arquivo)) {
        if (livro.codigo == codigo) {
            fclose(arquivo);
            return livro;
        }
    }
    fclose(arquivo);
    livro.codigo = -1;
    return livro;
}

void listarLivros() {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Livro livro;

    while (fread(&livro, sizeof(Livro), 1, arquivo)) {
        printf("\nCódigo: %d\n", livro.codigo);
        printf("Título: %s\n", livro.titulo);
        printf("Autor: %s\n", livro.autor);
        printf("Ano de publicação: %d\n", livro.ano);
    }
    fclose(arquivo);
}
