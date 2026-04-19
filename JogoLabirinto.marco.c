//Programa desenvolvido pelo aluno: Marco Antônio de Almeida Santos.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

/* ============================================================
   CONSTANTES Valores fixos que representam cada tipo de celula na matriz.
   ============================================================ */
#define N 10
#define PAREDE       1
#define CAMINHO      0
#define SAIDA       -1
#define ARMADILHA   -2

/* Pontuacao que o jogador adquire jogando*/
#define PONTOS_MOVIMENTO  10
#define PENALIDADE_PAREDE 15
#define PENALIDADE_ARMADILHA 30
#define BONUS_VITORIA 200

/* Cores (Windows Console) */
#define COR_RESET     7   /* Cinza claro (padrao) */
#define COR_JOGADOR   11  /* Ciano brilhante */
#define COR_PAREDE    8   /* Cinza escuro */
#define COR_SAIDA     10  /* Verde brilhante */
#define COR_ARMADILHA 12  /* Vermelho brilhante */
#define COR_CAMINHO   7   /* Cinza claro */
#define COR_TITULO    14  /* Amarelo */
#define COR_INFO      9   /* Azul brilhante */
#define COR_ALERTA    12  /* Vermelho */
#define COR_SUCESSO   10  /* Verde */

/* ============================================================
   VARIAVEIS GLOBAIS
   ============================================================ */
int labirinto[N][N] = {
    { 0,  1,  0,  0,  0,  0,  1,  0,  0,  0},
    { 0,  1,  0,  1,  1,  0,  1,  0,  1,  0},
    { 0,  0,  0,  1,  0,  0,  0,  0,  1,  0},
    { 1,  1,  0,  1,  0,  1,  1, -2,  1,  0},
    { 0,  0,  0,  0,  0,  1,  0,  0,  0,  0},
    { 0,  1,  1,  1,  0,  1,  0,  1,  1,  1},
    { 0,  1,  0,  0,  0,  0,  0,  1,  0,  0},
    { 0,  1,  0,  1,  1,  1,  0,  1,  0,  1},
    { 0,  0,  0,  1,  0, -2,  0,  0,  0,  1},
    { 1,  1,  0,  0,  0,  1,  1,  1,  0, -1}
};

int posX = 0, posY = 0;
int movimentos = 0;
int pontuacao = 0;
int jogoAtivo = 1;

/* ============================================================
   FUNCOES DE COR
   ============================================================ */
