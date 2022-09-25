// Atualiza��o 1.2
// Melhorando a intera��o com o jogador (menu de upgrades e escolha de a��o)
// --- dia: 25/09

// Autor: H�garo Veloso
// Email: hvelosocontato@gmail.com

// Importar bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>

// Valores base do jogador
#define BASE_VIDA_MAXIMA_JOGADOR 5.0
#define BASE_DANO_JOGADOR 10
#define BASE_CHANCE_CRITICO_JOGADOR 10
#define BASE_CURA_JOGADOR 10
#define BASE_ESCUDO_JOGADOR 5
#define VALOR_ESCUDO_INICIAL_JOGADOR 5
#define VALOR_VIDA_MAXIMA_INICIAL_JOGADOR 100
#define VALOR_SORTE_INICIAL_jOGADOR 4
#define VALOR_MANA_INICIAL_JOGADOR 2
#define VALOR_POCAO_INICIAL_JOGADOR 2
#define MULTIPLICADOR_NORMAL_CRITICO 2.25
#define MULTIPLICADOR_MAGICO 3.0
#define MULTIPLICADOR_MAGICO_CRITICO 5.0

// Valores base dos inimigos
#define BASE_DANO_INIMIGO 10
#define BASE_CHANCE_CRITICO_INIMIGO 7
#define BASE_VIDA_INIMIGO 50
#define BASE_CURA_INIMIGO 5
#define BASE_CHANCE_DESVIO_INIMIGO 5
#define VALOR_MODIFICAR_DESVIO_INIMIGO 30

// Limite dos atributos do jogador
#define LIMITE_ESCUDO_JOGADOR 80
#define LIMITE_CHANCE_CRITICO_JOGADOR 99
#define LIMITE_SORTE_JOGADOR 20

// Limite dos atributos do inimigo
#define LIMITE_CHANCE_CRITICO_INIMIGO 75
#define LIMITE_CHANCE_DESVIO_INIMIGO 50

// Valores do jogo
#define TAMANHO_MAXIMO_NOME 20
#define RECOMPENSA_MANA 2
#define RECOMPENSA_POCAO 2
#define RECOMPENSA_PONTOS 3
#define RECOMPENSA_SCORE 50
#define LIMITE_UPGRADE 10

// Declara��o de vari�veis globais
// Vari�veis do jogador
char nomeDoJogador[TAMANHO_MAXIMO_NOME];
float vidaMaximaJogador = VALOR_VIDA_MAXIMA_INICIAL_JOGADOR;
int danoJogador;
int chanceCriticoJogador;
int curaJogador;
int escudoJogador = 0;
int pocaoJogador = VALOR_POCAO_INICIAL_JOGADOR;
int manaJogador = VALOR_MANA_INICIAL_JOGADOR;
int sorte = VALOR_SORTE_INICIAL_jOGADOR;

int valorEscudoJogador; // Quanto o jogador recebe de escudo por po��o
float vidaJogador;

// Vari�veis dos inimigos
float vidaMaximaInimigo;
int danoInimigo;
int chanceCriticoInimigo;
float vidaInimigo;
int curaInimigo;
int chanceDesvioInimigo;

// Vari�veis do jogo
int fase = 0;
int score = 0;
int pontos = 0;
float danoCausado;
int combo = 0;
int limiteDesvioAtingido = 0;
int op;

// Prot�tipo das fun��es do programa
// Menus do jogo
void TelaInicial(); // Da output na tela de inicio do jogo
void LogJogador(); // Da output nas estat�sticas do jogador
void LogInimigo(); // Da output nas estat�sticas do inimigo
void Titulo(); // Da output num cabe�alho com a fase e a pontua��o do jogador
void Log(); // Da output em todas as informa��es relevantes para o combate
void BackSpace(int repeticao); // Exclui um n�mero x de caracteres
void ApagarCaracteres(int repeticao); // Exclui um n�mero x de caracteres do console
void ApagarTudo(int repeticao); // Usa o BackSpace e o ApagarCaracteres juntos

// Inicializa��o dos personagens do jogo
void IniciarInimigo(); // Estabelece os atributos dos inimigos ao longo das fases
void IniciarJogador(); // Estabelece os atributos do jogador ao longo das fases

