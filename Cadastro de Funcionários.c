#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "funcionarios.dat"

// Definição da struct do funcionário
typedef struct {
    int id;
    char nome[50];
    char cargo[30];
    int salario;
} Funcionario;

// Declaração das funções
void cadastrarFuncionario(Funcionario funcionario);
void alterarFuncionario(int id);
void excluirFuncionario(int id);
Funcionario consultarFuncionario(int id);
void listarFuncionarios();

// Função principal
int main() {
    int opcao, id;
    Funcionario funcionario;

    do {
        printf("\n--- Sistema de Cadastro de Funcionários ---\n");
        printf("1. Cadastrar novo funcionário\n");
        printf("2. Alterar dados de um funcionário\n");
        printf("3. Excluir funcionário\n");
        printf("4. Consultar informações de um funcionário\n");
        printf("5. Listar todos os funcionários\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n--- Cadastrar Novo Funcionário ---\n");
                printf("ID: ");
                scanf("%d", &funcionario.id);
                printf("Nome: ");
                getchar(); // Limpar buffer do teclado
                fgets(funcionario.nome, 50, stdin);
                funcionario.nome[strcspn(funcionario.nome, "\n")] = '\0'; // Remover '\n'
                printf("Cargo: ");
                fgets(funcionario.cargo, 30, stdin);
                funcionario.cargo[strcspn(funcionario.cargo, "\n")] = '\0';
                printf("Salário (em reais): ");
                scanf("%d", &funcionario.salario);
                cadastrarFuncionario(funcionario);
                break;

            case 2:
                printf("\n--- Alterar Dados do Funcionário ---\n");
                printf("Informe o ID: ");
                scanf("%d", &id);
                alterarFuncionario(id);
                break;

            case 3:
                printf("\n--- Excluir Funcionário ---\n");
                printf("Informe o ID: ");
                scanf("%d", &id);
                excluirFuncionario(id);
                break;

            case 4:
                printf("\n--- Consultar Funcionário ---\n");
                printf("Informe o ID: ");
                scanf("%d", &id);
                funcionario = consultarFuncionario(id);
                if (funcionario.id != -1) {
                    printf("\nID: %d\n", funcionario.id);
                    printf("Nome: %s\n", funcionario.nome);
                    printf("Cargo: %s\n", funcionario.cargo);
                    printf("Salário: R$ %d\n", funcionario.salario);
                } else {
                    printf("Funcionário não encontrado!\n");
                }
                break;

            case 5:
                printf("\n--- Lista de Todos os Funcionários ---\n");
                listarFuncionarios();
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
void cadastrarFuncionario(Funcionario funcionario) {
    FILE *arquivo = fopen(ARQUIVO, "ab");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fwrite(&funcionario, sizeof(Funcionario), 1, arquivo);
    fclose(arquivo);
    printf("Funcionário cadastrado com sucesso!\n");
}

void alterarFuncionario(int id) {
    FILE *arquivo = fopen(ARQUIVO, "rb+");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Funcionario funcionario;
    int encontrado = 0;

    while (fread(&funcionario, sizeof(Funcionario), 1, arquivo)) {
        if (funcionario.id == id) {
            encontrado = 1;
            printf("Novo Cargo: ");
            getchar(); // Limpar buffer
            fgets(funcionario.cargo, 30, stdin);
            funcionario.cargo[strcspn(funcionario.cargo, "\n")] = '\0';
            printf("Novo Salário (em reais): ");
            scanf("%d", &funcionario.salario);

            fseek(arquivo, -sizeof(Funcionario), SEEK_CUR);
            fwrite(&funcionario, sizeof(Funcionario), 1, arquivo);
            break;
        }
    }
    fclose(arquivo);
    if (encontrado) {
        printf("Dados alterados com sucesso!\n");
    } else {
        printf("Funcionário não encontrado!\n");
    }
}

void excluirFuncionario(int id) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (arquivo == NULL || temp == NULL) {
        perror("Erro ao abrir os arquivos");
        return;
    }
    Funcionario funcionario;
    int encontrado = 0;

    while (fread(&funcionario, sizeof(Funcionario), 1, arquivo)) {
        if (funcionario.id == id) {
            encontrado = 1;
        } else {
            fwrite(&funcionario, sizeof(Funcionario), 1, temp);
        }
    }
    fclose(arquivo);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Funcionário excluído com sucesso!\n");
    } else {
        printf("Funcionário não encontrado!\n");
    }
}

Funcionario consultarFuncionario(int id) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    Funcionario funcionario;
    funcionario.id = -1; // Indicador de não encontrado

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return funcionario;
    }

    while (fread(&funcionario, sizeof(Funcionario), 1, arquivo)) {
        if (funcionario.id == id) {
            fclose(arquivo);
            return funcionario;
        }
    }
    fclose(arquivo);
    funcionario.id = -1;
    return funcionario;
}

void listarFuncionarios() {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Funcionario funcionario;

    while (fread(&funcionario, sizeof(Funcionario), 1, arquivo)) {
        printf("\nID: %d\n", funcionario.id);
        printf("Nome: %s\n", funcionario.nome);
        printf("Cargo: %s\n", funcionario.cargo);
        printf("Salário: R$ %d\n", funcionario.salario);
    }
    fclose(arquivo);
}
