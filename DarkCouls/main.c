/*
adicionado:
 - Tela de score (com problemas);
 - Menu de escolha de telas;
 - Agora o jogador não por escolher upgrades já maximizados;
 - Atualização em como os valores aleatórios interagem com as estruturas condicionais;
 - Log do jogador agora imprime sua vida máxima quando o jogador morre;
 - Adicionado cores no console;
*/
// --- 26/09

// Autor: Hígaro Veloso
// Email: hvelosocontato@gmail.com

// Importar bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <string.h>

// Constantes de cores, pegas em: https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define DEFINIR_VERMELHO "\x1b[31m"
#define DEFINIR_VERDE "\x1b[32m"
#define DEFINIR_AMARELO "\x1b[33m"
#define DEFINIR_AZUL "\x1b[34m"
#define DEFINIR_ROXO "\x1b[35m"
#define DEFINIR_CIANO "\x1b[36m"
#define DEFINIR_PADRAO "\x1b[0m"

// Valores base do jogador
#define BASE_VIDA_MAXIMA_JOGADOR 12.0
#define BASE_DANO_JOGADOR 10
#define BASE_CHANCE_CRITICO_JOGADOR 5
#define BASE_CURA_JOGADOR 10
#define BASE_ESCUDO_JOGADOR 5
#define VALOR_ESCUDO_INICIAL_JOGADOR 5
#define VALOR_VIDA_MAXIMA_INICIAL_JOGADOR 100
#define VALOR_SORTE_INICIAL_jOGADOR 4
#define VALOR_MANA_INICIAL_JOGADOR 2
#define VALOR_POCAO_INICIAL_JOGADOR 2
#define MULTIPLICADOR_NORMAL_CRITICO 1.5
#define MULTIPLICADOR_MAGICO 2.25
#define MULTIPLICADOR_MAGICO_CRITICO 3.5

// Valores base dos inimigos
#define BASE_DANO_INIMIGO 7
#define BASE_CHANCE_CRITICO_INIMIGO 7
#define BASE_VIDA_INIMIGO 25
#define BASE_CURA_INIMIGO 8
#define BASE_CHANCE_DESVIO_INIMIGO 3
#define VALOR_MODIFICAR_DESVIO_INIMIGO 30

// Limite dos atributos do jogador
#define LIMITE_ESCUDO_JOGADOR 80
#define LIMITE_CHANCE_CRITICO_JOGADOR 99
#define LIMITE_SORTE_JOGADOR 20

// Limite dos atributos do inimigo
#define LIMITE_CHANCE_CRITICO_INIMIGO 75
#define LIMITE_CHANCE_DESVIO_INIMIGO 50

// Valores do jogo
#define GOD_MODE 9999;
#define PATH_SCORE "C:\\Users\\LENOVO\\Desktop\\jogoGen\\Arquivo\\ScoreTable.txt"
#define TAMANHO_MAXIMO_NOME 15
#define RECOMPENSA_MANA 1
#define RECOMPENSA_POCAO 1
#define RECOMPENSA_PONTOS 3
#define RECOMPENSA_SCORE 50
#define LIMITE_UPGRADE 10

// Declaração de variáveis globais
// Variáveis do jogador
char nomeJogador[TAMANHO_MAXIMO_NOME];
float vidaMaximaJogador = VALOR_VIDA_MAXIMA_INICIAL_JOGADOR;
int danoJogador;
int chanceCriticoJogador;
int curaJogador;
int escudoJogador = 0;
int pocaoJogador = VALOR_POCAO_INICIAL_JOGADOR;
int manaJogador = VALOR_MANA_INICIAL_JOGADOR;
int sorte = VALOR_SORTE_INICIAL_jOGADOR;
int multiplicadorCura = 0;

int valorEscudoJogador; // Quanto o jogador recebe de escudo por poção
float vidaJogador;

// Variáveis dos inimigos
float vidaMaximaInimigo;
int danoInimigo;
int chanceCriticoInimigo;
float vidaInimigo;
int curaInimigo;
int chanceDesvioInimigo;
int atordoado = 0;
float vidaAdicionalInimigo = 0;
int danoAdicionalInimigo = 0;
int criticoAdicionalInimigo = 0;
int curaAdicionalInimigo = 0;

// Variáveis do jogo
int fase = 0;
int score = 0;
int pontos = 0;
float danoCausado;
int combo = 0;
int limiteDesvioAtingido = 0;
int op;
int adicaoEspecial = 0;
int godmode = 0;