// Upgrades
int upgrades[5] = {0, 0, 0, 0, 0}; // Array de upgrades, � usado para saber quanto o jogador upou cada upgrada
int TesteUpgradeMax(int idx); // Testa se certo upgrade vou upado mais que o limite
char *ImprimirNomeUpgrade(char caractere); // Imprime o n�emro do upgrade que o jogador selecionou
int EscolhaDeUpgrades(int i); // Recebe um valor de escolha para qual upgrade upar e subtrai 1, para que seja compat�vel com o array
int IniciarVetEscolhidos(int length, int vetor[]); // Inicia o vetor de upgrades escolhidos
int EscolhasContem(int vetor[], int length, int numTeste); // Verifica se o jogador escolheu o mesmo upgrade mais de uma vez
void MenuDeUpgrade(); // Da output nas op��es de upgrade para o jogador

// Escolha das a��es
int Menu(); // Da output nas op��es de escolha de a��o e deixa o jogador escolher uma delas
void EscolhaAcaoJogador(int escolha); // Executa a a��o escolhida pelo jogador
int OpcaoAcaoInimigo(int valorSorteado); /* Determina a a��o que o inimigo ir� tomar de acordo com a quantidade de vida.
                                            Incrementa ou decrementa o desvio do inimigo*/
void EscolhaAcaoInimigo(op); // Executa a a��o sorteadao pelo inimigo
int ValorAleatorio(int max); // Retorna um valor inteiro, de zero at� um valor m�ximo
float Ataque(int dano, int chanceCritico); // Op��o de ataque normal, causa dano comum ou cr�tico
float AtaqueMagico(int dano, int chanceCritico, char *personagem); /* Op��o de ataque m�gico, causa dano comum ou cr�tico,
                                                                      sendo que no cr�tico h� a op��o de desviar do ataque */
void UsarPocao(); // Cura o jogador e aumenta o valor do seu escudo

// Fun��es de mec�nicas do jogo
int AcaoCombo(int comboLimite); // Faz com que o inimigo tenha um limite (combo) de ataques especiais seguidos
void CuraProximaFase(); // Maximiza a vida do jogador e acrescenta 2 po��es e 2 manas
void DecrementarDesvio(); // Diminui a chance do inimigo desviar de um ataque
void IncrementarDesvio(); // Aumenta a chance do inimigo desviar de um ataque
void Prosseguir(); // Congela o console at� que o jogador pressione espa�o
int TesteVida(int valorTeste); /* Testa a porcentagem de vida do inimigo e retorna 1 caso a vida seja igual ou
                                  menor do valor teste */
float CorrigirVida(float vida); // Transforma a vida em zero caso ela esteja negativa
int ChecarVitoria(); // Checa se o inimigo morreu
int TestarVitoria(); // Testa se o jogador matou o inimigo

// Fun��o main
int main()
{

    // Iniciar jogo
    setlocale(LC_ALL, "Portuguese");
    TelaInicial();
    IniciarJogador();

    do{

        fase++;
        IniciarInimigo();
        do{

            system("cls");
            // Output do menu
            Log();
            op = Menu();
            printf("\n");

            // A��o do jogador
            EscolhaAcaoJogador(op);

            // Teste de vit�ria
            if(TestarVitoria())
            {
                break;
            }

            Prosseguir();

            // A��o do inimigo
            printf("A��o do inimigo:\n");

            // Determina o comportamento do inimigo conforme sua vida
            op = ValorAleatorio(10);
            op = OpcaoAcaoInimigo(op);

            // A��o do inimigo
            EscolhaAcaoInimigo(op);
            Prosseguir();

            // Teste de derrota
            vidaJogador = CorrigirVida(vidaJogador);
        } while(vidaJogador > 0);

        // Testa se o jogador morreu
        if(vidaJogador > 0)
        {
            // Tela p�s batalha, apresenta o menu de upgrades e te cura para a pr�xima fase
            system("cls");
            printf("\tVoc� derrotou seu inimigo!\n");
            Prosseguir();

            system("cls");
            MenuDeUpgrade();

            system("cls");
            CuraProximaFase();
            printf("Ap�s da batalha, voc� se senta ao redor de uma fogueira.\n");
            printf("Voc� deixa as chamas te aquecerem.\n");
            Prosseguir();
            printf("\to----------------------o\n");
            printf("\tVida maximizada: %.0f\n", vidaJogador);
            printf("\tPo��o +2 e mana +2\n");
            printf("\n\tSe prepare para a fase %d!\n", fase + 1);
            LogJogador();
            printf("\to----------------------o\n");
            Prosseguir();
        }
        else
        {
            break;
        }
    } while(1);

    /* Tela final do jogo, apresenta os atributos do jogador e do inimigo,
    a fase que chegou e a pontua��o final */
    system("cls");
    printf("\t!-----Voc� Perdeu!-----!\n");
    printf("\t   Status do jogo:\n");
    Log();
    printf("\nAutor: H�garo Veloso");
    printf("\nSe encontrou algum bug ou falha no jogo, por favor mande uma mensagem para:\n");
    printf("Email: hvelosocontato@gmail.com\n");
    return 0;
}

