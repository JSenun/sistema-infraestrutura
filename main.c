#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Criando Macros para usar no decorer do código

// Macros para calculos de meio de transporte
// IMPORTANTE - Dados extraidos da internet como referência
#define INDICE_DESEMPREGO 8.0
#define POPULACAO_INFANTIL 17.0 
#define INDICE_APOSENTADORIA 15.0
#define MEDIA_MORADORES_RESIDENCIA 3.5


typedef struct Casa {
    int ID;             // Nome da casa
    int numMoradores;   // Número de moradores na casa
    double gastoEletrico; // Gasto elétrico da casa em kWh
    double gastoAgua;    // Gasto de água da casa em litros
};

typedef struct Rua {
    int ID;             // Nome da rua
    double comprimento; // Comprimento da rua em quilômetros
    int anoObra;        // Ano de inauguração da Obra
    int numCasas;       // Número de casas na rua
    struct Casa casas[20]; // Vetor de casas na rua (até 20 casas)
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

// Função para criar uma casa
void criarCasa(struct Casa *casa, int ID, int numMoradores, double gastoEletrico, double gastoAgua) {
    casa->ID = ID;
    casa->numMoradores = numMoradores;
    casa->gastoEletrico = gastoEletrico;
    casa->gastoAgua = gastoAgua;
}

// Função para criar uma rua
void criarRua(struct Rua *rua, int ID, double comprimento, int anoObra, int numCasas) {
    rua->ID = ID;
    rua->comprimento = comprimento;
    rua->anoObra = anoObra;
    rua->numCasas = numCasas;

    for (int i = 0; i < numCasas; i++) {
        int casaID = i + 1;
        int numMoradores = randomInRange(1, 5);  // Número de moradores (1 a 5)
        double gastoEletrico = (rand() % 300) / 10.0;  // Gasto elétrico em kWh (0 a 30)
        double gastoAgua = (rand() % 200) / 10.0;  // Gasto de água em litros (0 a 20)

        criarCasa(&rua->casas[i], casaID, numMoradores, gastoEletrico, gastoAgua);
    }
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

    // Calcula o número total de casas com base no número de habitantes
    int numCasas = habitantes / MEDIA_MORADORES_RESIDENCIA;

    // Inicializa o comprimento total das ruas
    double comprimentoTotalRuas = 0.0;

    // Cria ruas para o distrito e distribui casas entre elas
    for (int i = 0; i < numRuas; i++) {
        int ruaID = i + 1;

        // Calcula o máximo comprimento permitido para esta rua
        double comprimentoMaximo = (area / numRuas) * 2; // Exemplo: divide a área igualmente entre as ruas e multiplica por 2

        // Calcula o comprimento da rua, garantindo que não exceda o máximo
        double comprimento = randomInRange(1, comprimentoMaximo);

        // Atualiza o comprimento total das ruas
        comprimentoTotalRuas += comprimento;

        int anoObra = randomInRange(1950, 2020); // Ano de inauguração da obra
        
        // Calcula o número de casas para esta rua
        int numCasasNaRua = numCasas / numRuas;

        // Atribui as casas a esta rua
        criarRua(&distrito->ruas[i], ruaID, comprimento, anoObra, numCasasNaRua);

        // Reduz o número de casas restantes
        numCasas -= numCasasNaRua;

        // Verifica se o comprimento total das ruas ultrapassou a área máxima
        if (comprimentoTotalRuas > area) {
            break;
        }
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

// Função para listar as informações das casas em um distrito
void listarCasasNoDistrito(const Distrito *distrito) {
    printf("\nInformações das Casas no Distrito %d:\n", distrito->ID);
    for (int i = 0; i < 50; i++) {
        if (distrito->ruas[i].ID == 0 || distrito->ruas[i].ID <= 0) {
            break; // Sai do loop se não houver mais ruas
        }
        printf("Rua %d:\n", distrito->ruas[i].ID);
        for (int j = 0; j < distrito->ruas[i].numCasas; j++) {
            printf("Casa %d: Número de Moradores: %d | Gasto Elétrico: %.2f kWh | Gasto de Água: %.2f litros\n",
                   distrito->ruas[i].casas[j].ID, distrito->ruas[i].casas[j].numMoradores,
                   distrito->ruas[i].casas[j].gastoEletrico, distrito->ruas[i].casas[j].gastoAgua);
        }
    }
}

Distrito* iniciaSimulacao(){
    int areaCidade;
    int totalHabitantes;

    printf("Digite a área estimada da cidade em KM²: ");
    scanf("%d", &areaCidade);

    printf("Digite o total estimado de habitantes da cidade: ");
    scanf("%d", &totalHabitantes);

    // Calcula o mínimo e o máximo de habitantes por distrito
    int minHabitantesPorDistrito = 0.0075 * totalHabitantes;  // 0.75%
    int maxHabitantesPorDistrito = 0.03 * totalHabitantes;   // 3%
    
    // Calcula o mínimo e o máximo de área por distrito
    double minAreaPorDistrito = 0.0055 * areaCidade;  // 0.55%
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
    Distrito* distritos = malloc(sizeof(Distrito) * numDistritos);

    for (int i = 0; i < numDistritos; i++) {
        int habitantesNesteDistrito = randomInRange(minHabitantesPorDistrito, maxHabitantesPorDistrito);
        double areaNesteDistrito = randomInRange(minAreaPorDistrito, maxAreaPorDistrito);
        int numRuas = randomInRange(10,40);

        criarDistrito(&distritos[i], i + 1, areaNesteDistrito, habitantesNesteDistrito, numRuas); 
    }

    return distritos;
}

int main() {

    Distrito* distritos = iniciaSimulacao();

    listarCasasNoDistrito(&distritos[3]);
    listarCasasNoDistrito(&distritos[25]);
    listarCasasNoDistrito(&distritos[32]);

    return 0;
}