// Protótipo das funções do programa
// Menus do jogo
void TelaDeScore(); // Mostra as pontuações registradas
void ModoDeus(); // Trapaça: Aumenta o valor dos upgrades
void TelaInicial(); // Da output na tela de inicio do jogo
void MenuDeTelas(); // Apresenta as opções de qual tela encaminhar o jogador
void LogJogador(); // Da output nas estatísticas do jogador
void LogInimigo(); // Da output nas estatísticas do inimigo
void Titulo(); // Da output num cabeçalho com a fase e a pontuação do jogador
void Log(); // Da output em todas as informações relevantes para o combate
void BackSpace(int repeticao); // Exclui um número x de caracteres
void ApagarCaracteres(int repeticao); // Exclui um número x de caracteres do console
void ApagarTudo(int repeticao); // Usa o BackSpace e o ApagarCaracteres juntos

// Inicialização dos personagens do jogo
void IniciarInimigo(); // Estabelece os atributos dos inimigos ao longo das fases
void IniciarJogador(); // Estabelece os atributos do jogador ao longo das fases

// Upgrades
int upgrades[5] = {0, 0, 0, 0, 0}; // Array de upgrades, é usado para saber quanto o jogador upou cada upgrada
char escolhasPossiveis[6] = {'1', '2', '3', '4', '5', '6'}; // Array de escolhas possíveis para upgrades
int TesteUpgradeMax(int idx); // Testa se certo upgrade vou upado mais que o limite
char *ImprimirNomeUpgrade(char caractere); // Imprime o núemro do upgrade que o jogador selecionou
int ContemChar(char vetor[], int length, int caractereTeste); // Verifica se o jogador escolheu o um upgrade já maximizado
int EscolhaDeUpgrades(int i); // Recebe um valor de escolha para qual upgrade upar e subtrai 1, para que seja compatível com o array
int ContemInt(int vetor[], int length, int numTeste); // Verifica se o jogador escolheu o mesmo upgrade mais de uma vez
int IniciarVetEscolhidos(int length, int vetor[]); // Inicia o vetor de upgrades escolhidos
void MenuDeUpgrade(); // Da output nas opções de upgrade para o jogador
void InimigoUpgrade(); // Aumenta os atributos do inimigo aleatóriamente

// Escolha das ações
int Menu(); // Da output nas opções de escolha de ação e deixa o jogador escolher uma delas
void EscolhaAcaoJogador(int escolha); // Executa a ação escolhida pelo jogador
int OpcaoAcaoInimigo(int valorSorteado); /* Determina a ação que o inimigo irá tomar de acordo com a quantidade de vida.
                                            Incrementa ou decrementa o desvio do inimigo*/
void EscolhaAcaoInimigo(op); // Executa a ação sorteadao pelo inimigo
int ValorAleatorio(int max); // Retorna um valor inteiro, de zero até um valor máximo
float Ataque(int dano, int chanceCritico); // Opção de ataque normal, causa dano comum ou crítico
float AtaqueMagico(int dano, int chanceCritico, char *personagem); /* Opção de ataque mágico, causa dano comum ou crítico,
                                                                      sendo que no crítico há a opção de desviar do ataque */
void UsarPocao(); // Cura o jogador e aumenta o valor do seu escudo

// Funções do jogo
int AcaoCombo(int comboLimite); // Faz com que o inimigo tenha um limite (combo) de ataques especiais seguidos
void CuraProximaFase(); // Maximiza a vida do jogador e acrescenta 2 poções e 2 manas
void DecrementarDesvio(); // Diminui a chance do inimigo desviar de um ataque
void IncrementarDesvio(); // Aumenta a chance do inimigo desviar de um ataque
void Prosseguir(); // Congela o console até que o jogador pressione espaço
int TesteVida(int valorTeste); /* Testa a porcentagem de vida do inimigo e retorna 1 caso a vida seja igual ou
                                  menor do valor teste */
float CorrigirVida(float vida); // Transforma a vida em zero caso ela esteja negativa
int ChecarVitoria(); // Checa se o inimigo morreu
int TestarVitoria(); // Testa se o jogador matou o inimigo
void SaveScore(char *nomeDoJogador, int placar); // Salva o a pontuação de um jogador
void ApagarScore(); // Apaga todo o arquivo com a pontuação dos jogadores