// Fun��es do programa
// Menus do jogo
void TelaInicial()
{
    // Da output na tela de inicio do jogo
    printf("o---------- Dark Couls ----------o\n");
    printf("\nInsira seu nome sem acentos: ");

    int pos = 0;
    char ch;

    while(1)
    {
        ch = getch();

        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
        {
            putchar(ch);
            nomeDoJogador[pos++] = ch;
        }

        if(pos > 0 && ch == '\b')
        {
            pos--;
            putchar('\b');
            putchar(' ');
            putchar('\b');
        }

        if(pos > 0 && ch == '\r')
        {
            nomeDoJogador[pos] = '\0';
            break;
        }
    }
    printf("\n");
    Prosseguir();
}

void LogJogador()
{
    // Da output nas estat�sticas do jogador
    printf("\to----------------------o\n");
    printf("\t\t%s:\n", nomeDoJogador);
    printf("\tVida: %.0f\n", vidaJogador);
    printf("\tDano: %d\n", danoJogador);
    printf("\tChance de cr�tico: %d%%\n", chanceCriticoJogador);
    printf("\tMana: %d\n", manaJogador);
    printf("\tPoc�es: %d\n", pocaoJogador);
    printf("\tEscudo: %d%%\n", escudoJogador);
}

void LogInimigo()
{
    // Da output nas estat�sticas do inimigo
    printf("\t\t%d� Inimigo:\n", fase);
    printf("\tVida: %.0f\n", vidaInimigo);
    printf("\tDano: %d\n", danoInimigo);
    printf("\tChance de cr�tico: %d%%\n", chanceCriticoInimigo);
    printf("\tChance de desvio: %d%%\n", chanceDesvioInimigo);
    printf("\to----------------------o\n");
}

void Titulo()
{
    // Da output num cabe�alho com a fase e a pontua��o do jogador
    printf("\to----------------------o\n");
    printf("\t%d� Fase; Score: %d\n", fase, score);
}

void Log()
{
    // Da output em todas as informa��es relevantes para o combate
    Titulo();
    LogJogador();
    printf("\n");
    LogInimigo();
}

void BackSpace(int repeticao)
{
    // Exclui um n�mero x de caracteres
    for(int i = 0; i < repeticao; i++)
    {
        putchar('\b');
    }
}

void ApagarCaracteres(int repeticao)
{
    // Exclui um n�mero x de caracteres do console
    for(int i = 0; i < repeticao; i++)
    {
        putchar(' ');
    }
}

void ApagarTudo(int repeticao)
{
    // Usa o BackSpace e o ApagarCaracteres juntos
    ApagarCaracteres(repeticao);
    BackSpace(repeticao);
}

// Inicializa��o dos personagens do jogo
void IniciarInimigo()
{
    // Estabelece os atributos dos inimigos ao longo das fases
    danoInimigo = BASE_DANO_INIMIGO * fase;
    if(danoInimigo >= vidaJogador && vidaJogador > 20)
    {
        danoInimigo = vidaJogador - 20;
    }
    chanceCriticoInimigo = BASE_CHANCE_CRITICO_INIMIGO * fase;
    if(chanceCriticoInimigo > LIMITE_CHANCE_CRITICO_INIMIGO)
    {
        chanceCriticoInimigo = LIMITE_CHANCE_CRITICO_INIMIGO;
    }
    vidaMaximaInimigo = BASE_VIDA_INIMIGO * fase;
    vidaInimigo = vidaMaximaInimigo;
    curaInimigo = BASE_CURA_INIMIGO * fase;
    if(curaInimigo > danoJogador && danoJogador > 10)
    {
        curaInimigo = danoJogador - 10;
    }
    chanceDesvioInimigo = BASE_CHANCE_DESVIO_INIMIGO * fase;
    if(chanceDesvioInimigo > LIMITE_CHANCE_DESVIO_INIMIGO)
    {
        chanceDesvioInimigo = LIMITE_CHANCE_DESVIO_INIMIGO;
    }
    limiteDesvioAtingido = 0;
}

