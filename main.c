#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

// Criando Macros para usar no decorer do c�digo

// Macros para calculos de meio de transporte
// IMPORTANTE - Dados extraidos da internet como refer�ncia
#define INDICE_DESEMPREGO 8.0
#define POPULACAO_INFANTIL 17.0 
#define INDICE_APOSENTADORIA 15.0
#define MEDIA_MORADORES_RESIDENCIA 3.5
#define MEDIA_GASTO_ENERGIA 24.0 // kWh por habitante por dia para refer�ncia
#define MEDIA_GASTO_AGUA 110.0 // Litros por habitante por dia para refer�ncia


typedef struct Casa {
    int ID;             // Nome da casa
    int numMoradores;   // N�mero de moradores na casa
    double gastoEletrico; // Gasto el�trico da casa em kWh
    double gastoAgua;    // Gasto de �gua da casa em litros
}Casa;

typedef struct Rua {
    int ID;             // Nome da rua
    double comprimento; // Comprimento da rua em quil�metros
    int anoObra;        // Ano de inaugura��o da Obra
    int numCasas;       // N�mero de casas na rua
    struct Casa casas[200]; // Vetor de casas na rua (at� 200 casas)
}Rua;

typedef struct Onibus {
    int ID;           // Identifica��o do �nibus
    int capacidade;   // Capacidade de passageiros do �nibus
} Onibus;

typedef struct LinhaOnibus {
    int ID;             // Identifica��o da linha de �nibus
    struct Rua *inicio; // Rua de in�cio da linha
    struct Rua *fim;    // Rua de t�rmino da linha
    struct Onibus onibus; // Cria o "modelo" de �nibus que opera na linha
    int quantOnibus; // Quantos �nibus operam na linha
} LinhaOnibus;

typedef struct EstacaoTratamentoAgua {
    int ID;             // Identifica��o da esta��o de tratamento de �gua
    double capacidade;  // Capacidade de tratamento de �gua em litros por segundo
} EstacaoTratamentoAgua;

typedef struct UsinaEnergia {
    int ID;             // Identifica��o da usina de energia
    double capacidade;  // Capacidade de gera��o de energia em kilowatts
} UsinaEnergia;

typedef struct {
    int ID; 
    double area; // �rea em KM quadrados do distrito
    int habitantes; // N�m. de habitantes
    struct Rua ruas[50]; // Vetor de ruas de cada distrito
    int desempregados; 
    int populacaoInfantil;// N�m. de habitantes menores de 17 anos
    int aposentados; 
    double gastoTotalAgua;   // Gasto total de �gua em litros no distrito
    double gastoTotalEnergia;// Gasto total de energia em kWh no distrito
    struct EstacaoTratamentoAgua estacaoAgua;
    struct UsinaEnergia usinaEnergia;
    double densidadeHabitacional; // N�m. Habitantes / �rea
    int carros; // Estimativa de quantos carros podem existir no distrito simulado - 1 a cada 10 pessoas
    int motos; // Estimativa de quantas motos podem existir no distrito simulado - 1 a cada 20 pessoas
    struct LinhaOnibus linhasOnibus[15]; // Vetor de linhas de �nibus
} Distrito;

// Fun��o para gerar um n�mero aleat�rio entre min e max (inclusive)
int randomInRange(int min, int max) {
    // Certifique-se de que max � pelo menos min + 1 para evitar problemas com % 0
    if (max < min) {
        int temp = max;
        max = min;
        min = temp;
    }

    // Gera um n�mero aleat�rio no intervalo [min, max]
    return min + rand() % (max - min + 1);
}
// Fun��o para criar uma casa
void criarCasa(struct Casa *casa, int ID, int numMoradores, double gastoEletrico, double gastoAgua) {
    casa->ID = ID;
    casa->numMoradores = numMoradores;
    casa->gastoEletrico = gastoEletrico;
    casa->gastoAgua = gastoAgua;
}

