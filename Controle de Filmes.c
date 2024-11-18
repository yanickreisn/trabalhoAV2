#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "filmes.dat"

// Definição da struct do filme
typedef struct {
    int id;
    char titulo[50];
    char genero[20];
    int ano;
} Filme;

// Declaração das funções
void adicionarFilme(Filme filme);
void alterarFilme(int id);
void excluirFilme(int id);
Filme consultarFilme(int id);
void listarFilmes();

// Função principal
int main() {
    int opcao, id;
    Filme filme;

    do {
        printf("\n--- Sistema de Controle de Filmes ---\n");
        printf("1. Adicionar novo filme\n");
        printf("2. Alterar dados de um filme\n");
        printf("3. Excluir filme\n");
        printf("4. Consultar filme pelo ID\n");
        printf("5. Listar todos os filmes\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n--- Adicionar Novo Filme ---\n");
                printf("ID: ");
                scanf("%d", &filme.id);
                printf("Título: ");
                getchar(); // Limpar buffer do teclado
                fgets(filme.titulo, 50, stdin);
                filme.titulo[strcspn(filme.titulo, "\n")] = '\0'; // Remover '\n'
                printf("Gênero: ");
                fgets(filme.genero, 20, stdin);
                filme.genero[strcspn(filme.genero, "\n")] = '\0';
                printf("Ano de lançamento: ");
                scanf("%d", &filme.ano);
                adicionarFilme(filme);
                break;

            case 2:
                printf("\n--- Alterar Dados do Filme ---\n");
                printf("Informe o ID: ");
                scanf("%d", &id);
                alterarFilme(id);
                break;

            case 3:
                printf("\n--- Excluir Filme ---\n");
                printf("Informe o ID: ");
                scanf("%d", &id);
                excluirFilme(id);
                break;

            case 4:
                printf("\n--- Consultar Filme pelo ID ---\n");
                printf("Informe o ID: ");
                scanf("%d", &id);
                filme = consultarFilme(id);
                if (filme.id != -1) {
                    printf("\nID: %d\n", filme.id);
                    printf("Título: %s\n", filme.titulo);
                    printf("Gênero: %s\n", filme.genero);
                    printf("Ano: %d\n", filme.ano);
                } else {
                    printf("Filme não encontrado!\n");
                }
                break;

            case 5:
                printf("\n--- Lista de Todos os Filmes ---\n");
                listarFilmes();
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
void adicionarFilme(Filme filme) {
    FILE *arquivo = fopen(ARQUIVO, "ab");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fwrite(&filme, sizeof(Filme), 1, arquivo);
    fclose(arquivo);
    printf("Filme adicionado com sucesso!\n");
}

void alterarFilme(int id) {
    FILE *arquivo = fopen(ARQUIVO, "rb+");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Filme filme;
    int encontrado = 0;

    while (fread(&filme, sizeof(Filme), 1, arquivo)) {
        if (filme.id == id) {
            encontrado = 1;
            printf("Novo Título: ");
            getchar(); // Limpar buffer
            fgets(filme.titulo, 50, stdin);
            filme.titulo[strcspn(filme.titulo, "\n")] = '\0';
            printf("Novo Gênero: ");
            fgets(filme.genero, 20, stdin);
            filme.genero[strcspn(filme.genero, "\n")] = '\0';
            printf("Novo Ano de Lançamento: ");
            scanf("%d", &filme.ano);

            fseek(arquivo, -sizeof(Filme), SEEK_CUR);
            fwrite(&filme, sizeof(Filme), 1, arquivo);
            break;
        }
    }
    fclose(arquivo);
    if (encontrado) {
        printf("Dados do filme alterados com sucesso!\n");
    } else {
        printf("Filme não encontrado!\n");
    }
}

void excluirFilme(int id) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (arquivo == NULL || temp == NULL) {
        perror("Erro ao abrir os arquivos");
        return;
    }
    Filme filme;
    int encontrado = 0;

    while (fread(&filme, sizeof(Filme), 1, arquivo)) {
        if (filme.id == id) {
            encontrado = 1;
        } else {
            fwrite(&filme, sizeof(Filme), 1, temp);
        }
    }
    fclose(arquivo);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Filme excluído com sucesso!\n");
    } else {
        printf("Filme não encontrado!\n");
    }
}

Filme consultarFilme(int id) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    Filme filme;
    filme.id = -1; // Indicador de não encontrado

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return filme;
    }

    while (fread(&filme, sizeof(Filme), 1, arquivo)) {
        if (filme.id == id) {
            fclose(arquivo);
            return filme;
        }
    }
    fclose(arquivo);
    filme.id = -1;
    return filme;
}

void listarFilmes() {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Filme filme;

    while (fread(&filme, sizeof(Filme), 1, arquivo)) {
        printf("\nID: %d\n", filme.id);
        printf("Título: %s\n", filme.titulo);
        printf("Gênero: %s\n", filme.genero);
        printf("Ano: %d\n", filme.ano);
    }
    fclose(arquivo);
}