void IniciarJogador()
{
    // Estabelece os atributos do jogador ao longo das fases
    vidaJogador = vidaMaximaJogador;
    danoJogador = BASE_DANO_JOGADOR;
    chanceCriticoJogador = BASE_CHANCE_CRITICO_JOGADOR;
    curaJogador = BASE_CURA_JOGADOR;
    valorEscudoJogador = VALOR_ESCUDO_INICIAL_JOGADOR;
    if(valorEscudoJogador > LIMITE_ESCUDO_JOGADOR)
    {
        valorEscudoJogador = LIMITE_ESCUDO_JOGADOR;
    }
}

// Upgrades
int TesteUpgradeMax(int idx)
{
    // Testa se certo upgrade vou upado mais que o limite
    return (upgrades[idx] < LIMITE_UPGRADE) ? 1 : 0;
}

char *ImprimirNomeUpgrade(char caractere)
{
    // Imprime o n�emro do upgrade que o jogador selecionou
    switch(caractere)
    {
        case '1':
            printf(" - Aumentar Vida?");
            BackSpace(18);
            break;
        case '2':
            printf(" - Aumentar Dano?");
            BackSpace(18);
            break;
        case '3':
            printf(" - Aumentar Cr�tico?");
            BackSpace(21);
            break;
        case '4':
            printf(" - Aumentar Cura e Escudo?");
            BackSpace(27);
            break;
        case '5':
            printf(" - Aumentar Sorte?");
            BackSpace(19);
            break;
        case '6':
            printf(" - Comprar Cura e Po��o?");
            BackSpace(25);
            break;
    }
}

int EscolhaDeUpgrades(int i)
{
    /* Recebe um valor de escolha para qual upgrade upar e subtrai 1,
    para que seja compat�vel com o array */
    printf("%d� -> ", i);

    char chOp = 'n';
    int pos = 0;

    while(1)
    {
        char auxOp = getch();

        if(auxOp >= '1' && auxOp <= '6' && pos == 0)
        {
            pos++;
            putchar(auxOp);
            ImprimirNomeUpgrade(auxOp);
            chOp = auxOp;
        }

        if(auxOp == '\b' && pos > 0)
        {
            pos--;
            ApagarTudo(30);
            chOp = 'n';
        }

        if(auxOp == '\r' && pos > 0)
        {
            chOp -= '0';
            break;
        }
    }

    return (int)chOp - 1;
}

int EscolhasContem(int vetor[], int length, int numTeste)
{
    // Verifica se o jogador escolheu o mesmo upgrade mais de uma vez
    for(int idx = 0; idx < length; idx++)
    {
        if(vetor[idx] == numTeste)
        {
            return 1;
        }
    }
    return 0;
}

int IniciarVetEscolhidos(int length, int vetor[])
{
    // Inicia o vetor de upgrades escolhidos
    for(int idx = 0; idx < length; idx++)
    {
        vetor[idx] = -1;
    }
}

