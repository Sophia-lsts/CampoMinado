#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define RESET "\033[0m"
#define NEGRITO "\033[1m"
#define ITALICO "\033[3m"
#define CIANO "\033[36m"
#define PRETO "\033[30m"
#define AZUL "\033[34m"
#define CINZA "\033[37m"
#define VERDE "\033[32m"
#define MAGENTA "\033[35m"
#define VERMELHO "\033[31m"
#define fundoBranco "\033[47m"
#define fundoVerde "\033[42m"
#define fundoVermelho "\033[41m"
#define fundoMagenta "\033[45m"
#define tamCampo 7
#define numMaxPlayers 20

enum casas {Vazio = 0, umaProx = 1, duasProx = 2, tresProx = 3, quatroProx = 4, naoRevelado = 5, Bomba = 6};
int lose = 1;
int revelados[tamCampo][tamCampo];
int M[tamCampo][tamCampo];

typedef struct{
        char nome[40];
        int dificuldade;
        int pontuação;
    } Infos;

void clean(){
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif
}

void menu(){

    char start;

    printf("\n\n\n\n\n\n\n");
    printf("                                                          ");
    printf(MAGENTA fundoBranco "B E M     V I N D O      A O     C A M P O     M I N A D O !" RESET);
    printf("\n\n");
    printf("                                                                                         ");
    printf(VERMELHO "_❈ \n" RESET);
    printf("                                                                                        /\n");
    printf("                                                                                   ");
    printf(PRETO "✩✩✩✩✩✩\n");
    printf("                                                                                 ✩✩✩✩✩✩✩✩✩\n");
    printf("                                                                                ✩✩✩✩✩✩✩✩✩✩✩\n");
    printf("                                                                                 ✩✩✩✩✩✩✩✩✩\n");
    printf("                                                                                  ✩✩✩✩✩✩✩\n\n" RESET);
    printf("                                                                      ");
    printf(MAGENTA fundoBranco "Clique qualquer tecla para começar!" RESET);

    do{
    scanf("%c", &start);
    } while(start != '\n');
    
    return;

}

int verificaCoordenadas(int linha, int coluna) {
    if (linha < 0 || linha >= tamCampo || coluna < 0 || coluna >= tamCampo) return 0; 
    return 1; 
}

void verificaVizinhos(int linha, int coluna){

    int i,j;
    int quantBomb = 0;


    for (i = linha - 1; i <= linha + 1; i++) {
        for (j = coluna - 1; j <= coluna + 1; j++) {
            if(M[i][j] == Bomba && verificaCoordenadas(i,j) == 1) quantBomb++;
            if(i == linha && j == coluna) continue;
        }
    }

    switch(quantBomb){
        case 0:
            M[linha][coluna] = Vazio;
            break;
        case 1:
            M[linha][coluna] = umaProx;
            break;
        case 2:
            M[linha][coluna] = duasProx;
            break;
        case 3:
            M[linha][coluna] = tresProx;
            break;
        case 4:
            M[linha][coluna] = quatroProx;
            break;
        default:
            M[linha][coluna] = Vazio;
            break;
    }


}

void preencheCampo(){

    int i;
    int j;

     for (int i = 0; i < tamCampo; i++) {
        for (int j = 0; j < tamCampo; j++) {
            revelados[i][j] = naoRevelado;
            if (verificaCoordenadas(i, j) && M[i][j] != Bomba) verificaVizinhos(i, j);
        }
     }

}

int plantaBomba(Infos *user){

    int quantBombT = 0;
    int linBomb;
    int colBomb;
    int qBombVaria;

    srand(time(NULL));

    switch(user->dificuldade){
        case 1:
            qBombVaria = tamCampo/2;
            break;
        case 2:
            qBombVaria = tamCampo/2+tamCampo/3;
            break;
        case 3:
            qBombVaria = tamCampo;
            break;
    }

    while(quantBombT < qBombVaria){

        do{
            linBomb = ((rand() % tamCampo)); //antes tava mod 5 + 1, o que poderia gerar índice fora da matriz
            colBomb = ((rand() % tamCampo));
            }
        while((M[linBomb][colBomb] == Bomba)); //não plantar 2 bombas no mesmo lugar

        quantBombT++;
        M[linBomb][colBomb] = Bomba;

    }

    return quantBombT;

}

