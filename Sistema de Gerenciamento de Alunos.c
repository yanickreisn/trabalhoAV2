#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "alunos.dat"

// Definição da struct do aluno
typedef struct {
    int matricula;
    char nome[50];
    char curso[30];
    int idade;
} Aluno;

// Funções auxiliares
void salvarAluno(Aluno aluno);
void listarAlunos();
Aluno buscarAluno(int matricula);
void alterarAluno(int matricula);
void excluirAluno(int matricula);

// Menu principal
int main() {
    int opcao, matricula;
    Aluno aluno;

    do {
        printf("\n--- Sistema de Gerenciamento de Alunos ---\n");
        printf("1. Incluir novo aluno\n");
        printf("2. Alterar informações de um aluno\n");
        printf("3. Excluir aluno\n");
        printf("4. Consultar informações de um aluno\n");
        printf("5. Listar todos os alunos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n--- Incluir Novo Aluno ---\n");
                printf("Matrícula: ");
                scanf("%d", &aluno.matricula);
                printf("Nome: ");
                getchar(); // Limpar buffer do teclado
                fgets(aluno.nome, 50, stdin);
                aluno.nome[strcspn(aluno.nome, "\n")] = '\0'; // Remover '\n'
                printf("Curso: ");
                fgets(aluno.curso, 30, stdin);
                aluno.curso[strcspn(aluno.curso, "\n")] = '\0';
                printf("Idade: ");
                scanf("%d", &aluno.idade);
                salvarAluno(aluno);
                break;

            case 2:
                printf("\n--- Alterar Informações do Aluno ---\n");
                printf("Informe a matrícula: ");
                scanf("%d", &matricula);
                alterarAluno(matricula);
                break;

            case 3:
                printf("\n--- Excluir Aluno ---\n");
                printf("Informe a matrícula: ");
                scanf("%d", &matricula);
                excluirAluno(matricula);
                break;

            case 4:
                printf("\n--- Consultar Aluno ---\n");
                printf("Informe a matrícula: ");
                scanf("%d", &matricula);
                aluno = buscarAluno(matricula);
                if (aluno.matricula != -1) {
                    printf("\nMatrícula: %d\n", aluno.matricula);
                    printf("Nome: %s\n", aluno.nome);
                    printf("Curso: %s\n", aluno.curso);
                    printf("Idade: %d\n", aluno.idade);
                } else {
                    printf("Aluno não encontrado!\n");
                }
                break;

            case 5:
                printf("\n--- Lista de Todos os Alunos ---\n");
                listarAlunos();
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
void salvarAluno(Aluno aluno) {
    FILE *arquivo = fopen(ARQUIVO, "ab");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fwrite(&aluno, sizeof(Aluno), 1, arquivo);
    fclose(arquivo);
    printf("Aluno salvo com sucesso!\n");
}

void listarAlunos() {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Aluno aluno;
    while (fread(&aluno, sizeof(Aluno), 1, arquivo)) {
        printf("\nMatrícula: %d\n", aluno.matricula);
        printf("Nome: %s\n", aluno.nome);
        printf("Curso: %s\n", aluno.curso);
        printf("Idade: %d\n", aluno.idade);
    }
    fclose(arquivo);
}

Aluno buscarAluno(int matricula) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    Aluno aluno;
    aluno.matricula = -1; // Indicador de não encontrado
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return aluno;
    }
    while (fread(&aluno, sizeof(Aluno), 1, arquivo)) {
        if (aluno.matricula == matricula) {
            fclose(arquivo);
            return aluno;
        }
    }
    fclose(arquivo);
    aluno.matricula = -1;
    return aluno;
}

void alterarAluno(int matricula) {
    FILE *arquivo = fopen(ARQUIVO, "rb+");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Aluno aluno;
    int encontrado = 0;
    while (fread(&aluno, sizeof(Aluno), 1, arquivo)) {
        if (aluno.matricula == matricula) {
            encontrado = 1;
            printf("Novo Nome: ");
            getchar(); // Limpar buffer
            fgets(aluno.nome, 50, stdin);
            aluno.nome[strcspn(aluno.nome, "\n")] = '\0';
            printf("Novo Curso: ");
            fgets(aluno.curso, 30, stdin);
            aluno.curso[strcspn(aluno.curso, "\n")] = '\0';
            printf("Nova Idade: ");
            scanf("%d", &aluno.idade);

            fseek(arquivo, -sizeof(Aluno), SEEK_CUR);
            fwrite(&aluno, sizeof(Aluno), 1, arquivo);
            break;
        }
    }
    fclose(arquivo);
    if (encontrado) {
        printf("Aluno alterado com sucesso!\n");
    } else {
        printf("Aluno não encontrado!\n");
    }
}

void excluirAluno(int matricula) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (arquivo == NULL || temp == NULL) {
        perror("Erro ao abrir os arquivos");
        return;
    }
    Aluno aluno;
    int encontrado = 0;
    while (fread(&aluno, sizeof(Aluno), 1, arquivo)) {
        if (aluno.matricula == matricula) {
            encontrado = 1;
        } else {
            fwrite(&aluno, sizeof(Aluno), 1, temp);
        }
    }
    fclose(arquivo);
    fclose(temp);
    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Aluno excluído com sucesso!\n");
    } else {
        printf("Aluno não encontrado!\n");
    }
}