void MenuDeUpgrade()
{
    // Da output nas op��es de upgrade para o jogador
    printf("\to----------------------\n");
    printf("\to---- Pontos: %d\n", pontos);
    printf("\to----------------------\n");
    if(pontos == 1)
        {
        printf("\tEscolha %d upgrade:\n", pontos);
    }
    else
    {
        printf("\tEscolha %d upgrades diferentes:\n", pontos);
    }
    if(TesteUpgradeMax(0))
    {
        printf("\t[1] - Vida -> (Aumenta a vida)\n");
    }
    else
        {
        printf("\t[Max] - Vida.\n");
    }
    if(TesteUpgradeMax(1))
    {
        printf("\t[2] - Dano -> (Aumenta o dano).\n");
    }
    else
    {
        printf("\t[Max] - Dano.\n");
    }
    if(TesteUpgradeMax(2))
    {
        printf("\t[3] - Chance Cr�tico -> (Aumenta a chance de cr�tico).\n");
    }
    else
    {
        printf("\t[Max] - Chance Cr�tico.\n");
    }
    if(TesteUpgradeMax(3))
    {
        printf("\t[4] - Cura e Escudo. -> (Aumenta a cura e escudo recebido)\n");
    }
    else
    {
        printf("\t[Max] - Cura e Escudo.\n");
    }
    if(TesteUpgradeMax(4))
    {
        printf("\t[5] - Sorte. -> (Aumenta a chance de acertar ataques m�gicos)\n");
    }
    else
    {
        printf("\t[Max] - Sorte.\n");
    }
    printf("\t[6] - Po��o e Mana. -> (+2 po��es e +2 manas)\n");

    // Inicia e coloca os valores no vetor de escolha
    int escolhas[pontos];
    IniciarVetEscolhidos(pontos, escolhas);

    // Recebe as op��es de upgrade e testa se ela � ou n�o v�lida
    for(int idx = 1; idx <= pontos; idx++)
    {
        op = EscolhaDeUpgrades(idx);
        if(!EscolhasContem(escolhas, pontos, op) && TesteUpgradeMax(op))
        {
            switch(op)
            {
                case 0:
                    ApagarTudo(35);
                    printf("Vida aumentada!\n");
                    upgrades[op]++;
                    vidaMaximaJogador += BASE_VIDA_MAXIMA_JOGADOR * fase;
                    escolhas[idx - 1] = op;
                    break;
                case 1:
                    ApagarTudo(35);
                    printf("Dano aumentado!\n");
                    upgrades[op]++;
                    danoJogador += BASE_DANO_JOGADOR * fase;
                    escolhas[idx - 1] = op;
                    break;
                case 2:
                    ApagarTudo(35);
                    printf("Chance de cr�tico aumentada!\n");
                    upgrades[op]++;
                    chanceCriticoJogador += BASE_CHANCE_CRITICO_JOGADOR * fase;
                    if(chanceCriticoJogador > LIMITE_CHANCE_CRITICO_JOGADOR)
                    {
                        chanceCriticoJogador = LIMITE_CHANCE_CRITICO_JOGADOR;
                    }
                    escolhas[idx - 1] = op;
                    break;
                case 3:
                    ApagarTudo(35);
                    printf("Cura e escudo aumentados!\n");
                    upgrades[op]++;
                    curaJogador += BASE_CURA_JOGADOR * fase;
                    valorEscudoJogador += BASE_ESCUDO_JOGADOR * fase;
                    if(valorEscudoJogador > LIMITE_ESCUDO_JOGADOR)
                    {
                        valorEscudoJogador = LIMITE_ESCUDO_JOGADOR;
                    }
                    escolhas[idx - 1] = op;
                    break;
                case 4:
                    ApagarTudo(35);
                    printf("Sorte aumentada!\n");
                    upgrades[op]++;
                    sorte += fase;
                    if(sorte > LIMITE_SORTE_JOGADOR)
                    {
                        sorte = LIMITE_SORTE_JOGADOR;
                    }
                    escolhas[idx - 1] = op;
                    break;
                case 5:
                    ApagarTudo(35);
                    printf("+2 po��es e +2 manas\n");
                    pocaoJogador += 2;
                    manaJogador += 2;
                    break;
            }
        }
        else
        {
            printf("Valor inv�lido, tente outro.\n");
            idx--;
        }
    }
    Prosseguir();
}