void revela(int linha, int coluna, Infos *user){ //chamada recursiva

    if(!verificaCoordenadas(linha,coluna) || revelados[linha][coluna] != naoRevelado){
        return;
    }

    revelados[linha][coluna] = M[linha][coluna];

    user->pontuação += 25;

    if(M[linha][coluna] == Vazio){
        for (int i = linha - 1; i <= linha + 1; i++) {
            for (int j = coluna - 1; j <= coluna + 1; j++) {
                    if(!verificaCoordenadas(i,j)) continue;
                    if(revelados[i][j] != Vazio) { //isso é diferente de M[i][j] != Vazio !!!
                        if(i == linha && j == coluna) continue;
                        revela(i, j,user);
                        }
                    /*revelados[i][j] != Vazio, para não chamar recursão de novo em células já vazias e reveladas,
                    se essa condição for verdadeira, os revelados[i][j] == naoRevelados entrarão, sobre números revelados, nem chegarão aqui*/
                }
            }
    }
}

void campoRevelado(){

   int qLinhas = ((tamCampo-3)*3)+(tamCampo-3);

    printf(AZUL "\n                                                                      |" RESET);
        for (int i = 0; i < tamCampo; i++) printf(CIANO " %d |" RESET ,i);
        printf(AZUL "\n                                                                   ————————————————" RESET);
        for(int i=0;i<qLinhas;i++) printf(AZUL "—" RESET);
        for (int i = 0; i < tamCampo; i++) {
            printf("    \n");
            printf(CIANO "                                                                    %d |"RESET ,i);
            for (int j = 0; j < tamCampo; j++) {
                if(j == tamCampo -1){
                    if(M[i][j] != naoRevelado){
                        if(M[i][j] == Vazio) printf(AZUL "   |\n" RESET);
                        else if(M[i][j] == Bomba) printf(" ⦰ |\n" RESET);
                        else printf(CINZA " %d |\n"RESET , M[i][j]);
                        printf(AZUL "                                                                   ————————————————" RESET);
                        for(int i=0;i<qLinhas;i++) printf(AZUL "—" RESET);
                    }
                }
                else {
                    if(M[i][j] == Vazio) printf(CINZA "   |" RESET);
                    else if(M[i][j] == Bomba) printf(" ⦰ |");
                    else printf(CINZA " %d |" RESET , M[i][j]);
                }
            }
        }
    printf(AZUL "\n                                                                   ————————————————" RESET);
    for(int i=0;i<qLinhas;i++) printf(AZUL "—" RESET);
}

void prtMatriz(){

    int qLinhas = ((tamCampo-3)*3)+(tamCampo-3);

    printf(AZUL "\n                                                                      |" RESET);
        for (int i = 0; i < tamCampo; i++) printf(CIANO " %d |" RESET ,i);
        printf(AZUL "\n                                                                   ————————————————" RESET);
        for(int i=0;i<qLinhas;i++) printf(AZUL "—" RESET);
        for (int i = 0; i < tamCampo; i++) {
            printf("    \n");
            printf(CIANO "                                                                    %d |"RESET ,i);
            for (int j = 0; j < tamCampo; j++) {
                if(j == tamCampo -1){
                    if(revelados[i][j] != naoRevelado){
                        if(revelados[i][j] == Vazio) printf(AZUL "   |\n" RESET);
                        else if(revelados[i][j] == Bomba) printf(" o* |\n" RESET);
                        else printf(CINZA " %d |\n"RESET , revelados[i][j]);
                        printf(AZUL "                                                                   ————————————————" RESET);
                        for(int i=0;i<qLinhas;i++) printf(AZUL "—" RESET);
                    }
                    else {
                        printf("▆▆▆|\n");
                        printf(AZUL "                                                                   ————————————————" RESET);
                        for(int i=0;i<qLinhas;i++) printf(AZUL "—" RESET);
                    }
                }
                else if (revelados[i][j] == naoRevelado){
                    printf(CINZA "▆▆▆|" RESET); 
                    }
                else {
                    if(revelados[i][j] == Vazio) printf(CINZA "   |" RESET);
                    else if(revelados[i][j] == Bomba) printf(" ◯*|");
                    else printf(CINZA " %d |" RESET , revelados[i][j]);
                }
            }
        }
    }