void setCor(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

/* ============================================================
   FUNCOES DE SOM
   ============================================================ */
void somMovimento(void) {
    Beep(600, 60);
}

void somParede(void) {
    Beep(200, 300);
    Beep(150, 200);
}

void somArmadilha(void) {
    Beep(800, 80);
    Beep(400, 80);
    Beep(200, 200);
}

void somVitoria(void) {
    Beep(523, 150);
    Beep(659, 150);
    Beep(784, 150);
    Beep(1047, 400);
}

void somInicio(void) {
    Beep(440, 100);
    Beep(550, 100);
    Beep(660, 200);
}

/* ============================================================
   FUNCAO: Limpar tela
   ============================================================ */
void limparTela(void) {
    system("cls");
}

/* ============================================================
   FUNCAO: Exibir cabecalho
   ============================================================ */
void exibirCabecalho(void) {
    setCor(COR_TITULO);
    printf("==============================================\n");
    printf("         *** LABIRINTO DO MARCO ***        \n");
    printf("==============================================\n");
    setCor(COR_INFO);
    printf(" Controles: W=Cima  S=Baixo  A=Esq  D=Dir\n");
    printf("==============================================\n\n");
    setCor(COR_RESET);
}

/* ============================================================
   FUNCAO: Exibir legenda
   ============================================================ */
void exibirLegenda(void) {
    printf("\n");
    setCor(COR_TITULO);
    printf(" LEGENDA:\n");
    setCor(COR_RESET);
    printf("  ");
    setCor(COR_JOGADOR);  printf("@");
    setCor(COR_RESET);    printf(" Voce     ");
    setCor(COR_PAREDE);   printf("#");
    setCor(COR_RESET);    printf(" Parede   ");
    setCor(COR_SAIDA);    printf("S");
    setCor(COR_RESET);    printf(" Saida\n");
    printf("  ");
    setCor(COR_ARMADILHA);printf("X");
    setCor(COR_RESET);    printf(" Armadilha   ");
    setCor(COR_CAMINHO);  printf(".");
    setCor(COR_RESET);    printf(" Caminho\n\n");
}

/* ============================================================
   FUNCAO: Exibir informacoes do jogador
   ============================================================ */
void exibirInfo(void) {
    setCor(COR_INFO);
    printf(" Posicao: [Linha: %d | Coluna: %d]", posX + 1, posY + 1);
    setCor(COR_RESET);
    printf("   Movimentos: ");
    setCor(COR_TITULO);
    printf("%d", movimentos);
    setCor(COR_RESET);
    printf("   Pontuacao: ");
    setCor(COR_SUCESSO);
    printf("%d\n\n", pontuacao);
    setCor(COR_RESET);
}

/* ============================================================
   FUNCAO: Mostrar labirinto
   ============================================================ */
void mostrarLabirinto(void) {
    int i, j;

    /* Cabecalho das colunas */
    setCor(COR_INFO);
    printf("     ");
    for (j = 0; j < N; j++) {
        printf("%2d", j + 1);
    }
    printf("\n");
    printf("    +");
    for (j = 0; j < N; j++) {
        printf("--");
    }
    printf("+\n");
    setCor(COR_RESET);

    for (i = 0; i < N; i++) {
        /* Numero da linha */
        setCor(COR_INFO);
        printf(" %2d |", i + 1);
        setCor(COR_RESET);

        for (j = 0; j < N; j++) {
            if (i == posX && j == posY) {
                setCor(COR_JOGADOR);
                printf("@ ");
            } else if (labirinto[i][j] == PAREDE) {
                setCor(COR_PAREDE);
                printf("# ");
            } else if (labirinto[i][j] == SAIDA) {
                setCor(COR_SAIDA);
                printf("S ");
            } else if (labirinto[i][j] == ARMADILHA) {
                setCor(COR_ARMADILHA);
                printf("X ");
            } else {
                setCor(COR_CAMINHO);
                printf(". ");
            }
        }
        setCor(COR_INFO);
        printf("|");
        setCor(COR_RESET);
        printf("\n");
    }

    setCor(COR_INFO);
    printf("    +");
    for (j = 0; j < N; j++) {
        printf("--");
    }
    printf("+\n");
    setCor(COR_RESET);
}

/* ============================================================
   FUNCAO: Validar movimento
   Retorna: 1 se valido, 0 se parede/fora, -1 se armadilha, -2 se saida
   ============================================================ */
int validarMovimento(int novoX, int novoY) {
    /* Fora dos limites ou parede */
    if (novoX < 0 || novoX >= N || novoY < 0 || novoY >= N) return 0;
    if (labirinto[novoX][novoY] == PAREDE) return 0;
    if (labirinto[novoX][novoY] == ARMADILHA) return -1;
    if (labirinto[novoX][novoY] == SAIDA) return -2;
    return 1;
}

/* ============================================================
   FUNCAO: Tocar som
   tipo: 0=movimento, 1=parede, 2=armadilha, 3=vitoria
   ============================================================ */
void tocarSom(int tipo) {
    switch (tipo) {
        case 0: somMovimento();  break;
        case 1: somParede();     break;
        case 2: somArmadilha();  break;
        case 3: somVitoria();    break;
    }
}

/* ============================================================
   FUNCAO: Processar movimento
   ============================================================ */
void processarMovimento(char cmd) {
    int novoX = posX;
    int novoY = posY;
    int resultado;

    /* Calcula nova posicao */
    if      (cmd == 'W') novoX--;
    else if (cmd == 'S') novoX++;
    else if (cmd == 'A') novoY--;
    else if (cmd == 'D') novoY++;
    else {
        setCor(COR_ALERTA);
        printf("\n Comando invalido! Use W/A/S/D.\n");
        setCor(COR_RESET);
        Sleep(800);
        return;
    }

    resultado = validarMovimento(novoX, novoY);

    if (resultado == 0) {
        /* Parede ou fora */
        tocarSom(1);
        pontuacao -= PENALIDADE_PAREDE;
        if (pontuacao < 0) pontuacao = 0;
        setCor(COR_ALERTA);
        printf("\n BOOM! Voce bateu numa parede! -%d pontos.\n", PENALIDADE_PAREDE);
        setCor(COR_RESET);
        Sleep(700);
    } else if (resultado == -1) {
        /* Armadilha */
        posX = novoX;
        posY = novoY;
        movimentos++;
        pontuacao -= PENALIDADE_ARMADILHA;
        if (pontuacao < 0) pontuacao = 0;
        tocarSom(2);
        setCor(COR_ALERTA);
        printf("\n ARMADILHA! Voce perdeu %d pontos!\n", PENALIDADE_ARMADILHA);
        setCor(COR_RESET);
        Sleep(900);
        /* Remove armadilha apos pisar */
        labirinto[novoX][novoY] = CAMINHO;
    } else if (resultado == -2) {
        /* Saida */
        posX = novoX;
        posY = novoY;
        movimentos++;
        pontuacao += BONUS_VITORIA;
        jogoAtivo = 0;
    } else {
        /* Movimento valido */
        posX = novoX;
        posY = novoY;
        movimentos++;
        pontuacao += PONTOS_MOVIMENTO;
        tocarSom(0);
    }
}

/* ============================================================
   FUNCAO: Tela de vitoria
   ============================================================ */
void telaVitoria(void) {
    limparTela();
    tocarSom(3);
    setCor(COR_SUCESSO);
    printf("\n\n");
    printf("  ==========================================\n");
    printf("  ||                                      ||\n");
    printf("  ||   PARABENS! VOCE ESCAPOU!            ||\n");
    printf("  ||                                      ||\n");
    printf("  ==========================================\n\n");
    setCor(COR_TITULO);
    printf("  Movimentos realizados : %d\n", movimentos);
    printf("  Pontuacao final       : %d\n", pontuacao);
    setCor(COR_RESET);
    printf("\n  Pressione ENTER para sair...\n");
}

/* ============================================================
   FUNCAO: Menu inicial
   ============================================================ */
void menuInicial(void) {
    limparTela();
    setCor(COR_TITULO);
    printf("\n\n");
    printf("   ==========================================\n");
    printf("   ||    *** LABIRINTO DA PERDI��O ***     ||\n");
    printf("   ||           Versao 2.0                 ||\n");
    printf("   ==========================================\n\n");
    setCor(COR_INFO);
    printf("   -> Labirinto 10x10 com armadilhas!\n");
    printf("   -> Ganhe pontos a cada passo correto.\n");
    printf("   -> Cuidado com as armadilhas (X)!\n");
    printf("   -> Encontre a saida (S) para vencer.\n\n");
    setCor(COR_SUCESSO);
    printf("   Pressione ENTER para iniciar...\n");
    setCor(COR_RESET);
    getchar();
    somInicio();
}

/* ============================================================
   FUNCAO PRINCIPAL
   ============================================================ */
int main(void) {
    char comando;

    menuInicial();

    while (jogoAtivo) {
        limparTela();
        exibirCabecalho();
        exibirInfo();
        mostrarLabirinto();
        exibirLegenda();

        /* Verifica vitoria (caso entre pela validacao) */
        if (!jogoAtivo) break;

        setCor(COR_INFO);
        printf(" Movimento (W/A/S/D): ");
        setCor(COR_RESET);
        scanf(" %c", &comando);
        comando = toupper(comando);

        processarMovimento(comando);
    }

    telaVitoria();
    getchar();
    getchar();

    return 0;
}