// Escolha das a��es
int Menu()
{
    // Da output nas op��es de escolha de a��o e deixa o jogador escolher uma delas
    printf("\tEscolha uma a��o:\n");
    printf("\t[1] - Atacar.\n");
    if(manaJogador > 0)
    {
        printf("\t[2] - Ataque m�gico.\n");
    }
    if(pocaoJogador > 0)
    {
        printf("\t[3] - Po��o\n");
    }
    printf(" -> ");

    char chOp = 'n';
    int pos = 0;
    char auxOp;

    while(1)
    {
        auxOp = getch();

        if(pos == 0 && auxOp >= '1' && auxOp <= '3')
        {
            pos++;
            chOp = auxOp;
            putchar(chOp);
        }

        if(pos > 0 && auxOp == '\b')
        {
            pos--;
            putchar('\b');
            putchar(' ');
            putchar('\b');
            chOp = 'n';
        }

        if(pos > 0 && chOp != 'n' && auxOp == '\r')
        {
            chOp -= '0';
            break;
        }
    }

    return (int)chOp;
}

void EscolhaAcaoJogador(int escolha)
{
    // Executa a a��o escolhida pelo jogador
    printf("Sua a��o:\n");
    switch(op)
    {
        case 1: // Op��o do ataque comum
            printf("\tVoc� ataca!\n");

            if(ValorAleatorio(101) <= chanceDesvioInimigo)
            {
                printf("\tO inimigo desviou do seu ataque!\n");
            }
            else
            {
                danoCausado = Ataque(danoJogador, chanceCriticoJogador);
                printf("\tVoc� causa %.0f de dano no inimigo.\n", danoCausado);
                vidaInimigo -= danoCausado;
            }
            break;
        case 2: // Op��o do ataque m�gico
                if(manaJogador > 0)
                {
                    manaJogador--;
                    printf("\tVoc� usou um ataque m�gico!\n");
                if(ValorAleatorio(101) <= chanceDesvioInimigo)
                {
                    printf("\tO inimigo desviou do seu ataque!\n");
                }
                else
                {
                    danoCausado = AtaqueMagico(danoJogador, chanceCriticoJogador, "Jogador");
                    printf("\tVoc� causa %.0f de dano no inimigo.\n", danoCausado);
                    vidaInimigo -= danoCausado;
                }
            }
            else
            {
                printf("\tVoc� est� sem mana!\n");
            }
            break;
        case 3: // Op��o de cura
            if(pocaoJogador > 0)
            {
                printf("\tVoc� usou uma po��o e levantou seu escudo!\n");
                UsarPocao();
            }
            else
            {
                printf("\tVoc� est� sem po��es!\n");
            }
            break;
        default: // Caso nenhuma op��o v�lida tenha sido escolhida
            printf("\tVoc� fica parado e n�o toma nenhuma a��o.\n");
            break;
    }
}

int OpcaoAcaoInimigo(int valorSorteado)
{
    // Determina a a��o que o inimigo ir� tomar de acordo com a quantidade de vida. Incrementa ou decrementa o desvio do inimigo
    if(TesteVida(30))
    {
        if(valorSorteado < 2)
        {
            return 1;
        }
        else if(valorSorteado < 5)
        {
            return AcaoCombo(1);
        }
        else
        {
            return 3;
        }
        // Modifica a chance do inimigo desviar de um ataque conforme os turnos se passam
        if(limiteDesvioAtingido)
        {
            if(chanceDesvioInimigo > 0)
            {
                printf("\tO inimigo est� se cansando, sua agilidade diminui!\n");
                printf("\tChance de desvio -%d%%\n", VALOR_MODIFICAR_DESVIO_INIMIGO);
                DecrementarDesvio();
                chanceDesvioInimigo = (chanceDesvioInimigo < 0) ? 0 : chanceDesvioInimigo;
            }
        }
        else
        {
            printf("\tO inimigo est� com pouca vida, ele se concentra e aumenta sua agilidade!\n");
            printf("\tChance de desvio +%d%%\n", VALOR_MODIFICAR_DESVIO_INIMIGO);
            IncrementarDesvio();
        }
    }
    else if(TesteVida(60))
    {
        if(valorSorteado < 4)
        {
            return 1;
        }
        else if(valorSorteado < 8)
        {
            return AcaoCombo(2);
        }
        else
        {
            return 3;
        }
    }
    else
    {
        if(valorSorteado < 6)
        {
            return 1;
        }
        else if(valorSorteado < 9)
        {
            return AcaoCombo(3);
        }
        else
        {
            return 3;
        }
    }
}