// Função main
int main()
{

    setlocale(LC_ALL, "Portuguese");
    IniciarJogador();

    do{
        // Iniciar jogo
        MenuDeTelas();

        do{

            fase++;
            IniciarInimigo();
            do{

                system("cls");
                // Output do menu
                Log();
                op = Menu();
                printf("\n");

                // Ação do jogador
                EscolhaAcaoJogador(op);

                // Teste de vitória
                if(TestarVitoria())
                {
                    printf(DEFINIR_AMARELO "\tVocê derrotou seu inimigo!\n" DEFINIR_PADRAO);
                    Prosseguir();
                    break;
                }

                Prosseguir();

                // Ação do inimigo
                printf("Ação do inimigo:\n");

                if(!atordoado)
                {
                    // Determina o comportamento do inimigo conforme sua vida
                    op = ValorAleatorio(10);
                    op = OpcaoAcaoInimigo(op);

                    // Ação do inimigo
                    EscolhaAcaoInimigo(op);
                }
                else
                {
                    atordoado = 0;
                    printf("\tO inimigo está " DEFINIR_AMARELO "atordoado" DEFINIR_PADRAO " , ele não toma nenhuma ação!\n");
                }
                Prosseguir();

                // Teste de derrota
                vidaJogador = CorrigirVida(vidaJogador);
            } while(vidaJogador > 0);

            // Testa se o jogador morreu
            if(vidaJogador > 0)
            {
                // Tela pós batalha, apresenta o menu de upgrades e te cura para a próxima fase
                system("cls");
                MenuDeUpgrade();

                system("cls");
                CuraProximaFase();
                printf(DEFINIR_CIANO "Após da batalha, você se senta ao redor de uma " DEFINIR_VERMELHO "fogueira.\n"  DEFINIR_CIANO);
                printf("Você deixa as chamas te aquecerem.\n" DEFINIR_PADRAO);
                Prosseguir();
                printf("\to----------------------o\n");
                printf("\tVida maximizada: " DEFINIR_VERDE "%.0f\n" DEFINIR_PADRAO, vidaJogador);
                printf("\tPoção +%d e mana +%d\n", RECOMPENSA_POCAO, RECOMPENSA_MANA);
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
        a fase que chegou e a pontuação final */
        system("cls");
        printf(DEFINIR_VERMELHO "\t!-----Você Perdeu!-----!\n\n" DEFINIR_PADRAO);
        printf("\tStatus do jogo:\n");
        Log();
        printf("\n");
        SaveScore(nomeJogador, score);
        printf("\nAutor: Hígaro Veloso");
        printf("\nSe encontrou algum bug ou falha no jogo, por favor mande uma mensagem para:\n");
        printf("Email: hvelosocontato@gmail.com\n");

        Prosseguir();
        system("cls");
    } while(vidaJogador > 0);
    return 0;
}

// Funções do programa
// Menus do jogo
void TelaDeScore()
{
    // Mostra as pontuações registradas
    FILE *arquivo;
    char linha[TAMANHO_MAXIMO_NOME + 11];
    int idx = 1;

    printf(" o------------ SCORE --------------\n\n");
    arquivo = fopen(PATH_SCORE, "r");

    while(fgets(linha, TAMANHO_MAXIMO_NOME + 20, arquivo) != NULL)
    {
        printf("   %d° - %s", idx++, linha);
    }

    if(idx == 1)
    {
        printf("    -Nenhum jogador registrado!-\n");
    }

    fclose(arquivo);
    printf(" o---------------------------------\n");

    printf("\n [1]- Para apagar a lista de jogadores.");
    printf("\n [0]- Para voltar para o menu inicial.");
    printf("\n -> ");

    char opScore;
    int pos = 0;
    while(1)
    {
        char aux = getch();

        if(aux >= '0' && aux <= '1' && pos == 0)
        {
            pos++;
            putchar(aux);
            opScore = aux;
        }

        if(aux == '\b' && pos > 0)
        {
            pos--;
            putchar('\b');
            putchar(' ');
            putchar('\b');
        }

        if(aux == '\r' && pos > 0)
        {
            opScore -= '0';
            break;
        }
    }

    if(opScore == 1)
    {
        printf("\n");
        ApagarScore();
    }
}
/*
void ModoDeus()
{
    if(strcmp(&nomeJogador, "UUDDLRLRBAS"))
    {
        printf("%d", strcmp(&nomeJogador, "UUDDLRLRBAS"));----------------------------------------------------------------------------------------------------
        adicaoEspecial = GOD_MODE;
        nomeJogador[0] = '-';
        nomeJogador[1] = 'G';
        nomeJogador[2] = 'o';
        nomeJogador[3] = 'd';
        for(int idx = 4; idx < 7; idx++)
        {
            nomeJogador[idx] = '\b';
        }
        nomeJogador[4] = '\0';
        godmode = 1;
    }
}*/

void TelaInicial()
{
    // Da output na tela de inicio do jogo
    printf("o---------- Dark Couls ----------o\n");
    printf("\nInsira seu nome (sem acentos): ");

    int pos = 0;
    char ch;

    while(1)
    {
        ch = getch();

        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') && pos < TAMANHO_MAXIMO_NOME)
        {
            putchar(ch);
            nomeJogador[pos++] = ch;
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
            nomeJogador[pos] = '\0';
            break;
        }
    }

//    ModoDeus(); --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    printf("\n");
    Prosseguir();
}

void MenuDeTelas()
{
    // Apresenta as opções de qual tela encaminhar o jogador
    while(1)
    {
        printf("o---------- Dark Couls ----------o\n");
        printf("   Escolha uma opção:\n");
        printf("\n  [1] - Começar jogo.");
        printf("\n  [2] - Score.");
        printf("\n  [0] - Sair do Jogo.");
        printf("\n\n -> ");

        char chOp = 'n';
        int pos = 0;

        while(1)
        {
            char auxOp = getch();

            if(auxOp >= '0' && auxOp <= '2' && pos == 0)
            {
                pos++;
                putchar(auxOp);
                chOp = auxOp;
            }

            if(pos > 0 && auxOp == '\b')
            {
                pos--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
                chOp = 'n';
            }

            if(pos > 0 && auxOp == '\r')
            {
                chOp -= '0';
                break;
            }
        }

        system("cls");
        if((int)chOp == 1)
        {
            TelaInicial();
            break;
        }
        else if((int)chOp == 2)
        {
            TelaDeScore();
        }
        else
        {
            system("cls");
            printf("Você saiu do jogo!\n");
            Prosseguir();
            exit(1);
        }
        system("cls");
    }
}

void LogJogador()
{
    // Da output nas estatísticas do jogador
    printf("\to----------------------o\n");
    printf("\t\t%s:\n", nomeJogador);
    if(vidaJogador == 0)
    {
        printf("\tVida Max.: %.0f\n", vidaMaximaJogador);
    }
    printf("\tVida: %.0f\n", vidaJogador);
    printf("\tDano: %d\n", danoJogador);
    printf("\tChance de crítico: %d%%\n", chanceCriticoJogador);
    printf("\tMana: %d\n", manaJogador);
    printf("\tPocões: %d\n", pocaoJogador);
    printf("\tEscudo: %d%%\n", escudoJogador);
}

void LogInimigo()
{
    // Da output nas estatísticas do inimigo
    printf("\t\t%d° Inimigo:\n", fase);
    printf("\tVida: %.0f\n", vidaInimigo);
    printf("\tDano: %d\n", danoInimigo);
    printf("\tChance de crítico: %d%%\n", chanceCriticoInimigo);
    printf("\tChance de desvio: %d%%\n", chanceDesvioInimigo);
    printf("\to----------------------o\n");
}

void Titulo()
{
    // Da output num cabeçalho com a fase e a pontuação do jogador
    if(vidaJogador > 0){
        printf("\to----------------------o\n");
    }
    printf("\t%d° Fase; Score: %d\n", fase, score);
}

void Log()
{
    // Da output em todas as informações relevantes para o combate
    Titulo();
    LogJogador();
    printf("\n");
    LogInimigo();
}

void BackSpace(int repeticao)
{
    // Exclui um número x de caracteres
    for(int i = 0; i < repeticao; i++)
    {
        putchar('\b');
    }
}

void ApagarCaracteres(int repeticao)
{
    // Exclui um número x de caracteres do console
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

// Inicialização dos personagens do jogo
void IniciarInimigo()
{
    // Estabelece os atributos dos inimigos ao longo das fases
    InimigoUpgrade();

    danoInimigo = (BASE_DANO_INIMIGO * fase) + (danoAdicionalInimigo * fase);
    if(danoInimigo >= vidaJogador && vidaJogador > 20)
    {
        danoInimigo = vidaJogador - 20;
    }
    chanceCriticoInimigo = (BASE_CHANCE_CRITICO_INIMIGO * fase) + (criticoAdicionalInimigo * fase);
    if(chanceCriticoInimigo > LIMITE_CHANCE_CRITICO_INIMIGO)
    {
        chanceCriticoInimigo = LIMITE_CHANCE_CRITICO_INIMIGO;
    }
    vidaMaximaInimigo = (BASE_VIDA_INIMIGO * fase) + (vidaAdicionalInimigo * fase);
    vidaInimigo = vidaMaximaInimigo;
    curaInimigo = (BASE_CURA_INIMIGO * fase) + (curaAdicionalInimigo * fase);
    if(curaInimigo > danoJogador && danoJogador > 10)
    {
        curaInimigo = danoJogador - 10;
    }
    chanceDesvioInimigo = (!godmode) ? BASE_CHANCE_DESVIO_INIMIGO  + fase : 0;
    if(chanceDesvioInimigo > LIMITE_CHANCE_DESVIO_INIMIGO)
    {
        chanceDesvioInimigo = LIMITE_CHANCE_DESVIO_INIMIGO;
    }

    limiteDesvioAtingido = 0;
    atordoado = 0;
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
    // Imprime o nome do upgrade que o jogador selecionou
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
            printf(" - Aumentar Crítico?");
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
            printf(" - Comprar Cura e Poção?");
            BackSpace(25);
            break;
    }
}

int ContemChar(char vetor[], int length, int caractereTeste)
{
    // Verifica se o jogador escolheu o um upgrade já maximizado
    // Verifica se existe um certo caractere em um vetor de caracteres
    for(int idx = 0; idx < length; idx++)
    {
        if(vetor[idx] == caractereTeste)
        {
            return 1;
        }
    }
    return 0;
}

int EscolhaDeUpgrades(int i)
{
    /* Recebe um valor de escolha para qual upgrade upar e subtrai 1,
    para que seja compatível com o array */
    printf("%d° -> ", i);

    char chOp = 'n';
    int pos = 0;

    while(1)
    {
        char auxOp = getch();

        if(ContemChar(escolhasPossiveis, 6, auxOp) && pos == 0)
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

int ContemInt(int vetor[], int length, int numTeste)
{
    // Verifica se o jogador escolheu o mesmo upgrade mais de uma vez
    // Verifica se existe um certo número em um vetor de inteiros
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
    // Da output nas opções de upgrade para o jogador
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
        escolhasPossiveis[0] = '6';
    }
    if(TesteUpgradeMax(1))
    {
        printf("\t[2] - Dano -> (Aumenta o dano).\n");
    }
    else
    {
        printf("\t[Max] - Dano.\n");
        escolhasPossiveis[1] = '6';
    }
    if(TesteUpgradeMax(2) && chanceCriticoJogador != LIMITE_CHANCE_CRITICO_JOGADOR)
    {
        printf("\t[3] - Chance Crítico -> (Aumenta a chance de crítico).\n");
    }
    else
    {
        printf("\t[Max] - Chance Crítico.\n");
        escolhasPossiveis[2] = '6';
    }
    if(TesteUpgradeMax(3) && valorEscudoJogador != LIMITE_ESCUDO_JOGADOR)
    {
        printf("\t[4] - Cura e Escudo. -> (Aumenta a cura e escudo recebido)\n");
    }
    else
    {
        printf("\t[4] - Cura. Escudo já maximizado.\n");
        multiplicadorCura = BASE_CURA_JOGADOR;
    }
    if(TesteUpgradeMax(4) && sorte != LIMITE_SORTE_JOGADOR)
    {
        printf("\t[5] - Sorte. -> (Aumenta a chance de acertar ataques mágicos)\n");
    }
    else
    {
        printf("\t[Max] - Sorte.\n");
        escolhasPossiveis[4] = '6';
    }
    printf("\t[6] - Poção e Mana. -> (+1 poção e +1 mana)\n");
    printf("\n");

    // Inicia e coloca os valores no vetor de escolha
    int escolhas[pontos];
    IniciarVetEscolhidos(pontos, escolhas);

    // Recebe as opções de upgrade e testa se ela é ou não válida
    for(int idx = 1; idx <= pontos; idx++)
    {
        op = EscolhaDeUpgrades(idx);
        if(!ContemInt(escolhas, pontos, op) && TesteUpgradeMax(op))
        {
            switch(op)
            {
                case 0:

                    ApagarTudo(35);
                    float vidaAdicionalJogador = BASE_VIDA_MAXIMA_JOGADOR * fase + adicaoEspecial;
                    printf(DEFINIR_VERDE "Vida aumentada! +%.0f\n" DEFINIR_PADRAO, vidaAdicionalJogador);

                    upgrades[op]++;
                    vidaMaximaJogador += vidaAdicionalJogador;
                    escolhas[idx - 1] = op;

                    break;
                case 1:

                    ApagarTudo(35);
                    int danoAdicionalJogador = BASE_DANO_JOGADOR * fase + adicaoEspecial;
                    printf(DEFINIR_VERMELHO "Dano aumentado! +%d\n" DEFINIR_PADRAO, danoAdicionalJogador);

                    upgrades[op]++;
                    danoJogador += danoAdicionalJogador + adicaoEspecial;
                    escolhas[idx - 1] = op;

                    break;
                case 2:

                    ApagarTudo(35);
                    int criticoAdicionalJogador = BASE_CHANCE_CRITICO_JOGADOR * fase + adicaoEspecial;
                    printf(DEFINIR_AMARELO "Chance de crítico aumentada! +%d%%\n" DEFINIR_PADRAO, criticoAdicionalJogador);

                    upgrades[op]++;
                    chanceCriticoJogador += criticoAdicionalJogador;
                    if(chanceCriticoJogador > LIMITE_CHANCE_CRITICO_JOGADOR)
                    {
                        chanceCriticoJogador = LIMITE_CHANCE_CRITICO_JOGADOR;
                    }
                    escolhas[idx - 1] = op;

                    break;
                case 3:

                    ApagarTudo(35);
                    int curaAdicionalJogador = multiplicadorCura + (BASE_CURA_JOGADOR * fase) + adicaoEspecial;
                    int escudoAdicionalJogador = BASE_ESCUDO_JOGADOR * fase + adicaoEspecial;
                    if(escudoAdicionalJogador > 30)
                    {
                        escudoAdicionalJogador = 30;
                    }

                    printf(DEFINIR_VERDE "Cura ");
                    if(valorEscudoJogador != LIMITE_ESCUDO_JOGADOR)
                    {
                        printf("e escudo aumentados! +%d cura e +%d%% escudo"
                               , curaAdicionalJogador, escudoAdicionalJogador);
                        curaJogador += curaAdicionalJogador;
                        valorEscudoJogador += escudoAdicionalJogador;
                    }
                    else
                    {
                        printf("aumentada! +%d", curaAdicionalJogador);
                        curaJogador += curaAdicionalJogador;
                    }
                    printf("\n" DEFINIR_PADRAO);

                    upgrades[op]++;
                    if(valorEscudoJogador > LIMITE_ESCUDO_JOGADOR)
                    {
                        valorEscudoJogador = LIMITE_ESCUDO_JOGADOR;
                    }
                    escolhas[idx - 1] = op;

                    break;
                case 4:

                    ApagarTudo(35);
                    printf(DEFINIR_VERDE "Sorte aumentada!\n"  DEFINIR_PADRAO);
                    upgrades[op]++;
                    sorte += 1 + adicaoEspecial;
                    if(sorte > LIMITE_SORTE_JOGADOR)
                    {
                        sorte = LIMITE_SORTE_JOGADOR;
                    }
                    escolhas[idx - 1] = op;

                    break;
                case 5:

                    ApagarTudo(35);
                    printf(DEFINIR_VERDE "+%d poção e +%d mana\n"  DEFINIR_PADRAO, RECOMPENSA_POCAO + adicaoEspecial, RECOMPENSA_MANA + adicaoEspecial);
                    pocaoJogador += RECOMPENSA_POCAO + adicaoEspecial;
                    manaJogador += RECOMPENSA_MANA + adicaoEspecial;

                    break;
            }
        }
        else
        {
            printf("Valor inválido, tente outro.\n");
            idx--;
        }
    }
    Prosseguir();
}

void InimigoUpgrade()
{
    // Aumenta os atributos do inimigo aleatóriamente
    int quantidadeUpada = 0;
    int pontosInimigo = (fase % 2 == 0) ? 2 : 1;

    while(quantidadeUpada < pontosInimigo)
    {
        int opIn = ValorAleatorio(4);

        switch(opIn)
        {
            case 0:
                vidaAdicionalInimigo += BASE_VIDA_INIMIGO;
                break;
            case 1:
                danoAdicionalInimigo += BASE_DANO_INIMIGO;
                break;
            case 2:
                criticoAdicionalInimigo += BASE_CHANCE_CRITICO_INIMIGO;
                break;
            case 3:
                curaAdicionalInimigo += BASE_CURA_INIMIGO;
                break;
        }
        quantidadeUpada++;
    }
}

// Escolha das ações
int Menu()
{
    // Da output nas opções de escolha de ação e deixa o jogador escolher uma delas
    printf("\tEscolha uma ação:\n");
    printf("\t[1] - Atacar.\n");
    if(manaJogador > 0)
    {
        printf("\t[2] - Ataque mágico.\n");
    }
    if(pocaoJogador > 0)
    {
        printf("\t[3] - Poção\n");
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
    // Executa a ação escolhida pelo jogador
    printf("Sua ação:\n");
    switch(op)
    {
        case 1: // Opção do ataque comum
            printf("\tVocê ataca!\n");

            if(ValorAleatorio(100) + 1 <= chanceDesvioInimigo && atordoado == 0)
            {
                printf("\tO inimigo desviou do seu ataque!\n");
            }
            else
            {
                danoCausado = Ataque(danoJogador, chanceCriticoJogador);
                printf("\tVocê causa " DEFINIR_VERMELHO "%.0f" DEFINIR_PADRAO " de dano no inimigo.\n", danoCausado);
                vidaInimigo -= danoCausado;
            }
            break;
        case 2: // Opção do ataque mágico
                if(manaJogador > 0)
                {
                    manaJogador--;
                    printf("\tVocê usou um ataque mágico!\n");
                if(ValorAleatorio(100) + 1 <= chanceDesvioInimigo && atordoado == 0)
                {
                    printf("\tO inimigo desviou do seu ataque!\n");
                }
                else
                {
                    danoCausado = AtaqueMagico(danoJogador, chanceCriticoJogador, "Jogador");
                    printf("\tVocê causa " DEFINIR_VERMELHO "%.0f" DEFINIR_PADRAO " de dano no inimigo.\n", danoCausado);
                    vidaInimigo -= danoCausado;
                }
            }
            else
            {
                printf("\tVocê está sem mana!\n");
            }
            break;
        case 3: // Opção de cura
            if(pocaoJogador > 0)
            {
                printf("\tVocê usou uma poção e levantou seu escudo!\n");
                UsarPocao();
            }
            else
            {
                printf("\tVocê está sem poções!\n");
            }
            break;
        default: // Caso nenhuma opção válida tenha sido escolhida
            printf("\tVocê fica parado e não toma nenhuma ação.\n");
            break;
    }
}

int OpcaoAcaoInimigo(int valorSorteado)
{
    // Determina a ação que o inimigo irá tomar de acordo com a quantidade de vida. Incrementa ou decrementa o desvio do inimigo
    if(TesteVida(30))
    {
        if(valorSorteado < 2)
        {
            return 1;
        }
        else if(valorSorteado < 4)
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
                printf("\tO inimigo está se cansando, sua agilidade diminui!\n");
                printf("\tChance de desvio -%d%%\n", VALOR_MODIFICAR_DESVIO_INIMIGO);
                DecrementarDesvio();
                chanceDesvioInimigo = (chanceDesvioInimigo < 0) ? 0 : chanceDesvioInimigo;
            }
        }
        else
        {
            printf("\tO inimigo está com pouca vida, ele se concentra e aumenta sua agilidade!\n");
            printf("\tChance de desvio +%d%%\n", VALOR_MODIFICAR_DESVIO_INIMIGO);
            IncrementarDesvio();
        }
    }
    else if(TesteVida(60))
    {
        if(valorSorteado < 2)
        {
            return 1;
        }
        else if(valorSorteado < 5)
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
        else if(valorSorteado < 8)
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
    // Executa a ação sorteadao pelo inimigo
    switch(op)
    {
        case 1: // Ataque comum do inimigo
            printf("\tO inimigo ataca!\n");
            if(escudoJogador == 0)
            {
                danoCausado = Ataque(danoInimigo, chanceCriticoInimigo);
                printf("\tO inimigo causa " DEFINIR_VERMELHO "%.0f" DEFINIR_PADRAO " de dano em você.\n", danoCausado);
                vidaJogador -= danoCausado;
            }
            else
            {
                danoCausado = Ataque(danoInimigo, chanceCriticoInimigo);
                if(danoCausado > 0)
                {
                    danoCausado -= danoCausado * escudoJogador / 100;
                    printf("\tVocê bloqueou " DEFINIR_CIANO "%d%%" DEFINIR_PADRAO " do dano.\n", escudoJogador);
                    escudoJogador = 0;
                }
                printf("\tO inimigo causa " DEFINIR_VERMELHO "%.0f" DEFINIR_PADRAO " de dano em você.\n", danoCausado);
                vidaJogador -= danoCausado;
            }
            break;
        case 2: // Ataque mágico do inimigo
            printf("\tO inimigo usou um ataque mágico!\n");
            if(escudoJogador == 0)
            {
                danoCausado = AtaqueMagico(danoInimigo, chanceCriticoInimigo, "Inimigo");
                printf("\tO inimigo causa " DEFINIR_VERMELHO "%.0f" DEFINIR_PADRAO " de dano em você.\n", danoCausado);
                vidaJogador -= danoCausado;
            }
            else
            {
                danoCausado = AtaqueMagico(danoInimigo, chanceCriticoInimigo, "Inimigo");
                if(danoCausado > 0)
                {
                    danoCausado -= danoCausado * escudoJogador / 100;
                    printf("\tVocê bloqueou " DEFINIR_CIANO "%d%%" DEFINIR_PADRAO " do dano.\n", escudoJogador);
                    escudoJogador = 0;
                }
                printf("\tO inimigo causa " DEFINIR_VERMELHO "%.0f" DEFINIR_PADRAO " de dano em você.\n", danoCausado);
                vidaJogador -= danoCausado;
            }
            break;
        case 3: // Cura do inimigo
            printf("\tO inimigo conjurou uma magia de cura!\n");
            printf("\tVida " DEFINIR_VERDE "+%d\n" DEFINIR_PADRAO, curaInimigo);
            vidaInimigo += curaInimigo;
            break;
    }
}

int ValorAleatorio(int max)
{
    // Retorna um valor inteiro, de zero até um valor máximo
    srand(time(0));
    return rand() % max;
}

float Ataque(int dano, int chanceCritico)
{
    // Opção de ataque normal, causa dano comum ou crítico
    if(ValorAleatorio(100) + 1 > chanceCritico)
    {
        return dano;
    }
    else
    {
        printf("\tDano crítico!\n");
        return dano * MULTIPLICADOR_NORMAL_CRITICO;
    }
}

float AtaqueMagico(int dano, int chanceCritico, char *personagem)
{
    /* Opção de ataque mágico, causa dano comum ou crítico, sendo que no crítico
    há a opção de desviar do ataque */
    if(ValorAleatorio(100) + 1 > chanceCritico)
    {
        return dano * MULTIPLICADOR_MAGICO;
    }
    else
    {
        if(personagem == "Jogador")
        {
            printf("\tEscolha uma posição para atacar: [1] - [2]\n");
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

            int checkAtordoado = atordoado;
            if(!checkAtordoado && op == ValorAleatorio(sorte) + 1 && chanceDesvioInimigo > 0)
            {
                printf("\tO inimigo desviou do seu ataque!\n");
                return 0;
            }
            else
            {
                printf("\tO inimigo não conseguiu desviar!\n");
                if(!checkAtordoado)
                {
                    printf("\tO inimigo está " DEFINIR_AMARELO "atordoado!\n" DEFINIR_PADRAO);
                    atordoado = 1;
                }
                return dano * MULTIPLICADOR_MAGICO_CRITICO;
            }
        }
        else
        {
            printf("\tEscolha uma posição para desviar: [1] - [2] - [3]\n");
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
                printf("\tVocê não conseguiu desviar!\n");
                return dano * MULTIPLICADOR_MAGICO_CRITICO;
            }
            else
            {
                printf("\tVocê desviou do ataque com sucesso!\n");
                printf("\tO inimigo está " DEFINIR_AMARELO "atordoado!\n" DEFINIR_PADRAO);
                atordoado = 1;
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
    printf("\tVida " DEFINIR_VERDE "+%d\n" DEFINIR_PADRAO, curaJogador);
    printf("\tEscudo " DEFINIR_VERDE "+%d%%\n" DEFINIR_PADRAO, valorEscudoJogador);
}

// Funções de mecânicas do jogo
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
    if(chanceDesvioInimigo >= LIMITE_CHANCE_DESVIO_INIMIGO && !godmode){
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
    // Congela o console até que o jogador pressione espaço
    int size = 31;
    printf("\nPresione ESPAÇO para continuar.");

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
    // Maximiza a vida do jogador e acrescenta 2 poções e 2 manas
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

void SaveScore(char *nomeDoJogador, int scoreDoJogador)
{
    // Salva o a pontuação de um jogador
    if(score > 0)
    {
        FILE *arquivo;
        arquivo = fopen(PATH_SCORE, "a");

        fprintf(arquivo, "%s: ", nomeDoJogador);
        fprintf(arquivo, "%d pontos\n", scoreDoJogador);

        fclose(arquivo);
        printf(DEFINIR_VERDE "\to--- SCORE SALVO! ---o\n" DEFINIR_PADRAO);
    }
    else
    {
        printf(DEFINIR_VERMELHO "\to--- Score muito baixo para salvar! ---o\n" DEFINIR_PADRAO);
    }
}

void ApagarScore()
{
    // Apaga todo o arquivo com a pontuação dos jogadores
    FILE *arquivo;
    arquivo = fopen(PATH_SCORE, "w");

    fclose(arquivo);

    printf(DEFINIR_VERMELHO " o--- SCORE APAGADO! ---o\n" DEFINIR_PADRAO);

    Prosseguir();
}
