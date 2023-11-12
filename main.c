#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

// Criando Macros para usar no decorer do código

// Macros para calculos de meio de transporte
// IMPORTANTE - Dados extraidos da internet como referência
#define INDICE_DESEMPREGO 8.0
#define POPULACAO_INFANTIL 17.0 
#define INDICE_APOSENTADORIA 15.0
#define MEDIA_MORADORES_RESIDENCIA 3.5
#define MEDIA_GASTO_ENERGIA 24.0 // kWh por habitante por dia
#define MEDIA_GASTO_AGUA 110.0 // Litros por habitante por dia

/*
*
* Estruturas 
*
*/

typedef struct Casa {
    int ID;             // Nome da casa
    int numMoradores;   // Número de moradores na casa
    double gastoEletrico; // Gasto elétrico da casa em kWh
    double gastoAgua;    // Gasto de água da casa em litros
}Casa;

typedef struct Rua {
    int ID;             // Nome da rua
    double comprimento; // Comprimento da rua em quilômetros
    int anoObra;        // Ano de inauguração da Obra
    int numCasas;       // Número de casas na rua
    struct Casa casas[200]; // Vetor de casas na rua (até 200 casas)
}Rua;

typedef struct Onibus {
    int ID;           // Identificação do ônibus
    int capacidade;   // Capacidade de passageiros do ônibus
} Onibus;

typedef struct LinhaOnibus {
    int ID;             // Identificação da linha de ônibus
    struct Rua *inicio; // Rua de início da linha
    struct Rua *fim;    // Rua de término da linha
    struct Onibus onibus; // Cria o "modelo" de ônibus que opera na linha
    int quantOnibus; // Quantos ônibus operam na linha
} LinhaOnibus;

typedef struct EstacaoTratamentoAgua {
    int ID;             // Identificação da estação de tratamento de água
    double capacidade;  // Capacidade de tratamento de água em litros por segundo
} EstacaoTratamentoAgua;

typedef struct UsinaEnergia {
    int ID;             // Identificação da usina de energia
    double capacidade;  // Capacidade de geração de energia em kilowatts
} UsinaEnergia;

typedef struct {
    int ID; 
    double area; // Área em KM quadrados do distrito
    int habitantes; // Núm. de habitantes
    struct Rua ruas[50]; // Vetor de ruas de cada distrito
    int desempregados; 
    int populacaoInfantil;// Núm. de habitantes menores de 17 anos
    int aposentados; 
    double gastoTotalAgua;   // Gasto total de água em litros no distrito
    double gastoTotalEnergia;// Gasto total de energia em kWh no distrito
    struct EstacaoTratamentoAgua estacaoAgua;
    struct UsinaEnergia usinaEnergia;
    double densidadeHabitacional; // Núm. Habitantes / Área
    int carros; // Estimativa de quantos carros podem existir no distrito simulado - 1 a cada 10 pessoas
    int motos; // Estimativa de quantas motos podem existir no distrito simulado - 1 a cada 20 pessoas
    struct LinhaOnibus linhasOnibus[15]; // Vetor de linhas de ônibus
} Distrito;

/*
*
* Funções para a simulação
*
*/

// Função para gerar um número aleatório entre min e max (inclusive)
int randomInRange(int min, int max) {
    // Certifique-se de que max é pelo menos min + 1 para evitar problemas com % 0
    if (max < min) {
        int temp = max;
        max = min;
        min = temp;
    }

    // Gera um número aleatório no intervalo [min, max]
    return min + rand() % (max - min + 1);
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

    // Limita o número de casas ao tamanho do vetor
    if (numCasas > 200) {
        printf("Aviso: O número de casas na rua %d foi limitado a 200.\n", ID);
        numCasas = 200;
    }

    for (int i = 0; i < numCasas; i++) {
        int casaID = i + 1;
        int numMoradores = randomInRange(1, 5);  // Número de moradores (1 a 5)
        double gastoEletrico = numMoradores * MEDIA_GASTO_ENERGIA; 
        double gastoAgua = numMoradores * MEDIA_GASTO_AGUA;

        criarCasa(&rua->casas[i], casaID, numMoradores, gastoEletrico, gastoAgua);
    }
}