void EscolhaAcaoInimigo(op)
{
    // Executa a a��o sorteadao pelo inimigo
    switch(op)
    {
        case 1: // Ataque comum do inimigo
            printf("\tO inimigo ataca!\n");
            if(escudoJogador == 0)
            {
                danoCausado = Ataque(danoInimigo, chanceCriticoInimigo);
                printf("\tO inimigo causa %.0f de dano em voc�.\n", danoCausado);
                vidaJogador -= danoCausado;
            }
            else
            {
                danoCausado = Ataque(danoInimigo, chanceCriticoInimigo);
                if(danoCausado > 0)
                {
                    danoCausado -= danoCausado * escudoJogador / 100;
                    printf("\tVoc� bloqueou %d%% do dano.\n", escudoJogador);
                    escudoJogador = 0;
                }
                printf("\tO inimigo causa %.0f de dano em voc�.\n", danoCausado);
                vidaJogador -= danoCausado;
            }
            break;
        case 2: // Ataque m�gico do inimigo
            printf("\tO inimigo usou um ataque m�gico!\n");
            if(escudoJogador == 0)
            {
                danoCausado = AtaqueMagico(danoInimigo, chanceCriticoInimigo, "Inimigo");
                printf("\tO inimigo causa %.0f de dano em voc�.\n", danoCausado);
                vidaJogador -= danoCausado;
            }
            else
            {
                danoCausado = AtaqueMagico(danoInimigo, chanceCriticoInimigo, "Inimigo");
                if(danoCausado > 0)
                {
                    danoCausado -= danoCausado * escudoJogador / 100;
                    printf("\tVoc� bloqueou %d%% do dano.\n", escudoJogador);
                    escudoJogador = 0;
                }
                printf("\tO inimigo causa %.0f de dano em voc�.\n", danoCausado);
                vidaJogador -= danoCausado;
            }
            break;
        case 3: // Cura do inimigo
            printf("\tO inimigo conjurou uma magia de cura!\n");
            printf("\tVida +%d\n", curaInimigo);
            vidaInimigo += curaInimigo;
            break;
    }
}

int ValorAleatorio(int max)
{
    // Retorna um valor inteiro, de zero at� um valor m�ximo
    srand(time(0));
    return rand() % max;
}

float Ataque(int dano, int chanceCritico)
{
    // Op��o de ataque normal, causa dano comum ou cr�tico
    if(ValorAleatorio(101) > chanceCritico)
    {
        return dano;
    }
    else
    {
        printf("\tDano cr�tico!\n");
        return dano * MULTIPLICADOR_NORMAL_CRITICO;
    }
}

float AtaqueMagico(int dano, int chanceCritico, char *personagem)
{
    /* Op��o de ataque m�gico, causa dano comum ou cr�tico, sendo que no cr�tico
    h� a op��o de desviar do ataque */
    if(ValorAleatorio(101) > chanceCritico)
    {
        return dano * MULTIPLICADOR_MAGICO;
    }
    else
    {
        if(personagem == "Jogador")
        {
            printf("\tEscolha uma posi��o para atacar: [1] - [2]\n");
            printf(" -> ");

            char chOp = 'n';
            int pos = 0;

            while(1)
            {
                char auxOp = getch();

                if(auxOp >= '1' && auxOp <= '2' && pos == 0)
                {
                    pos++;
                    putchar(auxOp);
                    chOp = auxOp;
                }

                if(auxOp == '\b' && pos > 0)
                {
                    pos--;
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                    chOp = 'n';
                }

                if(auxOp == '\r' && pos > 0)
                {
                    chOp -= '0';
                    break;
                }
            }

            op = (int)chOp;
            printf("\n");

            if(op != ValorAleatorio(sorte) + 1)
            {
                printf("\tO inimigo n�o conseguiu desviar!\n");
                return dano * MULTIPLICADOR_MAGICO_CRITICO;
            }
            else
            {
                printf("\tO inimigo desviou do seu ataque!\n");
                return 0;
            }
        }
        else
        {
            printf("\tEscolha uma posi��o para desviar: [1] - [2] - [3]\n");
            printf(" -> ");

            char chOp = 'n';
            int pos = 0;

            while(1)
            {
                char auxOp = getch();

                if(auxOp >= '1' && auxOp <= '3' && pos == 0)
                {
                    pos++;
                    putchar(auxOp);
                    chOp = auxOp;
                }

                if(auxOp == '\b' && pos > 0)
                {
                    pos--;
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                    chOp = 'n';
                }

                if(auxOp == '\r' && pos > 0)
                {
                    chOp -= '0';
                    break;
                }
            }

            op = (int)chOp;
            printf("\n");

            int posicaoAtaque = ValorAleatorio(3) + 1;
            int posicaoErrada1;
            int posicaoErrada2;

            if(posicaoAtaque == 1)
            {
                posicaoErrada1 = 2;
                posicaoErrada2 = 3;
            }
            else if(posicaoAtaque == 2)
            {
                posicaoErrada1 = 1;
                posicaoErrada2 = 3;
            }
            else
            {
                posicaoErrada1 = 1;
                posicaoErrada2 = 2;
            }

            if(op != posicaoErrada1 && op != posicaoErrada2)
            {
                printf("\tVoc� n�o conseguiu desviar!\n");
                return dano * MULTIPLICADOR_MAGICO_CRITICO;
            }
            else
            {
                printf("\tVoc� desviou do ataque com sucesso!\n");
                printf("\tMana +1, Po��o +1\n");
                manaJogador++;
                pocaoJogador++;
                return 0;
            }
        }
    }
}

