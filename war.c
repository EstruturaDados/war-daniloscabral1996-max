// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define MAX_NOME 50
#define MAX_COR 20
#define NUM_MISSOES 2

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char* nome;   
    char* cor;    
    int tropas;  
} Territorio;


// --- Protótipos das Funções ---

Territorio* alocarMapa(int num_territorios);
void inicializarTerritorios(Territorio* mapa, int num_territorios);
void liberarMemoria(Territorio* mapa, int num_territorios);
void exibirMenuPrincipal(void);
void exibirMapa(const Territorio* mapa, int num_territorios);
void exibirMissao(int id_missao, int alvo_missao);
void faseDeAtaque(Territorio* mapa, int num_territorios);
void simularAtaque(Territorio* mapa, int atacante, int defensor, int num_territorios);
int sortearMissao(int* alvo_missao);
int verificarVitoria(const Territorio* mapa, int num_territorios, int id_missao, int alvo_missao, const char* cor_jogador);
void limparBufferEntrada(void);


// --- Função Principal (main) ---


// --- Função Principal (main) ---
int main() {
    // Configura o idioma para português
    setlocale(LC_ALL, "Portuguese");

    // Inicializa a semente para números aleatórios
    srand(time(NULL));

    // Aloca o mapa
    Territorio* mapa = alocarMapa(NUM_TERRITORIOS);
    if (mapa == NULL) {
        printf("Erro: Não foi possível alocar memória para o mapa!\n");
        return 1;
    }

    // Inicializa os territórios com dados padrão
    inicializarTerritorios(mapa, NUM_TERRITORIOS);

    // Define a cor do jogador (vamos assumir que é Verde)
    const char* cor_jogador = "Verde";

    // Sorteia a missão do jogador
    int alvo_missao;
    int id_missao = sortearMissao(&alvo_missao);

    // Laço principal do jogo
    int opcao;
    do {
        // Exibe o mapa e a missão
        exibirMapa(mapa, NUM_TERRITORIOS);
        exibirMissao(id_missao, alvo_missao);

        // Exibe o menu e lê a escolha do jogador
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1: // Atacar
                faseDeAtaque(mapa, NUM_TERRITORIOS);
                break;
            case 2: // Verificar Missão
                if (verificarVitoria(mapa, NUM_TERRITORIOS, id_missao, alvo_missao, cor_jogador)) {
                    printf("Parabéns! Você cumpriu a missão e venceu o jogo!\n");
                    opcao = 0; // Encerra o jogo
                } else {
                    printf("Você ainda não cumpriu a missão.\n");
                }
                break;
            case 0: // Sair
                printf("Obrigado por jogar! Até a próxima!\n");
                break;
            default:
                printf("Opção inválida! Escolha 0, 1 ou 2.\n");
        }

        // Pausa para o jogador ler o resultado
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            limparBufferEntrada();
        }
    } while (opcao != 0);

    // Libera a memória
    liberarMemoria(mapa, NUM_TERRITORIOS);
    return 0;
}

// --- Implementação das Funções ---

// Aloca dinamicamente o vetor de territórios
Territorio* alocarMapa(int num_territorios) {
    Territorio* mapa = (Territorio*)calloc(num_territorios, sizeof(Territorio));
    if (mapa == NULL) {
        return NULL; // Retorna NULL se a alocação falhar
    }
    for (int i = 0; i < num_territorios; i++) {
        mapa[i].nome = (char*)calloc(MAX_NOME, sizeof(char));
        mapa[i].cor = (char*)calloc(MAX_COR, sizeof(char));
        if (mapa[i].nome == NULL || mapa[i].cor == NULL) {
            printf("Erro: Falha ao alocar memória para strings!\n");
            liberarMemoria(mapa, num_territorios);
            return NULL;
        }
    }
    return mapa;
}

// Inicializa os territórios com dados padrão
void inicializarTerritorios(Territorio* mapa, int num_territorios) {
    char* nomes[] = {"Winterfell", "Arryn", "Highgarden", "Lançassolar", "Rochedo Casterly"};
    char* cores[] = {"Cinza", "Azul", "Verde", "Amarelo", "Vermelho"};
    int tropas[] = {10, 3, 6, 7, 8};

    for (int i = 0; i < num_territorios; i++) {
        strncpy(mapa[i].nome, nomes[i], MAX_NOME);
        strncpy(mapa[i].cor, cores[i], MAX_COR);
        mapa[i].tropas = tropas[i];
    }
}

// Libera a memória alocada para o mapa
void liberarMemoria(Territorio* mapa, int num_territorios) {
    for (int i = 0; i < num_territorios; i++) {
        free(mapa[i].nome);
        free(mapa[i].cor);
    }
    free(mapa);
}