int win(int limitaBomba){

    int cont = 0;

    for (int i = 0; i < tamCampo; i++) {
        for (int j = 0; j < tamCampo; j++) {
            if(revelados[i][j] != naoRevelado) cont++; 
        }
    }

    if(cont == ((tamCampo*tamCampo) - limitaBomba)) return 1;
    else return 0;

}

void game(Infos *user){

    int linha, coluna;

    int bombDefinidas = plantaBomba(user);
    preencheCampo();
    clean();

    while(lose == 1 || win(bombDefinidas) == 0){

        printf("\n\n\n\n\n\n");
        prtMatriz();

        do{
            do{
                printf(MAGENTA "\n\n                                                                      Digite a linha que deseja:" );
                scanf("%d", &linha);
                if(linha < 0 || linha > tamCampo) {
                printf("\n                                                                        ");
                printf(fundoBranco VERMELHO "Digite uma linha válida!" RESET);
                printf("\n");
                sleep(1);
                clean();
                prtMatriz();
                }
            }
            while(linha < 0 || linha > tamCampo);

            printf("                                                                      Digite a coluna que deseja:" RESET);
            scanf("%d", &coluna);
            if(!verificaCoordenadas(linha,coluna)) {
                printf("\n");
                printf("                                                                       ");
                printf(VERMELHO fundoBranco "Digite uma coluna válida!\n" RESET);
                printf("\n");
                sleep(1);
                clean();
                printf("\n\n\n\n\n\n");
                prtMatriz();
                }
            else if(revelados[linha][coluna] != naoRevelado){
                printf("\n");
                printf("                                                                     ");
                printf(VERMELHO fundoBranco "Essa célula já foi selecionada." RESET);
                printf("\n");
                sleep(1);
                clean();
                printf("\n\n\n\n\n\n");
                prtMatriz();
            }
        }
        while(!verificaCoordenadas(linha,coluna) || revelados[linha][coluna] != naoRevelado);

        printf("\n");
        printf("                                                                      ");
        printf("Você selecionou a coordenada:" RESET);
        printf("\n");
        printf("                                                                         ");
        printf(fundoMagenta "Linha: %d | Coluna: %d" RESET, linha,coluna);
        printf("\n");
        sleep(1);

        switch(M[linha][coluna]){
            case Bomba:
                clean();
                revelados[linha][coluna] = M[linha][coluna];
                printf("\n\n\n\n");
                prtMatriz();
                printf(MAGENTA "\n\n                                                                Você selecionou um local com bomba :c\n" RESET);
                sleep(7);
                clean();
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
                printf(PRETO "                                                                     ⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤\n" RESET);
                printf("                                                                    ⎰ ");
                printf(fundoVermelho"                                " RESET);
                printf(" ⎱\n" RESET);
                printf("                                                                    ⟪ ");
                printf(fundoVermelho " D    E     F     E     A     T " RESET);
                printf(" ⟫\n" );
                printf("                                                                    ");
                printf("⎱ ");
                printf(fundoVermelho "                                " RESET);
                printf(" ⎰\n");
                printf("                                                                     ");
                printf(PRETO "⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤\n" RESET);
                sleep(3);
                clean();
                return;
            case Vazio:
                revela(linha,coluna,user);
                break;
            default:
                revelados[linha][coluna] = M[linha][coluna];
                user->pontuação += 25;
                break;
        }

        if(win(bombDefinidas)) {
            lose = 0;
            clean();
            printf("\n\n\n\n\n\n");
            printf("                                                                         ");
            printf(VERDE fundoBranco " P A R A B É N S ! " RESET);
            printf("\n\n");
            printf("                                                            ");
            printf(VERDE fundoBranco " V O C Ê     E N C E R R O U     O     J O G O! " RESET);
            printf("\n\n");
            campoRevelado();
            sleep(7);
            clean();
            printf("\n\n\n\n\n");
            clean();
            printf("\n\n\n\n\n\n\n\n\n\n\n\n");
            printf(PRETO "                                                                     ⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤\n" RESET);
            printf("                                                                    ⎰ ");
            printf(fundoBranco"                               " RESET);
            printf(" ⎱\n");
            printf("                                                                    ⟪ ");
            printf(VERDE fundoBranco" V   I    C    T    O    R   Y " RESET);
            printf(" ⟫\n");
            printf("                                                                    ⎱ ");
            printf(fundoBranco "                               " RESET);
            printf(" ⎰\n");
            printf(PRETO "                                                                     ⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤⪤\n" RESET);
            sleep(7); 
            return;
        }
            clean();

    }
    printf("\n\n");

}