// Fun��o para criar uma rua
void criarRua(struct Rua *rua, int ID, double comprimento, int anoObra, int numCasas) {
    rua->ID = ID;
    rua->comprimento = comprimento;
    rua->anoObra = anoObra;
    rua->numCasas = numCasas;

    // Limita o n�mero de casas ao tamanho do vetor
    if (numCasas > 200) {
        printf("Aviso: O n�mero de casas na rua %d foi limitado a 200.\n", ID);
        numCasas = 200;
    }

    for (int i = 0; i < numCasas; i++) {
        int casaID = i + 1;
        int numMoradores = randomInRange(1, 5);  // N�mero de moradores (1 a 5)
        double gastoEletrico = numMoradores * MEDIA_GASTO_ENERGIA; 
        double gastoAgua = numMoradores * MEDIA_GASTO_AGUA;

        criarCasa(&rua->casas[i], casaID, numMoradores, gastoEletrico, gastoAgua);
    }
}

// Fun��o para criar um �nibus
void criarOnibus(Onibus *onibus, int ID, int capacidade) {
    onibus->ID = ID;
    onibus->capacidade = capacidade;
}

void criarLinhaOnibus(LinhaOnibus *linha, int ID, Rua *ruas, int numRuas) {
    linha->ID = ID;
    linha->inicio = &ruas[randomInRange(0, numRuas - 1)];  // Atribui uma rua aleat�ria como in�cio
    linha->fim = &ruas[randomInRange(0, numRuas - 1)];     // Atribui outra rua aleat�ria como fim
    linha->quantOnibus = randomInRange(3, 15);

    // Verifica se a rua de in�cio e a rua de t�rmino n�o s�o as mesmas
    while (linha->inicio == linha->fim) {
        linha->fim = &ruas[randomInRange(0, numRuas - 1)];
    }

    // Chama a fun��o criarOnibus para inicializar o �nibus na linha
    criarOnibus(&linha->onibus, ID, randomInRange(20, 50));  // Capacidade do �nibus entre 20 e 50 passageiros
}

// Fun��o para criar e atribuir linhas de �nibus a ruas em um distrito
void criarLinhasDeOnibus(Distrito *distrito) {
    for (int i = 0; i < 15; i++) {  // Criar at� 15 linhas de �nibus
        int linhaID = i + 1;
        criarLinhaOnibus(&distrito->linhasOnibus[i], linhaID, distrito->ruas, 50);  // Assumindo que h� no m�ximo 50 ruas no distrito
    }
}
// Fun��o para criar Esta��o de �gua para cada distrito
void criarEstacaoTratamentoAgua(EstacaoTratamentoAgua *estacao, int ID, double capacidade) {
    estacao->ID = ID;
    estacao->capacidade = capacidade;
}

// FUn��o para criar Usina de �gua para cada distrito
void criarUsinaEnergia(UsinaEnergia *usina, int ID, double capacidade) {
    usina->ID = ID;
    usina->capacidade = capacidade;
}

// Fun��o para criar e inicializar um distrito
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

    // Calcula o n�mero total de casas com base no n�mero de habitantes
    int numCasas = habitantes / MEDIA_MORADORES_RESIDENCIA;

    // Inicializa o comprimento total das ruas
    double comprimentoTotalRuas = 0.0;

// Cria ruas para o distrito e distribui casas entre elas
    for (int i = 0; i < numRuas; i++) {
        int ruaID = i + 1;

        // Calcula o m�ximo comprimento permitido para esta rua
        double comprimentoMaximo = (area / numRuas) * 2; // Exemplo: divide a �rea igualmente entre as ruas e multiplica por 2

        // Calcula o comprimento da rua, garantindo que n�o exceda o m�ximo
        double comprimento = randomInRange(2, comprimentoMaximo);

        // Atualiza o comprimento total das ruas
        comprimentoTotalRuas += comprimento;

        int anoObra = randomInRange(1950, 2020); // Ano de inaugura��o da obra

        // Calcula o n�mero de casas para esta rua
        int numCasasNaRua = numCasas / numRuas;

        // Limita o n�mero de casas ao tamanho do vetor para evitar bugs
        if (numCasasNaRua > 200) {
            numCasasNaRua = 200;
        }

        // Atribui as casas a esta rua
        criarRua(&distrito->ruas[i], ruaID, comprimento, anoObra, numCasasNaRua);

        // Reduz o n�mero de casas restantes
        numCasas -= numCasasNaRua;

        // Verifica se o comprimento total das ruas ultrapassou a �rea m�xima
        if (comprimentoTotalRuas > area) {
            break;
        }
    }
    
    // Cria e atribui linhas de �nibus ao distrito
    for (int i = 0; i < 15; i++) {  // Criar at� 15 linhas de �nibus
        int linhaID = i + 1;
        criarLinhaOnibus(&distrito->linhasOnibus[i], linhaID, distrito->ruas, numRuas);
    }
    
    for (int i = 0; i < numRuas; i++) {
        for (int j = 0; j < distrito->ruas[i].numCasas; j++) {
            distrito->gastoTotalAgua += distrito->ruas[i].casas[j].gastoAgua;
            distrito->gastoTotalEnergia += distrito->ruas[i].casas[j].gastoEletrico;
            }
    }

    // Inicializa��o da esta��o de tratamento de �gua e da usina de energia
    double capacidadeEstacaoAgua = distrito->gastoTotalAgua * 1.1; // Exemplo: 10% a mais que o gasto total
    double capacidadeUsinaEnergia = distrito->gastoTotalEnergia * 1.2; // Exemplo: 20% a mais que o gasto total

    criarEstacaoTratamentoAgua(&distrito->estacaoAgua, 1,capacidadeEstacaoAgua);
    criarUsinaEnergia(&distrito->usinaEnergia, 1, capacidadeUsinaEnergia);

}