// Função para criar um ônibus
void criarOnibus(Onibus *onibus, int ID, int capacidade) {
    onibus->ID = ID;
    onibus->capacidade = capacidade;
}

void criarLinhaOnibus(LinhaOnibus *linha, int ID, Rua *ruas, int numRuas) {
    linha->ID = ID;
    linha->inicio = &ruas[randomInRange(0, numRuas - 1)];  // Atribui uma rua aleatória como início
    linha->fim = &ruas[randomInRange(0, numRuas - 1)];     // Atribui outra rua aleatória como fim
    linha->quantOnibus = randomInRange(3, 15);

    // Verifica se a rua de início e a rua de término não são as mesmas
    while (linha->inicio == linha->fim) {
        linha->fim = &ruas[randomInRange(0, numRuas - 1)];
    }

    // Chama a função criarOnibus para inicializar o ônibus na linha
    criarOnibus(&linha->onibus, ID, randomInRange(20, 50));  // Capacidade do ônibus entre 20 e 50 passageiros
}

// Função para criar e atribuir linhas de ônibus a ruas em um distrito
void criarLinhasDeOnibus(Distrito *distrito) {
    for (int i = 0; i < 15; i++) {  // Criar até 15 linhas de ônibus
        int linhaID = i + 1;
        criarLinhaOnibus(&distrito->linhasOnibus[i], linhaID, distrito->ruas, 50);  // Assumindo que há no máximo 50 ruas no distrito
    }
}
// Função para criar Estação de água para cada distrito
void criarEstacaoTratamentoAgua(EstacaoTratamentoAgua *estacao, int ID, double capacidade) {
    estacao->ID = ID;
    estacao->capacidade = capacidade;
}

// FUnção para criar Usina de água para cada distrito
void criarUsinaEnergia(UsinaEnergia *usina, int ID, double capacidade) {
    usina->ID = ID;
    usina->capacidade = capacidade;
}

