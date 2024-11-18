#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "veiculos.dat"

// Definição da struct do veículo
typedef struct {
    int id;
    char modelo[50];
    char placa[10];
    int ano;
} Veiculo;

// Declaração das funções
void adicionarVeiculo(Veiculo veiculo);
void alterarVeiculo(int id);
void excluirVeiculo(int id);
Veiculo consultarVeiculoPorPlaca(char placa[]);
void listarVeiculos();

// Função principal
int main() {
    int opcao, id;
    char placa[10];
    Veiculo veiculo;

    do {
        printf("\n--- Sistema de Registro de Veículos ---\n");
        printf("1. Adicionar novo veículo\n");
        printf("2. Alterar dados de um veículo\n");
        printf("3. Excluir veículo\n");
        printf("4. Consultar veículo pela placa\n");
        printf("5. Listar todos os veículos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n--- Adicionar Novo Veículo ---\n");
                printf("ID: ");
                scanf("%d", &veiculo.id);
                printf("Modelo: ");
                getchar(); // Limpar buffer do teclado
                fgets(veiculo.modelo, 50, stdin);
                veiculo.modelo[strcspn(veiculo.modelo, "\n")] = '\0'; // Remover '\n'
                printf("Placa: ");
                fgets(veiculo.placa, 10, stdin);
                veiculo.placa[strcspn(veiculo.placa, "\n")] = '\0';
                printf("Ano de fabricação: ");
                scanf("%d", &veiculo.ano);
                adicionarVeiculo(veiculo);
                break;

            case 2:
                printf("\n--- Alterar Dados do Veículo ---\n");
                printf("Informe o ID: ");
                scanf("%d", &id);
                alterarVeiculo(id);
                break;

            case 3:
                printf("\n--- Excluir Veículo ---\n");
                printf("Informe o ID: ");
                scanf("%d", &id);
                excluirVeiculo(id);
                break;

            case 4:
                printf("\n--- Consultar Veículo pela Placa ---\n");
                printf("Informe a placa: ");
                getchar(); // Limpar buffer do teclado
                fgets(placa, 10, stdin);
                placa[strcspn(placa, "\n")] = '\0';
                veiculo = consultarVeiculoPorPlaca(placa);
                if (veiculo.id != -1) {
                    printf("\nID: %d\n", veiculo.id);
                    printf("Modelo: %s\n", veiculo.modelo);
                    printf("Placa: %s\n", veiculo.placa);
                    printf("Ano: %d\n", veiculo.ano);
                } else {
                    printf("Veículo não encontrado!\n");
                }
                break;

            case 5:
                printf("\n--- Lista de Todos os Veículos ---\n");
                listarVeiculos();
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
void adicionarVeiculo(Veiculo veiculo) {
    FILE *arquivo = fopen(ARQUIVO, "ab");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    fwrite(&veiculo, sizeof(Veiculo), 1, arquivo);
    fclose(arquivo);
    printf("Veículo adicionado com sucesso!\n");
}

void alterarVeiculo(int id) {
    FILE *arquivo = fopen(ARQUIVO, "rb+");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Veiculo veiculo;
    int encontrado = 0;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        if (veiculo.id == id) {
            encontrado = 1;
            printf("Novo Modelo: ");
            getchar(); // Limpar buffer
            fgets(veiculo.modelo, 50, stdin);
            veiculo.modelo[strcspn(veiculo.modelo, "\n")] = '\0';
            printf("Nova Placa: ");
            fgets(veiculo.placa, 10, stdin);
            veiculo.placa[strcspn(veiculo.placa, "\n")] = '\0';
            printf("Novo Ano de Fabricação: ");
            scanf("%d", &veiculo.ano);

            fseek(arquivo, -sizeof(Veiculo), SEEK_CUR);
            fwrite(&veiculo, sizeof(Veiculo), 1, arquivo);
            break;
        }
    }
    fclose(arquivo);
    if (encontrado) {
        printf("Dados do veículo alterados com sucesso!\n");
    } else {
        printf("Veículo não encontrado!\n");
    }
}

void excluirVeiculo(int id) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (arquivo == NULL || temp == NULL) {
        perror("Erro ao abrir os arquivos");
        return;
    }
    Veiculo veiculo;
    int encontrado = 0;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        if (veiculo.id == id) {
            encontrado = 1;
        } else {
            fwrite(&veiculo, sizeof(Veiculo), 1, temp);
        }
    }
    fclose(arquivo);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Veículo excluído com sucesso!\n");
    } else {
        printf("Veículo não encontrado!\n");
    }
}

Veiculo consultarVeiculoPorPlaca(char placa[]) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    Veiculo veiculo;
    veiculo.id = -1; // Indicador de não encontrado

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return veiculo;
    }

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        if (strcmp(veiculo.placa, placa) == 0) {
            fclose(arquivo);
            return veiculo;
        }
    }
    fclose(arquivo);
    veiculo.id = -1;
    return veiculo;
}

void listarVeiculos() {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    Veiculo veiculo;

    while (fread(&veiculo, sizeof(Veiculo), 1, arquivo)) {
        printf("\nID: %d\n", veiculo.id);
        printf("Modelo: %s\n", veiculo.modelo);
        printf("Placa: %s\n", veiculo.placa);
        printf("Ano: %d\n", veiculo.ano);
    }
    fclose(arquivo);
}
