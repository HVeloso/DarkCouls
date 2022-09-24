// Jogo em C
// ---------------

// Importar bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

// Declara��o de vari�veis globais
//Vari�veis base do jogador
float baseVidaMaximaJogador = 5;
int baseDanoJogador = 10;
int baseChanceCriticoJogador = 10;
int baseCurajogador = 10;
int baseEscudoJogador = 5;

// Vari�veis do jogador
float vidaMaximaJogador = 100;
int danoJogador;
int chanceCriticoJogador;
int curaJogador;
int escudoJogador = 0;
int pocaoJogador = 2;
int manaJogador = 2;
int sorte = 4;

int upgrades[5] = { 0, 0, 0, 0, 0};

int valorEscudoJogador;
float vidaJogador;

// Vari�veis base dos inimigos
int baseDanoInimigo = 10;
int baseChanceCriticoInimigo = 7;
float baseVidaInimigo = 50;
int baseCuraInimigo = 5;
int baseChanceDesvioInimigo = 5;
float vidaMaximaInimigo;

// Vari�veis dos inimigos
int danoInimigo;
int chanceCriticoInimigo;
float vidaInimigo;
int curaInimigo;
int chanceDesvioInimigo;

// Vari�veis do jogo
int fase = 0;
int baseScore = 50;
int score = 0;
int pontos = 0;
float danoCausado;
int combo = 0;
int op;

// Fun��es do programa
void LogJogador(){
    printf("\to----------------------o\n");
    printf("\tVida: %.0f\n", vidaJogador);
    printf("\tDano: %d\n", danoJogador);
    printf("\tChance de cr�tico: %d%%\n", chanceCriticoJogador);
    printf("\tMana: %d\n", manaJogador);
    printf("\tPoc�es: %d\n", pocaoJogador);
    printf("\tEscudo: %d%%\n", escudoJogador);
}

void LogInimigo(){
    printf("\t\t%d� Inimigo:\n", fase);
    printf("\tVida: %.0f\n", vidaInimigo);
    printf("\tDano: %d\n", danoInimigo);
    printf("\tChance de cr�tico: %d%%\n", chanceCriticoInimigo);
    printf("\tChance de desvio: %d%%\n", chanceDesvioInimigo);
    printf("\to----------------------o\n");
}

void Log(){
    Titulo();
    LogJogador();
    printf("\n");
    LogInimigo();
}

void Titulo(){
    printf("\to----------------------o\n");
    printf("\t%d� Fase; Score: %d\n", fase, score);
}

void IniciarInimigo(){
    danoInimigo = baseDanoInimigo * fase;
    if(danoInimigo >= vidaJogador && vidaJogador > 20){
        danoInimigo = vidaJogador - 20;
    }
    chanceCriticoInimigo = baseChanceCriticoInimigo * fase;
    if(chanceCriticoInimigo > 60){
        chanceCriticoInimigo = 60;
    }
    vidaMaximaInimigo = baseVidaInimigo * fase;
    vidaInimigo = vidaMaximaInimigo;
    curaInimigo = baseCuraInimigo * fase;
    if(curaInimigo > danoJogador && danoJogador > 10){
        curaInimigo = danoJogador - 10;
    }
    chanceDesvioInimigo = baseChanceDesvioInimigo * fase;
    if(chanceDesvioInimigo > 35){
        chanceDesvioInimigo = 35;
    }
}

void IniciarJogador(){
    vidaJogador = vidaMaximaJogador;
    danoJogador = baseDanoJogador;
    chanceCriticoJogador = baseChanceCriticoJogador;
    curaJogador = baseCurajogador;
    valorEscudoJogador = 10;
    if(valorEscudoJogador > 90){
        valorEscudoJogador = 90;
    }
}

void CuraProximaFase(){
    if(vidaJogador <= vidaMaximaJogador){
	vidaJogador = vidaMaximaJogador;
    }
    pocaoJogador += 2;
    manaJogador += 2;
}

int TesteUpgradeMax(int idx){
    return (upgrades[idx] < 10) ? 1 : 0;
}

int EscolhaDeUpgrades(int i){
    printf("%d� -> ", i);
    scanf("%d", &op);
    return op - 1;
}