// Função para criar e inicializar um distrito
void criarDistrito(Distrito *distrito, int ID, double area, int habitantes, int numRuas) {
    distrito->ID = ID;
    distrito->area = area;
    distrito->habitantes = habitantes;

    // Calcula o número total de casas com base no número incial de habitantes
    int numCasas = habitantes / MEDIA_MORADORES_RESIDENCIA;

    // Inicializa o comprimento total das ruas
    double comprimentoTotalRuas = 0.0;
    
    // Reseta o número de habitantes no distrito
    distrito->habitantes = 0;

    // Cria ruas para o distrito e distribui casas entre elas
    for (int i = 0; i < numRuas; i++) {
        int ruaID = i + 1;

        // Calcula o máximo comprimento permitido para esta rua
        double comprimentoMaximo = (area / numRuas) * 2; // Exemplo: divide a área igualmente entre as ruas e multiplica por 2

        // Calcula o comprimento da rua, garantindo que não exceda o máximo
        double comprimento = randomInRange(2, comprimentoMaximo);

        // Atualiza o comprimento total das ruas
        comprimentoTotalRuas += comprimento;

        int anoObra = randomInRange(1950, 2020); // Ano de inauguração da obra

        // Calcula o número de casas para esta rua
        int numCasasNaRua = numCasas / numRuas;

        // Limita o número de casas ao tamanho do vetor para evitar bugs
        if (numCasasNaRua > 200) {
            numCasasNaRua = 200;
        }

        // Atribui as casas a esta rua
        criarRua(&distrito->ruas[i], ruaID, comprimento, anoObra, numCasasNaRua);
        
        // Atualiza o número de habitantes no distrito com base nos números gerados nas casas
        for (int j = 0; j < numCasasNaRua; j++) {
            distrito->habitantes += distrito->ruas[i].casas[j].numMoradores;
        }

        // Reduz o número de casas restantes
        numCasas -= numCasasNaRua;

        // Verifica se o comprimento total das ruas ultrapassou a área máxima
        if (comprimentoTotalRuas > area) {
            break;
        }
    }
    
    
    // Calcula indicadores com base no número de habitantes
    distrito->desempregados = distrito->habitantes * (INDICE_DESEMPREGO / 100);
    distrito->aposentados = distrito->habitantes * (INDICE_APOSENTADORIA / 100);
    distrito->populacaoInfantil = distrito->habitantes * (POPULACAO_INFANTIL / 100);
    distrito->densidadeHabitacional = distrito->habitantes / area;
    distrito->carros = distrito->habitantes / 10;
    distrito->motos = distrito->habitantes / 20;

    
    // Cria e atribui linhas de ônibus ao distrito
    for (int i = 0; i < 15; i++) {  // Criar até 15 linhas de ônibus
        int linhaID = i + 1;
        criarLinhaOnibus(&distrito->linhasOnibus[i], linhaID, distrito->ruas, numRuas);
    }
    
    for (int i = 0; i < numRuas; i++) {
        for (int j = 0; j < distrito->ruas[i].numCasas; j++) {
            distrito->gastoTotalAgua += distrito->ruas[i].casas[j].gastoAgua;
            distrito->gastoTotalEnergia += distrito->ruas[i].casas[j].gastoEletrico;
            }
    }

    // Inicialização da estação de tratamento de água e da usina de energia
    double capacidadeEstacaoAgua = distrito->gastoTotalAgua * 1.1; // Exemplo: 10% a mais que o gasto total
    double capacidadeUsinaEnergia = distrito->gastoTotalEnergia * 1.2; // Exemplo: 20% a mais que o gasto total

    criarEstacaoTratamentoAgua(&distrito->estacaoAgua, 1,capacidadeEstacaoAgua);
    criarUsinaEnergia(&distrito->usinaEnergia, 1, capacidadeUsinaEnergia);

}

/*
*
* Funções para "análise de dados"
*
*/