// Fun��o para listar as informa��es de um distrito
void listarInformacoesDistrito(const Distrito *distrito) {
    printf("\nInforma��es do Distrito %d:\n", distrito->ID);
    printf("�rea: %.2f km�\n", distrito->area);
    printf("Habitantes: %d\n", distrito->habitantes);
    printf("Densidade Habitacional: %.3f Hab/KM�\n", distrito->densidadeHabitacional);
    printf("Carros estimados no distrito: %d\n", distrito->carros);
    printf("Motos estimadas no distrito: %d\n", distrito->motos);
    printf("Desempregados: %d\n", distrito->desempregados);
    printf("Aposentados: %d\n", distrito->aposentados);
    printf("Menores de 17 anos: %d\n", distrito->populacaoInfantil);

    int totalRuas = 0;
    int totalCasas = 0;
    int totalLinhasOnibus = 0;
    int totalOnibus = 0;

    // Calcular totais
    for (int i = 0; i < 50; i++) {
        if (distrito->ruas[i].ID == 0 || distrito->ruas[i].ID <= 0) {
            break; // Sai do loop se n�o houver mais ruas
        }
        totalRuas++;
        totalCasas += distrito->ruas[i].numCasas;
    }

    for (int i = 0; i < 15; i++) {
        if (distrito->linhasOnibus[i].ID == 0 || distrito->linhasOnibus[i].ID <= 0) {
            break;  // Sai do loop se n�o houver mais linhas de �nibus
        }
        totalLinhasOnibus++;
        totalOnibus += distrito->linhasOnibus[i].quantOnibus;
    }

    printf("Total de Ruas: %d\n", totalRuas);
    printf("Total de Casas: %d\n", totalCasas);

    printf("Gasto Total de �gua no Distrito: %.2f litros\n", distrito->gastoTotalAgua);
    printf("Gasto Total de Energia no Distrito: %.2f kWh\n", distrito->gastoTotalEnergia);
    
    printf("Total de Linhas de �nibus: %d\n", totalLinhasOnibus);
    printf("Total de �nibus: %d\n\n", totalOnibus);
}

// Fun��o para listar as ruas de um distrito
void listarRuasDistrito(const Distrito *distrito) {
    printf("\nRuas do Distrito %d:\n", distrito->ID);
    for (int i = 0; i < 50; i++) {
        if (distrito->ruas[i].ID == 0 || distrito->ruas[i].ID <= 0) {
            break; // Sai do loop se n�o houver mais ruas
        }
        printf("Rua %d: Comprimento: %.2f km | Ano de Obra: %d | N�mero de Casas: %d\n",
               distrito->ruas[i].ID, distrito->ruas[i].comprimento, distrito->ruas[i].anoObra, distrito->ruas[i].numCasas);
    }
}