void player(Infos *user){

    char nome[20];
    char sobrenome[20];
    char nomeCompleto[50];
    int nível;

    printf("\n\n\n\n\n\n\n\n\n");

    printf(MAGENTA "                                                                         Digite seu 1º nome: " RESET);
    fgets(nome,sizeof(nome),stdin);
    nome[strlen(nome)-1] = '\0';
    printf(MAGENTA "\n                                                                         Digite seu sobrenome: " RESET);
    fgets(sobrenome,sizeof(sobrenome),stdin);

    snprintf(user->nome, sizeof(nomeCompleto), "%s %s", nome, sobrenome);

    do {
    printf(MAGENTA "\n                                                            Selecione um nível de jogo entre esses / 1 / 2 / 3 /: " RESET);
    scanf("%d", &user->dificuldade);
        if(user->dificuldade > 3 || user->dificuldade < 1){
            printf("\n                                                                     ");
            printf(fundoBranco VERMELHO "Digite uma dificuldade válida!" RESET);
            printf("\n");
            sleep(1);
            printf("\r");  
            printf("                  ");  
            printf("\r");
        }
    } while (user->dificuldade > 3 || user->dificuldade < 1);

} 

void points(Infos *user, int *minutes){

    int intervalos[5] = {1,2,3,4,5};
    int pontuações[5] = {300,200,80,50,25};

    if(!lose) user->pontuação += 500;

    switch(user->dificuldade){
        case 1:
            break;
        case 2:
            user->pontuação +=600;
            break;
        case 3:
            user->pontuação += 900;
            break;
    }

    if((user->dificuldade > 2) && !lose){ //bônus de tempo só vai pra dificuldade 3
        for(int i = 0; i < 6 ; i++){
            if(*minutes < intervalos[i]) {
                user->pontuação += pontuações[i]; 
                break;
            }
        }
    }

    if(lose) user->pontuação = 0;

}

void formatTime(int *time, int *seg, int *min){

    *seg = *time % 60;
    *min = (*time - *seg) / 60;

}

