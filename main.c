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

    //Gerando valores e atribuindo eles a cada Distrito
    for (int i = 0; i < numDistritos; i++) {
        int habitantesNesteDistrito = randomInRange(minHabitantesPorDistrito, maxHabitantesPorDistrito);
        double areaNesteDistrito = randomInRange(minAreaPorDistrito, maxAreaPorDistrito);

        distritos[i].ID = i + 1;
        distritos[i].habitantes = habitantesNesteDistrito; // Atribuindo o número de habitantes para cada distrito
        distritos[i].desempregados = habitantesNesteDistrito * (INDICE_DESEMPREGO / 100);
        distritos[i].aposentados = habitantesNesteDistrito * (INDICE_APOSENTADORIA / 100);
        distritos[i].populacaoInfantil = habitantesNesteDistrito * (POPULACAO_INFANTIL / 100);
        distritos[i].area = areaNesteDistrito; // Atribuindo o número da área para cada distrito
        distritos[i].densidadeHabitacional = habitantesNesteDistrito / areaNesteDistrito;
        distritos[i].carros = habitantesNesteDistrito / 10;
        distritos[i].motos = habitantesNesteDistrito / 20;
        printf("Distrito %d: %.2f área(KM²) | %d habitantes | Densidade Habitacional (Hab/KM²): %.3f  \n", distritos[i].ID,distritos[i].area, distritos[i].habitantes, distritos[i].densidadeHabitacional);
        printf("Carros estimados no distrito: %d | Motos estimadas no distrito: %d\n", distritos[i].carros, distritos[i].motos);
        printf("Desempregados: %d | Aposentados: %d | Menores de 17 anos: %d\n\n", distritos[i].desempregados, distritos[i].aposentados, distritos[i].populacaoInfantil);
    }

    return 0;
}