void UsarPocao()
{
    // Cura o jogador e aumenta o valor do seu escudo
    pocaoJogador--;
    vidaJogador += curaJogador;
    escudoJogador += valorEscudoJogador;
    if(escudoJogador > LIMITE_ESCUDO_JOGADOR)
    {
        escudoJogador = LIMITE_ESCUDO_JOGADOR;
    }
    printf("\tVida +%d\n", curaJogador);
    printf("\tEscudo +%d%%\n", valorEscudoJogador);
}

// Fun��es de mec�nicas do jogo
int TesteVida(int valorTeste)
{
    /* Testa a porcentagem de vida do inimigo e retorna 1 caso a vida seja igual ou menor
    do valor teste */
    return (vidaInimigo <= (vidaMaximaInimigo * ((float)valorTeste / 100))) ? 1 : 0;
}

void IncrementarDesvio()
{
    // Aumenta a chance do inimigo desviar de um ataque
    chanceDesvioInimigo += VALOR_MODIFICAR_DESVIO_INIMIGO;
    if(chanceDesvioInimigo >= LIMITE_CHANCE_DESVIO_INIMIGO){
        chanceDesvioInimigo = LIMITE_CHANCE_DESVIO_INIMIGO;
        limiteDesvioAtingido = 1;
    }
}

void DecrementarDesvio()
{
    // Diminui a chance do inimigo desviar de um ataque
    chanceDesvioInimigo -= VALOR_MODIFICAR_DESVIO_INIMIGO;
}

float CorrigirVida(float vida)
{
    // Transforma a vida em zero caso ela esteja negativa
    return (vida <= 0) ? 0 : vida;
}

int ChecarVitoria()
{
    // Checa se o inimigo morreu
    return (vidaInimigo <= 0) ? 1 : 0;
}

void Prosseguir()
{
    // Congela o console at� que o jogador pressione espa�o
    int size = 31;
    printf("\nPresione ESPA�O para continuar.");

    char caractere;
    do{
        caractere = getch();
    } while(caractere != ' ');

    BackSpace(size);
    ApagarTudo(size);
}

int AcaoCombo(int comboLimite)
{
    // Faz com que o inimigo tenha um limite (combo) de ataques especiais seguidos
    if(combo < comboLimite)
    {
        return 2;
        combo++;
    }
    else
    {
        return 1;
        combo = 0;
    }
}

void CuraProximaFase()
{
    // Maximiza a vida do jogador e acrescenta 2 po��es e 2 manas
    if(vidaJogador <= vidaMaximaJogador)
    {
        vidaJogador = vidaMaximaJogador;
    }
    pocaoJogador += RECOMPENSA_POCAO;
    manaJogador += RECOMPENSA_MANA;
}

int TestarVitoria()
{
    // Testa se o jogador matou o inimigo
    vidaInimigo = CorrigirVida(vidaInimigo);
    if(ChecarVitoria())
    {
        score += RECOMPENSA_SCORE * fase;
        pontos = RECOMPENSA_PONTOS;
        Prosseguir();
        return 1;
    }
    return 0;
}