int addPlayer(int *ranking, int *minutos, int *segundos, Infos *user){

    FILE *file = fopen("ranking.txt", "a"); //se existir ele deleta e cria novo vazio || sobreescreve arquivo

    if (file == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    fprintf(file, "\n     %d         %d         %02d:%02d          %d         %s",*ranking, user->pontuação,*minutos,*segundos,user->dificuldade,user->nome);
    fclose(file);
                   
}

int orderRanking(int *minutos, int *segundos, Infos *users[]) {
    
    FILE *file1 = fopen("ranking.txt", "r");

    if (file1 == NULL) {
        perror("Erro ao abrir arquivo.");
        return 1;
    }

    int ignora = 0;
    char linha[100];
    int cont = 0;

        fgets(linha, sizeof(linha), file1); //pula o cabeçalho

        while (fgets(linha, sizeof(linha), file1) != NULL) {
            if (strlen(linha) <= 1) continue;
            users[cont] = (Infos *)malloc(sizeof(Infos));

            if (users[cont] == NULL) {
                perror("Erro ao alocar memória.");
                fclose(file1);
                return 1;
            }

            sscanf(linha, "     %d         %d         %02d:%02d          %d         %[^\n]",
                &ignora,
                &users[cont]->pontuação,
                &minutos[cont],
                &segundos[cont],
                &users[cont]->dificuldade,
                users[cont]->nome);

                cont++;
        }

    if(cont > 1){
        for (int i = 0; i < cont; i++) {
            for (int j = i + 1; j < cont; j++) {
                if (users[i]->pontuação < users[j]->pontuação) {
                    Infos *temp = users[i];
                    int aux1 = minutos[i];
                    int aux2 = segundos[i];
                    minutos[i] = minutos[j];
                    segundos[i] = segundos[j];
                    users[i] = users[j];
                    users[j] = temp;
                    minutos[j] = aux1;
                    segundos[j] = aux2;
                }
            }
        }

        for (int i = 0; i < cont; i++) {
            for (int j = i + 1; j < cont; j++) {
                if (users[i]->pontuação == users[j]->pontuação) {
                    if(minutos[i] > minutos[j]){
                        int aux = minutos[i];
                        minutos[i] = minutos[j];
                        minutos[j] = aux;
                    }
                    else if(minutos[i] == minutos[j]){
                        if(segundos[i] > segundos[j]){
                            int auxx = segundos[i];
                            segundos[i] = segundos[j];
                            segundos[j] = auxx;
                        }
                    }
                }
            }
        }
    }

        fclose(file1);

    return cont;
}

int rewritingRanking(int *ranking,int *minutos, int *segundos, Infos *users[], int cont){

    FILE *file2 = fopen("ranking.txt", "w");

    char linha[100];
    int i = 0;

    if (file2 == NULL) {
        perror("Erro ao abrir arquivo.");
        return 1;
    }

    fprintf(file2 ,"| Ranking |  Pontuação |    Tempo   |  Dificuldade | Nome Completo" ); //cabeçalho
    printf("| Ranking |  Pontuação |    Tempo   |  Dificuldade | Nome Completo" ); //cabeçalho

    while (i < cont) {

        printf("\n\n     %d         %d         %02d:%02d          %d         %s" ,
        ranking[i],
        users[i]->pontuação,
        minutos[i],
        segundos[i],
        users[i]->dificuldade,
        users[i]->nome);

        fprintf(file2,"\n\n     %d         %d         %02d:%02d          %d         %s" ,
        ranking[i],
        users[i]->pontuação,
        minutos[i],
        segundos[i],
        users[i]->dificuldade,
        users[i]->nome);

        i++;
    } 

    fclose(file2);

}

int main(){

    int tempo;
    int quantPlayers = 0;
    int minutos[numMaxPlayers] = {0};
    int segundos[numMaxPlayers] = {0};
    int moreThanOne = 0;
    int ranking[numMaxPlayers] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

    Infos *user;
    user = (Infos *)malloc(sizeof(Infos));

    Infos *users[numMaxPlayers] = {0};
    menu();
    clean();
    player(user); 
    time_t inicio = time(NULL); 
    game(user);
    time_t fim = time(NULL);
    tempo = fim-inicio;
    formatTime(&tempo,segundos,minutos);
    points(user,minutos);

    /*************************************ADIÇÃO DO JOGADOR ATUAL***********************************/

    addPlayer(ranking,minutos,segundos,user); 

    /*********************LEITURA DO ARQUIVO PARA STRUCT LOCAL E ORDENAÇÃO**************************/

    clean();
    sleep(3);
    
    quantPlayers = orderRanking(minutos,segundos,users); 

    /****************************************ATUALIZAR ARQUIVO**************************************/

    if (quantPlayers > 1) {
        rewritingRanking(ranking,minutos,segundos,users,quantPlayers);
        }

    /***********************************************************************************************/

    for (int i = 0; i < numMaxPlayers; i++) {
        if (users[i] != NULL) {
            free(users[i]);
        }
    }

    free(user);

    return 0;

}