// Função para listar as informações de um distrito
void listarInformacoesDistrito(const Distrito *distrito) {
    printf("\nInformações do Distrito %d:\n", distrito->ID);
    printf("Área: %.2f km²\n", distrito->area);
    printf("Habitantes: %d\n", distrito->habitantes);
    printf("Densidade Habitacional: %.3f Hab/KM²\n", distrito->densidadeHabitacional);
    printf("Carros estimados no distrito: %d\n", distrito->carros);
    printf("Motos estimadas no distrito: %d\n", distrito->motos);

    int totalRuas = 0;
    int totalCasas = 0;
    int totalLinhasOnibus = 0;
    int totalOnibus = 0;

    // Calcular totais
    for (int i = 0; i < 50; i++) {
        if (distrito->ruas[i].ID == 0 || distrito->ruas[i].ID <= 0) {
            break; // Sai do loop se não houver mais ruas
        }
        totalRuas++;
        totalCasas += distrito->ruas[i].numCasas;
    }

    for (int i = 0; i < 15; i++) {
        if (distrito->linhasOnibus[i].ID == 0 || distrito->linhasOnibus[i].ID <= 0) {
            break;  // Sai do loop se não houver mais linhas de ônibus
        }
        totalLinhasOnibus++;
        totalOnibus += distrito->linhasOnibus[i].quantOnibus;
    }

    printf("Total de Ruas: %d\n", totalRuas);
    printf("Total de Casas: %d\n", totalCasas);

    printf("Gasto Total de Água no Distrito: %.2f litros\n", distrito->gastoTotalAgua);
    printf("Gasto Total de Energia no Distrito: %.2f kWh\n", distrito->gastoTotalEnergia);
    
    printf("Total de Linhas de Ônibus: %d\n", totalLinhasOnibus);
    printf("Total de Ônibus: %d\n\n", totalOnibus);
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

// Função para listar as informações das linhas de ônibus em um distrito
void listarLinhasOnibusDistrito(const Distrito *distrito) {
    printf("\nLinhas de Ônibus no Distrito %d:\n", distrito->ID);
    for (int i = 0; i < 15; i++) {  // Supondo que há no máximo 15 linhas de ônibus
        if (distrito->linhasOnibus[i].ID == 0 || distrito->linhasOnibus[i].ID <= 0) {
            break;  // Sai do loop se não houver mais linhas de ônibus
        }
        printf("Linha %d: Início: Rua %d, Fim: Rua %d, Capacidade dos Ônibus: %d, Ônibus operando na Linha: %d\n",
               distrito->linhasOnibus[i].ID,
               distrito->linhasOnibus[i].inicio->ID,
               distrito->linhasOnibus[i].fim->ID,
               distrito->linhasOnibus[i].onibus.capacidade,
               distrito->linhasOnibus[i].quantOnibus);
    }
}

void analiseDemografica(const Distrito *distrito) {
    int menoresDe17 = distrito->populacaoInfantil;
    int entre17e64 = distrito->habitantes - (distrito->populacaoInfantil + distrito->aposentados);
    int com65ouMais = distrito->aposentados;

    printf("\nAnálise Demográfica do Distrito %d:\n", distrito->ID);
    printf("Menores de 17 anos: %d\n", menoresDe17);
    printf("Entre 17 e 64 anos: %d\n", entre17e64);
    printf("Com 65 anos ou mais: %d\n", com65ouMais);

    // Calcular percentuais
    double percentualMenoresDe17 = (double)menoresDe17 / distrito->habitantes * 100;
    double percentualEntre17e64 = (double)entre17e64 / distrito->habitantes * 100;
    double percentualCom65ouMais = (double)com65ouMais / distrito->habitantes * 100;

    printf("Percentual de Menores de 17 anos: %.2f%%\n", percentualMenoresDe17);
    printf("Percentual Entre 17 e 64 anos: %.2f%%\n", percentualEntre17e64);
    printf("Percentual com 65 anos ou mais: %.2f%%\n", percentualCom65ouMais);
}

void analiseMobilidadeUrbana(const Distrito *distrito) {
    printf("\nAnálise de Mobilidade Urbana do Distrito %d:\n", distrito->ID);

    for (int i = 0; i < 15; i++) {
        if (distrito->linhasOnibus[i].ID == 0 || distrito->linhasOnibus[i].ID <= 0) {
            break; // Sai do loop se não houver mais linhas de ônibus
        }

        int totalPassageiros = distrito->linhasOnibus[i].quantOnibus * distrito->linhasOnibus[i].onibus.capacidade;
        double mediaPassageirosPorOnibus = (double)totalPassageiros / distrito->linhasOnibus[i].quantOnibus;

        printf("Linha %d: Média de Passageiros por Ônibus: %.2f\n",
               distrito->linhasOnibus[i].ID, mediaPassageirosPorOnibus);
    }
}

void listarInformacoesTotais(const Distrito *distritos, int numDistritos) {
    double areaTotal = 0.0;
    int habitantesTotal = 0;
    double gastoTotalAgua = 0.0;
    double gastoTotalEnergia = 0.0;

    for (int i = 0; i < numDistritos; i++) {
        areaTotal += distritos[i].area;
        habitantesTotal += distritos[i].habitantes;
        gastoTotalAgua += distritos[i].gastoTotalAgua;
        gastoTotalEnergia += distritos[i].gastoTotalEnergia;
    }

    printf("\nInformações Totais da Simulação:\n");
    printf("Área Total: %.2f km²\n", areaTotal);
    printf("Total de Habitantes: %d\n", habitantesTotal);
    printf("Gasto Total de Água em Todos os Distritos: %.2f litros\n", gastoTotalAgua);
    printf("Gasto Total de Energia em Todos os Distritos: %.2f kWh\n", gastoTotalEnergia);
}

// Função de comparação para qsort
int compararDistritos(const void *a, const void *b) {
    const Distrito *distritoA = (const Distrito *)a;
    const Distrito *distritoB = (const Distrito *)b;

    // Ordene em ordem decrescente de densidade habitacional
    if (distritoA->densidadeHabitacional > distritoB->densidadeHabitacional) {
        return -1;
    } else if (distritoA->densidadeHabitacional < distritoB->densidadeHabitacional) {
        return 1;
    } else {
        return 0;
    }
}

// Função para listar os top-N distritos com maior densidade habitacional
void listarTopNDistritos(const Distrito *distritos, int numDistritos, int N) {
    // Cria um vetor de distritos para classificação
    Distrito *distritosClassificados = malloc(sizeof(Distrito) * numDistritos);
    
    // Copia os dados dos distritos para o vetor classificado
    for (int i = 0; i < numDistritos; i++) {
        distritosClassificados[i] = distritos[i];
    }

    // Ordena o vetor de distritos em ordem decrescente de densidade habitacional
    qsort(distritosClassificados, numDistritos, sizeof(Distrito), compararDistritos);

    // Imprime os top-N distritos com maior densidade habitacional
    printf("\nTop-%d Distritos com Maior Densidade Habitacional:\n", N);
    for (int i = 0; i < N && i < numDistritos; i++) {
        printf("%d° - Distrito %d: Densidade Habitacional: %.3f Hab/KM²\n",i+1, distritosClassificados[i].ID, distritosClassificados[i].densidadeHabitacional);
    }

    // Libera a memória alocada para o vetor classificado
    free(distritosClassificados);
}

/*
*
* Funções Principais
*
*/

Distrito* iniciaSimulacao() {
    int areaCidade = randomInRange(750, 2000); // Área da cidade base para a simulação entre 1000 e 2000 KM² (Valor alterado durante a geração da simulação)
    int totalHabitantes = randomInRange(100000, 7500000); // Número total de habitantes entre 50.000 e 500.000 (Valor alterado durante a geração da simulação) 

    // Calcula o mínimo e o máximo de habitantes por distrito
    int minHabitantesPorDistrito = 0.075 * totalHabitantes;  // 0,75
    int maxHabitantesPorDistrito = 0.03 * totalHabitantes;   // 3%

    // Calcula o mínimo e o máximo de área por distrito
    double minAreaPorDistrito = 0.01 * areaCidade;  // 1%
    double maxAreaPorDistrito = 0.13 * areaCidade;    // 13%

    int numDistritos = 96; // Número de distritos a serem criados na simulação

    // Distribui aleatoriamente os habitantes entre os distritos
    srand(time(NULL));  // Inicializa a semente para números aleatórios

    // Cria um vetor de structs Distritos para a atribuição de dados;
    Distrito* distritos = malloc(sizeof(Distrito) * numDistritos);

    for (int i = 0; i < numDistritos; i++) {
        int habitantesNesteDistrito = randomInRange(minHabitantesPorDistrito, maxHabitantesPorDistrito);
        double areaNesteDistrito = randomInRange(minAreaPorDistrito, maxAreaPorDistrito);
        int numRuas = randomInRange(10, 40);

        criarDistrito(&distritos[i], i + 1, areaNesteDistrito, habitantesNesteDistrito, numRuas);
        //printf("Distrito %d - Criado com sucesso.\n", i);

    }
    
    printf("Número de distritos gerados automaticamente: %d\n", numDistritos);
    

    return distritos;
}


int main() {

    // Corrigindo problemas com caracteres no CMD
    setlocale(LC_ALL, "portuguese");

    Distrito* distritos = iniciaSimulacao();
    
    listarInformacoesTotais(distritos, 96);
    listarInformacoesDistrito(&distritos[4]);
    analiseDemografica(&distritos[4]);
    listarTopNDistritos(distritos, 96, 25);


    // Liberando a memória alocada para o vetor de distritos quando não for mais necessário.
    free(distritos);

    return 0;
}