// Exibe o menu principal
void exibirMenuPrincipal(void) {
    printf("\n=== Menu Principal ===\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

// Exibe o estado atual do mapa
void exibirMapa(const Territorio* mapa, int num_territorios) {
    printf("\n=== Mapa Atual ===\n");
    for (int i = 0; i < num_territorios; i++) {
        printf("Território %d: %s (%s) - %d tropas\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Exibe a missão atual do jogador
void exibirMissao(int id_missao, int alvo_missao) {
    printf("\n=== Missão ===\n");
    if (id_missao == 0) {
        char* cores[] = {"Cinza", "Azul", "Verde", "Amarelo", "Vermelho"};
        printf("Destruir o exército %s\n", cores[alvo_missao]);
    } else {
        printf("Conquistar %d territórios\n", alvo_missao);
    }
}

// Gerencia a fase de ataque
void faseDeAtaque(Territorio* mapa, int num_territorios) {
    int atacante, defensor;
    printf("Escolha o território atacante (1 a %d): ", num_territorios);
    scanf("%d", &atacante);
    printf("Escolha o território defensor (1 a %d): ", num_territorios);
    scanf("%d", &defensor);
    limparBufferEntrada();
    simularAtaque(mapa, atacante, defensor, num_territorios);
}

// Simula uma batalha entre dois territórios
void simularAtaque(Territorio* mapa, int atacante, int defensor, int num_territorios) {
    // Validações
    if (atacante < 1 || atacante > num_territorios || defensor < 1 || defensor > num_territorios) {
        printf("Erro: Territórios inválidos!\n");
        return;
    }
    if (atacante == defensor) {
        printf("Erro: Não é possível atacar o mesmo território!\n");
        return;
    }
    if (mapa[atacante - 1].tropas <= 1) {
        printf("Erro: O atacante não tem tropas suficientes!\n");
        return;
    }
    if (mapa[defensor - 1].tropas == 0) {
        printf("Erro: O defensor já foi conquistado!\n");
        return;
    }

    // Rola os dados
    int dado_ataque = rand() % 6 + 1;
    int dado_defesa = rand() % 6 + 1;

    printf("Batalha: %s (Ataque: %d) vs %s (Defesa: %d)\n", 
           mapa[atacante - 1].nome, dado_ataque, mapa[defensor - 1].nome, dado_defesa);

    // Lógica da batalha
    if (dado_ataque >= dado_defesa) {
        printf("Atacante vence! %s perde 1 tropa.\n", mapa[defensor - 1].nome);
        mapa[defensor - 1].tropas--;
        if (mapa[defensor - 1].tropas == 0) {
            printf("%s foi conquistado por %s!\n", mapa[defensor - 1].nome, mapa[atacante - 1].nome);
            strncpy(mapa[defensor - 1].cor, mapa[atacante - 1].cor, MAX_COR);
            mapa[defensor - 1].tropas = 1; // Ocupa com 1 tropa
            mapa[atacante - 1].tropas--;   // Atacante perde 1 tropa
        }
    } else {
        printf("Defensor vence! %s perde 1 tropa.\n", mapa[atacante - 1].nome);
        mapa[atacante - 1].tropas--;
    }
}

// Sorteia uma missão para o jogador
int sortearMissao(int* alvo_missao) {
    int id_missao = rand() % NUM_MISSOES;
    if (id_missao == 0) {
        *alvo_missao = rand() % 5; // Escolhe uma cor (0 a 4)
    } else {
        *alvo_missao = 3; // Conquistar 3 territórios
    }
    return id_missao;
}

// Verifica se a missão foi cumprida
int verificarVitoria(const Territorio* mapa, int num_territorios, int id_missao, int alvo_missao, const char* cor_jogador) {
    if (id_missao == 0) { // Destruir um exército
        char* cores[] = {"Cinza", "Azul", "Verde", "Amarelo", "Vermelho"};
        for (int i = 0; i < num_territorios; i++) {
            if (strcmp(mapa[i].cor, cores[alvo_missao]) == 0 && mapa[i].tropas > 0) {
                return 0; // Exército alvo ainda existe
            }
        }
        return 1; // Missão cumprida
    } else { // Conquistar territórios
        int conquistados = 0;
        for (int i = 0; i < num_territorios; i++) {
            if (strcmp(mapa[i].cor, cor_jogador) == 0) {
                conquistados++;
            }
        }
        return conquistados >= alvo_missao; // Verdadeiro se conquistou 3 ou mais
    }
}

// Limpa o buffer de entrada
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