int EscolhasContem(int vetor[], int length, int numTeste){
    for(int idx = 0; idx < length; idx++){
        if(vetor[idx] == numTeste){
            return 1;
        }
    }
    return 0;
}

int IniciarVetEscolhidos(int length, int vetor[]){
    for(int idx = 0; idx < length; idx++){
        vetor[idx] = -1;
    }
}

void MenuDeUpgrade(){
    printf("\to----------------------\n");
    printf("\to---- Pontos: %d\n", pontos);
    printf("\to----------------------\n");
    if(pontos == 1){
        printf("\tEscolha %d upgrade:\n", pontos);
    } else {
        printf("\tEscolha %d upgrades diferentes:\n", pontos);
    }
    if(TesteUpgradeMax(0)){
        printf("\t[1] - Vida -> (Aumenta a vida)\n");
    } else {
        printf("\t[Max] - Vida.\n");
    }
    if(TesteUpgradeMax(1)){
        printf("\t[2] - Dano -> (Aumenta o dano).\n");
    } else {
        printf("\t[Max] - Dano.\n");
    }
    if(TesteUpgradeMax(2)){
        printf("\t[3] - Chance Cr�tico -> (Aumenta a chance de cr�tico).\n");
    } else {
        printf("\t[Max] - Chance Cr�tico.\n");
    }
    if(TesteUpgradeMax(3)){
        printf("\t[4] - Cura e Escudo. -> (Aumenta a cura e escudo recebido)\n");
    } else {
        printf("\t[Max] - Cura e Escudo.\n");
    }
    if(TesteUpgradeMax(4)){
        printf("\t[5] - Sorte. -> (Aumenta a chance de acertar ataques m�gicos)\n");
    } else {
        printf("\t[Max] - Sorte.\n");
    }
    printf("\t[6] - Po��o e Mana. -> (+2 po��es e +2 manas)\n");

    int escolhas[pontos];
    IniciarVetEscolhidos(pontos, escolhas);

    for(int idx = 1; idx <= pontos; idx++){
        op = EscolhaDeUpgrades(idx);

        if(!EscolhasContem(escolhas, pontos, op) && (op > -1 && op < 6) && TesteUpgradeMax(op)){
            switch(op){
                case 0:
                    printf("Vida aumentada!\n");
                    upgrades[op]++;
                    vidaMaximaJogador += baseVidaMaximaJogador * fase;
                    escolhas[idx - 1] = op;
                    break;
                case 1:
                    printf("Dano aumentado!\n");
                    upgrades[op]++;
                    danoJogador += baseDanoJogador * fase;
                    escolhas[idx - 1] = op;
                    break;
                case 2:
                    printf("Chance de cr�tico aumentada!\n");
                    upgrades[op]++;
                    chanceCriticoJogador += baseChanceCriticoJogador * fase;
                    if(chanceCriticoJogador > 70){
                        chanceCriticoJogador = 70;
                    }
                    escolhas[idx - 1] = op;
                    break;
                case 3:
                    printf("Cura e escudo aumentados!\n");
                    upgrades[op]++;
                    curaJogador += baseCurajogador * fase;
                    valorEscudoJogador += baseEscudoJogador * fase;
                    if(valorEscudoJogador > 75){
                        valorEscudoJogador = 75;
                    }
                    escolhas[idx - 1] = op;
                    break;
                case 4:
                    printf("Sorte aumentada!\n");
                    upgrades[op]++;
                    sorte += fase;
                    if(sorte > 20){
                        sorte = 20;
                    }
                    escolhas[idx - 1] = op;
                    break;
                case 5:
                    printf("+2 po��es e +2 manas\n");
                    pocaoJogador += 2;
                    manaJogador += 2;
                    break;
            }
        } else {
            printf("Valor inv�lido, tente outro.\n");
            idx--;
        }
    }
    getch();
}

int Menu(){
    printf("\tEscolha uma a��o:\n");
    printf("\t[1] - Atacar.\n");
    if(manaJogador > 0){
        printf("\t[2] - Ataque m�gico.\n");
    }
    if(pocaoJogador > 0){
        printf("\t[3] - Po��o\n");
    }
    printf(" -> ");
    scanf("%d", &op);
    return op;
}