// Fun��o para listar as informa��es das casas em um distrito
void listarCasasNoDistrito(const Distrito *distrito) {
    printf("\nInforma��es das Casas no Distrito %d:\n", distrito->ID);
    for (int i = 0; i < 50; i++) {
        if (distrito->ruas[i].ID == 0 || distrito->ruas[i].ID <= 0) {
            break; // Sai do loop se n�o houver mais ruas
        }
        printf("Rua %d:\n", distrito->ruas[i].ID);
        for (int j = 0; j < distrito->ruas[i].numCasas; j++) {
            printf("Casa %d: N�mero de Moradores: %d | Gasto El�trico: %.2f kWh | Gasto de �gua: %.2f litros\n",
                   distrito->ruas[i].casas[j].ID, distrito->ruas[i].casas[j].numMoradores,
                   distrito->ruas[i].casas[j].gastoEletrico, distrito->ruas[i].casas[j].gastoAgua);
        }
    }
}

// Fun��o para listar as informa��es das linhas de �nibus em um distrito
void listarLinhasOnibusDistrito(const Distrito *distrito) {
    printf("\nLinhas de �nibus no Distrito %d:\n", distrito->ID);
    for (int i = 0; i < 15; i++) {  // Supondo que h� no m�ximo 15 linhas de �nibus
        if (distrito->linhasOnibus[i].ID == 0 || distrito->linhasOnibus[i].ID <= 0) {
            break;  // Sai do loop se n�o houver mais linhas de �nibus
        }
        printf("Linha %d: In�cio: Rua %d, Fim: Rua %d, Capacidade dos �nibus: %d, �nibus operando na Linha: %d\n",
               distrito->linhasOnibus[i].ID,
               distrito->linhasOnibus[i].inicio->ID,
               distrito->linhasOnibus[i].fim->ID,
               distrito->linhasOnibus[i].onibus.capacidade,
               distrito->linhasOnibus[i].quantOnibus);
    }
}

Distrito* iniciaSimulacao() {
    int areaCidade = randomInRange(900, 2000); // �rea da cidade entre 500 e 2000 KM�
    int totalHabitantes = randomInRange(75000, 750000); // N�mero total de habitantes entre 50.000 e 500.000

    // Calcula o m�nimo e o m�ximo de habitantes por distrito
    int minHabitantesPorDistrito = 0.0075 * totalHabitantes;  // 0.75%
    int maxHabitantesPorDistrito = 0.03 * totalHabitantes;   // 3%

    // Calcula o m�nimo e o m�ximo de �rea por distrito
    double minAreaPorDistrito = 0.01 * areaCidade;  // 1%
    double maxAreaPorDistrito = 0.13 * areaCidade;    // 13%

    int numDistritos = 96; // N�mero de distritos a serem criados na simula��o

    printf("�rea estimada da cidade: %d KM�\n", areaCidade);
    printf("N�mero total estimado de habitantes na cidade: %d\n", totalHabitantes);
    printf("M�nimo de habitantes por distrito: %d\n", minHabitantesPorDistrito);
    printf("M�ximo de habitantes por distrito: %d\n", maxHabitantesPorDistrito);

    // Distribui aleatoriamente os habitantes entre os distritos
    srand(time(NULL));  // Inicializa a semente para n�meros aleat�rios

    // Cria um vetor de structs Distritos para a atribui��o de dados;
    Distrito* distritos = malloc(sizeof(Distrito) * numDistritos);

    for (int i = 0; i < numDistritos; i++) {
        int habitantesNesteDistrito = randomInRange(minHabitantesPorDistrito, maxHabitantesPorDistrito);
        double areaNesteDistrito = randomInRange(minAreaPorDistrito, maxAreaPorDistrito);
        int numRuas = randomInRange(10, 40);

        criarDistrito(&distritos[i], i + 1, areaNesteDistrito, habitantesNesteDistrito, numRuas);
        //printf("Distrito %d - Criado com sucesso.\n", i);

    }
    
    printf("N�mero de distritos gerados automaticamente: %d\n", numDistritos);
    

    return distritos;
}


int main() {

    // Corrigindo problemas com caracteres no CMD
    setlocale(LC_ALL, "portuguese");

    Distrito* distritos = iniciaSimulacao();
    
    listarInformacoesDistrito(&distritos[4]);
    listarCasasNoDistrito(&distritos[4]);

    // Liberando a mem�ria alocada para o vetor de distritos quando n�o for mais necess�rio.
    free(distritos);

    return 0;
}