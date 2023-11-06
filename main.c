#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Criando Macros para usar no decorer do código

// Macros para calculos de meio de transporte
// IMPORTANTE - Dados extraidos da internet como referência
#define INDICE_DESEMPREGO 8.0
#define POPULACAO_INFANTIL 17.0 
#define INDICE_APOSENTADORIA 15.0


typedef struct Rua{
    int ID; // Nome da rua
    double comprimento; // Comprimento da rua em quilômetros
    int anoObra; // Ano de inauguração da Obra
    int numCasas; // Número de casas na rua
};

// Definição da struct Avenida, que herda características da struct Rua
typedef struct Avenida{
    struct Rua rua; // Uma avenida é uma rua com características adicionais
    int numero_de_lojas; // Número de lojas na avenida
};

typedef struct {
    int ID; 
    double area; // Área em KM quadrados do distrito
    int habitantes; // Núm. de habitantes
    struct Rua ruas[50]; // Vetor de ruas de cada distrito
    struct Avenida avenidas[5]; // Vetor de Avenidas de cada distrito
    int desempregados; 
    int populacaoInfantil;// Núm. de habitantes menores de 17 anos
    int aposentados; 
    double densidadeHabitacional; // Núm. Habitantes / Área
    int carros; // Estimativa de quantos carros podem existir no distrito simulado - 1 a cada 10 pessoas
    int motos; // Estimativa de quantas motos podem existir no distrito simulado - 1 a cada 20 pessoas
} Distrito;

// Função para gerar um número aleatório entre min e max
int randomInRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Função para criar uma rua
void criarRua(struct Rua *rua, int ID, double comprimento, int anoObra, int numCasas) {
    rua->ID = ID;
    rua->comprimento = comprimento;
    rua->anoObra = anoObra;
    rua->numCasas = numCasas;
}

// Função para criar e inicializar um distrito
void criarDistrito(Distrito *distrito, int ID, double area, int habitantes, int numRuas) {
    distrito->ID = ID;
    distrito->area = area;
    distrito->habitantes = habitantes;
    distrito->desempregados = habitantes * (INDICE_DESEMPREGO / 100);
    distrito->aposentados = habitantes * (INDICE_APOSENTADORIA / 100);
    distrito->populacaoInfantil = habitantes * (POPULACAO_INFANTIL / 100);
    distrito->densidadeHabitacional = habitantes / area;
    distrito->carros = habitantes / 10;
    distrito->motos = habitantes / 20;

    // Cria ruas para o distrito
    for (int i = 0; i < numRuas; i++) {
        int ruaID = i + 1;
        double comprimento = (rand() % 10) + 1;  // Comprimento da rua entre 1 e 10 quilômetros
        int anoObra = randomInRange(1950, 2020); // Ano de inauguração da obra
        int numCasas = randomInRange(5, 20);    // Número de casas na rua

        criarRua(&distrito->ruas[i], ruaID, comprimento, anoObra, numCasas);
    }
}

// Função para listar as informações de um distrito
void listarInformacoesDistrito(const Distrito *distrito) {
    printf("\nInformações do Distrito %d:\n", distrito->ID);
    printf("Área: %.2f km²\n", distrito->area);
    printf("Habitantes: %d\n", distrito->habitantes);
    printf("Densidade Habitacional: %.3f Hab/KM²\n", distrito->densidadeHabitacional);
    printf("Carros estimados no distrito: %d\n", distrito->carros);
    printf("Motos estimadas no distrito: %d\n", distrito->motos);
    printf("Desempregados: %d\n", distrito->desempregados);
    printf("Aposentados: %d\n", distrito->aposentados);
    printf("Menores de 17 anos: %d\n", distrito->populacaoInfantil);
}

// Função para listar as ruas de um distrito
void listarRuasDistrito(const Distrito *distrito) {
    printf("\nRuas do Distrito %d:\n", distrito->ID);
    for (int i = 0; i < 50; i++) {
        if (distrito->ruas[i].ID == 0 || distrito->ruas[i].ID <= 0) {
            break; // Sai do loop se não houver mais ruas
        }
        printf("Rua %d: Comprimento: %.2f km | Ano de Obra: %d | Número de Casas: %d\n",
               distrito->ruas[i].ID, distrito->ruas[i].comprimento, distrito->ruas[i].anoObra, distrito->ruas[i].numCasas);
    }
}

int main() {
    int areaCidade;
    int totalHabitantes;

    printf("Digite a área estimada da cidade em KM²: ");
    scanf("%d", &areaCidade);

    printf("Digite o total estimado de habitantes da cidade: ");
    scanf("%d", &totalHabitantes);

    // Calcula o mínimo e o máximo de habitantes por distrito
    int minHabitantesPorDistrito = 0.002 * totalHabitantes;  // 0.20%
    int maxHabitantesPorDistrito = 0.03 * totalHabitantes;   // 3%
    
    // Calcula o mínimo e o máximo de área por distrito
    double minAreaPorDistrito = 0.0015 * areaCidade;  // 0.15%
    double maxAreaPorDistrito = 0.13 * areaCidade;    // 13%

    // Calcula o mínimo e o máximo de distritos possíveis baseados nas estimativas dos habitantes
    int minDistritos = totalHabitantes / maxHabitantesPorDistrito;
    int maxDistritos = totalHabitantes / minHabitantesPorDistrito;

    printf("Mínimo de habitantes por distrito: %d\n", minHabitantesPorDistrito);
    printf("Máximo de habitantes por distrito: %d\n", maxHabitantesPorDistrito);
    printf("Mínimo de distritos possíveis: %d\n", minDistritos);
    printf("Máximo de distritos possíveis: %d\n", maxDistritos);

    // Recebe o número de distritos do usuário e verifica se está dentro do limite
    int numDistritos;
    printf("Digite o número de distritos que deseja criar: ");
    scanf("%d", &numDistritos);

    if (numDistritos < minDistritos || numDistritos > maxDistritos) {
        printf("O número de distritos deve estar entre %d e %d\n", minDistritos, maxDistritos);
        return 1;
    }

    // Distribui aleatoriamente os habitantes entre os distritos
    srand(time(NULL));  // Inicializa a semente para números aleatórios

    // Cria um vetor de structs Distritos para a atribuição de dados;
    Distrito distritos[numDistritos];

    for (int i = 0; i < numDistritos; i++) {
        int habitantesNesteDistrito = randomInRange(minHabitantesPorDistrito, maxHabitantesPorDistrito);
        double areaNesteDistrito = randomInRange(minAreaPorDistrito, maxAreaPorDistrito);
        int numRuas = randomInRange(10,40);

        criarDistrito(&distritos[i], i + 1, areaNesteDistrito, habitantesNesteDistrito, numRuas);

        listarInformacoesDistrito(&distritos[i]);
        
        listarRuasDistrito(&distritos[i]);
    }

    return 0;
}