int ValorAleatorio(int max){
    srand(time(0));
    return rand() % max;
}

float Ataque(int dano, int chanceCritico){
    if(ValorAleatorio(101) > chanceCritico){
        return dano;
    } else {
        printf("\tDano cr�tico!\n");
        return dano * 2.25;
    }
}

float AtaqueMagico(int dano, int chanceCritico, char *personagem){
    if(ValorAleatorio(101) > chanceCritico){
        return dano * 3;
    } else {

        if(personagem == "Jogador"){

            printf("\tEscolha uma posi��o para atacar: [1] - [2]\n");
            printf(" -> ");
            scanf("%d", &op);

            if(op != ValorAleatorio(sorte) + 1){
                printf("\tO inimigo n�o conseguiu desviar!\n");
                return dano * 5;
            } else {
                printf("\tO inimigo desviou do seu ataque!\n");
                return 0;
            }
        } else {

            printf("\tEscolha uma posi��o para desviar: [1] - [2] - [3]\n");
            printf(" -> ");
            scanf("%d", &op);

            int posicaoAtaque = ValorAleatorio(3) + 1;
            int posicaoErrada1;
            int posicaoErrada2;
            if(posicaoAtaque == 1){
                posicaoErrada1 = 2;
                posicaoErrada2 = 3;
            } else if(posicaoAtaque == 2){
                posicaoErrada1 = 1;
                posicaoErrada2 = 3;
            } else {
                posicaoErrada1 = 1;
                posicaoErrada2 = 2;
            }

            if(op != posicaoErrada1 && op != posicaoErrada2){
                printf("\tVoc� n�o conseguiu desviar!\n");
                return dano * 5;
            } else {
                printf("\tVoc� desviou do ataque com sucesso!\n");
                printf("\tMana +1, Po��o +1\n");
                manaJogador++;
                pocaoJogador++;
                return 0;
            }
        }
    }
}

void UsarPocao(){
    pocaoJogador--;
    vidaJogador += curaJogador;
    escudoJogador += valorEscudoJogador;
    if(escudoJogador > 90){
        escudoJogador = 90;
    }
    printf("\tVida +%d\n", curaJogador);
    printf("\tEscudo +%d%%\n", valorEscudoJogador);
}

int TesteVida(int valorTeste){
    return (vidaInimigo <= (vidaMaximaInimigo * ((float)valorTeste / 100))) ? 1 : 0;
}

void IncrementarDesvio(){
    chanceDesvioInimigo += 5;
}

float CorrigirVida(float vida){
    return (vida <= 0) ? 0 : vida;
}

int ChecarVitoria(float vida){
    return (vida <= 0) ? 1 : 0;
}

void Prosseguir(){
    printf("\nPresione qualquer tecla para continuar.");
    getch();
}

// Fun��o main
int main()
{

    // Iniciar jogo
    setlocale(LC_ALL, "Portuguese");
    printf("o---------- Dark Couls ----------o\n");
    Prosseguir();
    IniciarJogador();

    do{

        fase++;
        IniciarInimigo();
        do{

            system("cls");
            // Output do menu
            Log();
            op = Menu();

            // A��o do jogador
            printf("Sua a��o:\n");
            switch(op){
                case 1: // Op��o do ataque comum
                    printf("\tVoc� ataca!\n");

                    if(ValorAleatorio(101) <= chanceDesvioInimigo){
                        printf("\tO inimigo desviou do seu ataque!\n");
                    } else {
                        danoCausado = Ataque(danoJogador, chanceCriticoJogador);
                        printf("\tVoc� causa %.0f de dano no inimigo.\n", danoCausado);
                        vidaInimigo -= danoCausado;
                    }
                    break;
                case 2: // Op��o do ataque m�gico
                    if(manaJogador > 0){
                        manaJogador--;
                        printf("\tVoc� usou um ataque m�gico!\n");
                        if(ValorAleatorio(101) <= chanceDesvioInimigo){
                            printf("\tO inimigo desviou do seu ataque!\n");
                        } else {
                            danoCausado = AtaqueMagico(danoJogador, chanceCriticoJogador, "Jogador");
                            printf("\tVoc� causa %.0f de dano no inimigo.\n", danoCausado);
                            vidaInimigo -= danoCausado;
                        }
                    } else {
                        printf("\tVoc� est� sem mana!\n");
                    }
                    break;
                case 3: // Op��o de cura
                    if(pocaoJogador > 0){
                        printf("\tVoc� usou uma po��o e levantou seu escudo!\n");
                        UsarPocao();
                    } else {
                        printf("\tVoc� est� sem po��es!\n");
                    }
                    break;
                default: // Caso nenhuma op��o v�lida tenha sido escolhida
                    printf("\tVoc� fica parado e n�o toma nenhuma a��o.\n");
                    break;
            }

            // Teste de vit�ria
            vidaInimigo = CorrigirVida(vidaInimigo);
            if(ChecarVitoria(vidaInimigo)){
                score += baseScore * fase;
                pontos = 3;
                Prosseguir();
                break;
            }

            // A��o do inimigo
            Prosseguir();

            printf("\n\n");
            printf("A��o do inimigo:\n");

            // Determina o comportamento do inimigo conforme sua vida
            op = ValorAleatorio(10);

            if(TesteVida(30)){

                if(op < 2){
                    op = 1;
                } else if(op < 5){

                    if(combo == 0){
                        op = 2;
                        combo = 1;
                    } else {
                        op = 1;
                        combo = 0;
                    }
                } else {
                    op = 3;
                }
                // Aumento da chance de desvio do inimigo
                printf("\tO inimigo est� com pouca vida, ele se concentra e aumenta sua agilidade!\n");
                printf("\tChance de desvio +5%%\n");
                IncrementarDesvio();

            } else if(TesteVida(60)){

                if(op < 4){
                    op = 1;
                } else if(op < 8){

                    if(combo == 0){
                        op = 2;
                        combo = 1;
                    } else {
                        op = 1;
                        combo = 0;
                    }
                } else {
                    op = 3;
                }

            } else {

                if(op < 6){
                    op = 1;
                } else if(op < 9){

                    if(combo == 0){
                        op = 2;
                        combo = 1;
                    } else {
                        op = 1;
                        combo = 0;
                    }
                } else {
                    op = 3;
                }
            }

            switch(op){
                case 1: // Ataque comum do inimigo
                    printf("\tO inimigo ataca!\n");
                    if(escudoJogador == 0){
                        danoCausado = Ataque(danoInimigo, chanceCriticoInimigo);
                        printf("\tO inimigo causa %.0f de dano em voc�.\n", danoCausado);
                        vidaJogador -= danoCausado;
                    } else {
                        danoCausado = Ataque(danoInimigo, chanceCriticoInimigo);
                        if(danoCausado > 0){
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
                    if(escudoJogador == 0){
                        danoCausado = AtaqueMagico(danoInimigo, chanceCriticoInimigo, "Inimigo");
                        printf("\tO inimigo causa %.0f de dano em voc�.\n", danoCausado);
                        vidaJogador -= danoCausado;
                    } else {
                        danoCausado = AtaqueMagico(danoInimigo, chanceCriticoInimigo, "Inimigo");
                        if(danoCausado > 0){
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

            Prosseguir();

            // Teste de derrota
            vidaJogador = CorrigirVida(vidaJogador);

        } while(vidaJogador > 0);

        if(vidaJogador > 0){

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
            printf("\n\to----------------------o\n");
            printf("\tVida maximizada: %.0f\n", vidaJogador);
            printf("\tPo��o +2 e mana +2\n");
            printf("\n\tSe prepare para a fase %d!\n", fase + 1);
            LogJogador();
            printf("\to----------------------o\n");
            Prosseguir();
        } else {
            break;
        }
    } while(1);

    system("cls");
    printf("\t!-----Voc� Perdeu!-----!\n");
    printf("\t   Status do jogo:\n");
    Log();
    printf("\nAutor: H�garo Veloso");
    printf("\nSe encontrou algum bug ou falha no jogo, por favor mande uma mensagem para:\nEmail: hvelosocontato@gmail.com\n");
    return 0;
}


// Autor: H�garo Veloso
// Email: hvelosocontato@gmail.com
