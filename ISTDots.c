#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX(a,b)    (((a)>(b))?(a):(b))
#define M_PI 3.14159265
#define STRING_SIZE 100       // max size for some strings
#define TABLE_SIZE 850        // main game space size
#define LEFT_BAR_SIZE 150     // left white bar size
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y
#define SQUARE_SEPARATOR 8    // square separator in px
#define BOARD_SIZE_PER 0.7f   // board size in % wrt to table size
#define MAX_BOARD_POS 15      // maximum size of the board
#define MAX_COLORS 5
#define MARGIN 5

#define MAXNAME 9 // tamanho máximo do nome
#define MINSIZE 5 //tamanho minimo da medida de comprimento ou largura
#define MAXSIZE 15 // tamanho maximo da medida de comprimento ou largura maximo
#define MAXNUMBERPONTOS 99 // define o número máximo de pontos que podem existir
#define MAXIMUMMOVE 225

// declaration of the functions related to graphical issues
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** ,TTF_Font **,TTF_Font **,TTF_Font **);
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
int RenderTable(int, int, int [], TTF_Font *, SDL_Surface **, SDL_Renderer *, TTF_Font *);
void ProcessMouseEvent(int , int , int [], int , int *, int * );
void RenderPoints(int [][MAX_BOARD_POS], int, int, int [], int, SDL_Renderer *, int, int, int, int [][MAXIMUMMOVE]);
void RenderStats( SDL_Renderer *, TTF_Font *, int [], int , int );
void filledCircleRGBA(SDL_Renderer * , int , int , int , int , int , int );

void FuncaoDeLeitura(int *numerocores, int numeropontos[], int *numerojogadas );
void Leituradosparametrosdotabuleiro(int *flargtamanho, int *fcomptamanho);
void Leituradonome(char nome[]);
void gerarcores(int *numerodecores,int *fcomprimento, int *flargura, int boardtable[][MAXSIZE], int vetorintersecao[3], int cordoquadrado);
void limpezadotabuleiro(int *fcomprimento, int *flargura, int boardtable[][MAXSIZE]);
int  validarsequencia( int corinicial, int coordenadax, int coordenaday, int boardtable[][MAXSIZE]);
void searchvalues( int boardtable[][MAXSIZE], int valuetable[][MAXSIZE]);
void colocarpontosnovos(int boardtable[][MAXSIZE], int *fcomprimento, int *flargura);
int retroceder(int boardtable[MAXIMUMMOVE][MAXIMUMMOVE], int *_posicao);
int shuffle(int boardtable[][MAXSIZE], int *fcomprimento, int *flargura);
void shufflemove( int boardtable[][MAXSIZE], int fvetorsoma[100], int numerodecores, int fcomprimento, int flargura);
void validarquadrado( int positiontable[][MAXIMUMMOVE], int fpt_x, int fpt_y, int dimensao, int vetordeintersecao[3]);
void processarquadrado(int cordoquadrado, int boardtable[][MAXSIZE],int erraseboard[][MAXSIZE], int fcomprimento,int flargura);
void convertecoordenadas(int positiontable[2][MAXIMUMMOVE], int controlador, int boardtable[][MAXSIZE]);
void eliminarospontosdentro(int boardtable[][MAXSIZE], int fcomprimento, int flargura);
void contarpontosfinais(int boardtable[][MAXSIZE], int fvetorsomafinal[100], int fcomprimento, int flargura, int fnumerodecores);
void decrementodejogadas( int vecsomainicial[100], int vecsomafinal[100],int vecsomatotal[100], int vetorjogadas[100], int fnumerodecores);
void contarpontosiniciais(int boardtable[][MAXSIZE], int fvetorsomainicial[100], int fcomprimento, int flargura, int fnumerodecores);
int verificarobjetivos( int vetorjogadas[100], int numerodecores);
void apagarvaloresanteriores( int vetorsomainicial[100], int vetorsomafinal[100], int vetorsomashuffle[100]);
void undo(int boardtable[][MAXSIZE], int fcomprimento, int flargura, int saveboard[][MAXSIZE]);
void SaveJogada( int boardtable[][MAXSIZE], int fcomprimento, int flargura, int saveboard[][MAXSIZE]);
void Ficheiros(char nome[STRING_SIZE], int matrixstats[2][MAXIMUMMOVE],int numerodejogos);
void PontosParaNewGame( int vetorauxiliar[STRING_SIZE], int numerodecores, int vetorpontosiniciais[STRING_SIZE], int *guardajogadainicial,int numerodejogadas);
void guardarvaloresanteriores(int numerodejogadas, int *jogadasanteriores, int numerodecores, int vetorpontosanteriores[STRING_SIZE], int vetorauxiliar[STRING_SIZE]);
void guardaestatisticasdojogo(int numerodejogadas,int matrixestatisticas[2][MAXIMUMMOVE], int resultado,int numerodejogos, int numerodejogasiniciais);
void contarpontosshuffle(int boardtable[][MAXSIZE], int fvetorsomashuffle[100], int fcomprimento, int flargura, int fnumerodecores);
void instrucoes( SDL_Renderer *_renderer, TTF_Font *_font);
void RenderWin(SDL_Renderer *_renderer, TTF_Font *_font);
void RenderLOSE(SDL_Renderer *_renderer, TTF_Font *_font);
void RenderSHUFFLE(SDL_Renderer *_renderer, TTF_Font *_font);
void apagarmatrixvalidacao(int boardtable[2][MAXIMUMMOVE], int limite);
void retornapontosiniciais(int _numerodecores,int _numerodepontos[STRING_SIZE], int _vetortransf[STRING_SIZE]);
void retornapontosanteriores(int _numerodecores, int _numerodepontos[STRING_SIZE], int _vetorpontosanteriores[STRING_SIZE]);


// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "Rafael Cordeiro";
const char myNumber[] = "IST90171";
const int colors[3][6] = {{30, 253, 54, 105, 159,0},{147, 184, 61, 165, 2,0},{45, 19, 105, 131, 81,0}};
const int black=5;

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( void )
{
    int checkrato=0,largura=0,comprimento=0,numerodecores=0,numerodejogadas=0;
    int pos=0;
    int pressionarato=0;
    char nome[STRING_SIZE]={0};
    int numerodepontos[STRING_SIZE]={0};
    int xmov1,ymov1=0;
    int verificacao=0;
    int vetorsomainicial[STRING_SIZE]={0};
    int vetorsomafinal[STRING_SIZE]={0};
    int vetorsomatotal[STRING_SIZE]={0};
    int vetortransf[STRING_SIZE]={0};
    int vetorpontosanteriores[STRING_SIZE]={0};
    int jogadaanterior=0;
    int numerodejogos=0,vitoria=2;
    int numerojogadasinicial=0;
    int matrixstats[2][MAXIMUMMOVE]={{0}};
    int vetorsomashuffle[STRING_SIZE]={0};
    srand(1234);//gera a semente para a geração dos números aleatórios
    // esta função vai obter os parâmetros escolhidos pelo utilizador, para as definições e especificações do jogo
    Leituradosparametrosdotabuleiro(&largura,&comprimento);//pede ao utilizador a largura e o comprimento
    Leituradonome(nome);//pede ao utilizador o nome
    FuncaoDeLeitura( &numerodecores,numerodepontos,&numerodejogadas);//pede ao utilizador o numero de cores, o numero de pontos e o numero de jogadas
    PontosParaNewGame(vetortransf,numerodecores,numerodepontos, &numerojogadasinicial,numerodejogadas);// guarda todos os valores iniciais do tabuleiro e objetivos
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    TTF_Font *sans = NULL;
    TTF_Font *inst = NULL;
    TTF_Font *resultado = NULL;
    SDL_Surface *imgs[4];
    SDL_Event event;
    int jogadaundo=1;
    int quit = 0;
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    int square_size_px = 0, board_size_px[2] = {0};
    int board_pos_x = 0, board_pos_y = 0;
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};
    int pt_x = 0, pt_y = 0;
    int matrixvalidacao[2][MAXIMUMMOVE]={{-1}};
    int vetorintersecao[3]={0};
    board_pos_x = largura;
    board_pos_y = comprimento;

    limpezadotabuleiro(&comprimento, &largura,board);// coloca todos os pontos do tabuleiro a preto
    int newgame=0;
    int shufflevar=0;
    int errasematrix[MAXSIZE][MAXSIZE]={{0}};
    int savematrix[MAXSIZE][MAXSIZE]={{0}};
    // initialize graphics
    InitEverything(width, height, &serif, imgs, &window, &renderer,&sans,&inst,&resultado);

    while( quit == 0 )
    {
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                 quit=1;
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_n:
                        limpezadotabuleiro(&comprimento,&largura, board);
                        gerarcores(&numerodecores,&comprimento, &largura,board,vetorintersecao,pressionarato);
                        retornapontosiniciais(numerodecores,numerodepontos,vetortransf);//atualiza os objetivos
                        jogadaundo=0;
                        if(numerodejogos>0 && vitoria!=1 && newgame==1){//guardar como derrota se o utilizador pressionar a tecla n a meio do jogo
							numerodejogos++;
							vitoria=0;
							guardaestatisticasdojogo(numerodejogadas, matrixstats,vitoria,numerodejogos,numerojogadasinicial);
                        }
                        numerodejogadas=numerojogadasinicial;
                        vitoria=2;
                        newgame=1;  //atualiza a variável newgame para permitir o processamento dos movimentos do rato
                        break;
                    case SDLK_q:
						numerodejogos++;
                        vitoria=0;
                        guardaestatisticasdojogo(numerodejogadas, matrixstats,vitoria,numerodejogos,numerojogadasinicial);
                        quit=1;
                        break;
                    case SDLK_u:
                        if(jogadaundo==1){//só posso usar a tecla undo no mesmo jogo,isto é, se eu ganhar e pressionar undo, não posso voltar ao jogo anterior
                            undo(board,comprimento,largura,savematrix);
                            retornapontosanteriores(numerodecores,numerodepontos,vetorpontosanteriores);
                            numerodejogadas=jogadaanterior;
                        }
                        break;
                    default:
                        break;
                }
            }
            else if ( event.type == SDL_MOUSEBUTTONDOWN  && newgame==1)
            {
                ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
                //printf("Button down: %d %d\n", pt_x, pt_y);

                pressionarato=board[pt_x][pt_y];//guarda a cor do ponto pressionado
                checkrato=1;
                xmov1=pt_x;
                ymov1=pt_y;
                matrixvalidacao[0][0]=pt_x;//guarda a posição do ponto inicial
                matrixvalidacao[1][0]=pt_y;
                pos=1;
                jogadaundo=1;}
            else if ( event.type == SDL_MOUSEBUTTONUP && newgame==1)
            {
                checkrato=0;
                ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
                //printf("Button up: %d %d\n", pt_x, pt_y);
                 SaveJogada(board,comprimento,largura,savematrix);//guarda o tabuleiro da jogada anterior
                 guardarvaloresanteriores(numerodejogadas,&jogadaanterior,numerodecores,numerodepontos,vetorpontosanteriores);//guarda os objetivos da jogada anterior
                 contarpontosiniciais(board, vetorsomainicial,comprimento, largura,numerodecores);//conta o numero de pontos para cada cor antes de ser efetuada qualquer alteração
                if(vetorintersecao[0]==1)//caso exista validação do quadrado
                {   searchvalues(board,errasematrix);//vai colocar os pontos da sequencia a preto
                    eliminarospontosdentro(board,comprimento,largura);// vai colocar os pontos dentro do quadrado a preto
                    processarquadrado(pressionarato,board,errasematrix,comprimento,largura);// vai colocar a preto todos os pontos da cor do quadrado
                }


                convertecoordenadas(matrixvalidacao,pos,errasematrix);
                if(pos==1){// caso exista apenas uma bola pressionada, a matrix de eliminação não vai apagar esse ponto
                    errasematrix[matrixvalidacao[0][0]][matrixvalidacao[1][0]]=0;
                            }
                searchvalues(board,errasematrix);//coloca as posições sequenciadas a preto
                contarpontosfinais( board,vetorsomafinal,comprimento,largura,numerodecores);//conta o numero de pontos para cada cor depois de ser colocadas todas as casas necesárias a preto
                decrementodejogadas(vetorsomainicial,vetorsomafinal,vetorsomatotal,numerodepontos,numerodecores);//atualiza os objetivos
                colocarpontosnovos(board,&comprimento,&largura);//cria o efeito em cascata dos pontos,para as peças sequenciadas irem para o topo do tabuleiro
                gerarcores(&numerodecores,&comprimento,&largura,board,vetorintersecao,pressionarato);//se detetar pontos a preto, coloca uma cor aleatória
                if(shuffle(board,&comprimento,&largura)==1){//caso não existam mais jogadas possiveis
                    shufflemove(board,vetorsomashuffle,numerodecores,comprimento,largura);//com os meus numero de pontos para cada cor,vai baralhar todos os pontos de forma aleatória
                    shufflevar=1;//ativa a variável de shuffle, que vai ser utilizada para a imagem
                    }
                if(pos>1){// caso a sequencia tenha mais que um ponto,o
                    numerodejogadas--;
                    if(verificarobjetivos(numerodepontos,numerodecores)==1){//ganhou
						vitoria=1;
						guardaestatisticasdojogo(numerodejogadas, matrixstats,vitoria,numerodejogos,numerojogadasinicial);//guarda numa matriz as estitisticas dos jogo
						numerodejogos++;
						newgame=0;//coloco a variável newgame a 0 para o programa esperar até o jogador pressionar n ou q
						jogadaundo=0;
                    }
                    if(numerodejogadas==0 && vitoria!=1){
                        vitoria=0;//perdeu
                        guardaestatisticasdojogo(numerodejogadas, matrixstats,vitoria,numerodejogos,numerojogadasinicial);
                        numerodejogos++;
                        newgame=0;
                        jogadaundo=0;}
                }
               apagarvaloresanteriores(vetorsomainicial,vetorsomafinal,vetorsomashuffle);// coloca de novo a 0 todos os vetores utilizados para processar o shuffle e atualizar os objetivos
               apagarmatrixvalidacao(matrixvalidacao, pos);
               vetorintersecao[0]=0;


            }
            else if ( event.type == SDL_MOUSEMOTION && newgame==1)
            {   ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
                //printf("Moving Mouse: %d %d\n", pt_x, pt_y);
                if(checkrato==1){//caso tenha feito buttondown
                    if(validarsequencia(pressionarato,pt_x,pt_y,board)==1){//e se a seuqencia feita for válida(da mesma cor)
                        if((((pt_x-xmov1)==1) &&((pt_y-ymov1)==0)) || (((pt_x-xmov1)==-1) && ((pt_y-ymov1)==0)) || (((pt_x-xmov1)==0) && ((pt_y-ymov1)==1)) || (((pt_x-xmov1)==0) && ((pt_y-ymov1)==-1))){//caso a seuqencia feita seja entre pontos interligados
                            errasematrix[xmov1][ymov1] = 1;//em ambos os pontos, a matriz de eliminação vai estar a 1
                            errasematrix[pt_x][pt_y] = 1;
                            matrixvalidacao[0][pos]=pt_x;
                            matrixvalidacao[1][pos]=pt_y;
                            xmov1=pt_x;//contém os ponto processado anteriormente pelo MOUSEMOTION
                            ymov1=pt_y;
                            verificacao=retroceder(matrixvalidacao,&pos);
                            if(verificacao==1){//caso ele não tenha retrocedido
                                validarquadrado(matrixvalidacao,pt_x,pt_y,pos,vetorintersecao);
                                pos++;}
                            else if(verificacao==0){//caso ele tenha retrocedido
                                errasematrix[matrixvalidacao[0][pos-1]][matrixvalidacao[1][pos-1]]=0;//coloca a posição anterior ao mousse motion(do ponto que se retrocedeu) a 0 na matriz de eliminação
                                pos--;//diminuição do número de pontos da sequencia
                                vetorintersecao[0]=0;}//caso ele tenha feito um quadrado e haja retrocesso, a validação do quadrado passa a ser 0
                                                                                                                                                                                                            }
                                                                            }
        }
        }}
        // render game table
        square_size_px = RenderTable( board_pos_x, board_pos_y, board_size_px, serif, imgs, renderer, sans);
        // render board
        // faz render também do efeito de diminuir o efeito das bolas durante uma jogada
        RenderPoints(board, board_pos_x, board_pos_y, board_size_px, square_size_px, renderer,xmov1,ymov1,pos,matrixvalidacao);

        if(vitoria==1){//faz render de uma imagem para a vitória
            RenderWin(renderer,resultado);
        }
        if(vitoria==0){//faz render de uma imagem para a derrota
            RenderLOSE(renderer,resultado);

        }
        if(shufflevar==1){//faz render de uma imagem para o shuffle
            RenderSHUFFLE(renderer,resultado);
            SDL_Delay(220);

        }

        RenderStats(renderer, serif, numerodepontos, numerodecores,numerodejogadas);//atualizada a parte gráfica dos objetivos e jogadas
        instrucoes(renderer,inst);//faz render de uma retangulo com as intruções do jogo
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay
        if(shufflevar==1){// caso seja feito o shuffle, vai ser acionado um delay para manter a imagem de shuffle mais tempo
            SDL_Delay(2200);
            shufflevar=0;
        }
        SDL_Delay(120);
    }
    Ficheiros(nome, matrixstats,numerodejogos);//guarda num ficheiro as estatisticas do jogo
    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(serif);
    TTF_CloseFont(resultado);
    TTF_CloseFont(sans);
    TTF_CloseFont(inst);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

/**
 * ProcessMouseEvent: gets the square pos based on the click positions !
 * \param _mouse_pos_x position of the click on pixel coordinates
 * \param _mouse_pos_y position of the click on pixel coordinates
 * \param _board_size_px size of the board !
 * \param _square_size_px size of each square
 * \param _pt_x square nr
 * \param _pt_y square nr
 */
void ProcessMouseEvent(int _mouse_pos_x, int _mouse_pos_y, int _board_size_px[], int _square_size_px,
        int *_pt_x, int *_pt_y )
{
    // corner of the board
    int x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    int y_corner = (TABLE_SIZE - _board_size_px[1] - 15);

    // verify if valid cordinates
    if (_mouse_pos_x < x_corner || _mouse_pos_y < y_corner || _mouse_pos_x > (x_corner + _board_size_px[0])
        || _mouse_pos_y > (y_corner + _board_size_px[1]) )
    {
        *_pt_x = -1;
        *_pt_y = -1;
        return;
    }

    // computes the square where the mouse position is
    _mouse_pos_x = _mouse_pos_x - x_corner;
    _mouse_pos_y = _mouse_pos_y - y_corner;

    *_pt_x = _mouse_pos_x / (_square_size_px + SQUARE_SEPARATOR);
    *_pt_y = _mouse_pos_y / (_square_size_px + SQUARE_SEPARATOR);
}

/**
 * RenderPoints: renders the board
 * \param _board 2D array with integers representing board colors
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _square_size_px size of each square
 * \param _board_size_px size of the board
 * \param _renderer renderer to handle all rendering in a window
 */
 //UTILIZAÇÃO:Serve para render os pontos do tabuleiro,esta função foi alterada para melhorar a dinâmica do jogo quando se realiza uma jogada
 //QUANDO É UTILIZADA:No final do loop
//param _board 2D array with integers representing board colors
//param _board_pos_x number of positions in the board (x axis)
//param _board_pos_y number of positions in the board (y axis)
//param _square_size_px size of each square
//param _board_size_px size of the board
//param _renderer renderer to handle all rendering in a window
//param  fpt_x e y posição do ponto anterior ao que está a ser sequenciado
//param  fpos numero de pontos
//param  matrix matriz que contém todos os pontos da sequencia
void RenderPoints(int _board[][MAX_BOARD_POS], int _board_pos_x, int _board_pos_y,
        int _board_size_px[], int _square_size_px, SDL_Renderer *_renderer,int fpt_x,int fpt_y, int fpos,int matrix[2][MAXIMUMMOVE])
{
    int clr, x_corner, y_corner, circleX, circleY, circleR;
    float salto=((rand()%100)/500.0f)+0.25f;//gera um conjuntos de número aleatórios para permitir o efeito de "piscar"
    // corner of the board
    x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    y_corner = (TABLE_SIZE - _board_size_px[1] - 15);

    // renders the squares where the dots will appear
    for ( int i = 0; i < _board_pos_x; i++ )
    {
        for ( int j = 0; j < _board_pos_y; j++ )
        {
        if(i==fpt_x && j==fpt_y){//caso seja igual à posição que o utilizador está a apontar com o rato, a bola vai piscando
                circleR=(int)(_square_size_px*salto);


        }else{
            circleR=(int)(_square_size_px*0.40f);// caso não seja igual, a bola e criada normalmente

        }
        for(int k=0;k<fpos-1;k++){// para as posições da sequencia feita, as bolas dessa sequencia encolhem
            if(matrix[0][k]==i && matrix[1][k]==j){
                circleR=(int)(_square_size_px*0.20f);

            }


        }

                // define the size and copy the image to display
                circleX = x_corner + (i+1)*SQUARE_SEPARATOR + i*(_square_size_px)+(_square_size_px>>1);
                circleY = y_corner + (j+1)*SQUARE_SEPARATOR + j*(_square_size_px)+(_square_size_px>>1);
                // draw a circle
                clr = _board[i][j];
                filledCircleRGBA(_renderer, circleX, circleY, circleR, colors[0][clr], colors[1][clr], colors[2][clr]);
        }
    }
}

/**
 * filledCircleRGBA: renders a filled circle
 * \param _circleX x pos
 * \param _circleY y pos
 * \param _circleR radius
 * \param _r red
 * \param _g gree
 * \param _b blue
 */
void filledCircleRGBA(SDL_Renderer * _renderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b)
{
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    float step = M_PI / (_circleR*8);

    SDL_SetRenderDrawColor(_renderer, _r, _g, _b, 255);

    while (_circleR > 0)
    {
        for (degree = 0.0; degree < M_PI/2; degree+=step)
        {
            off_x = (int)(_circleR * cos(degree));
            off_y = (int)(_circleR * sin(degree));
            SDL_RenderDrawPoint(_renderer, _circleX+off_x, _circleY+off_y);
            SDL_RenderDrawPoint(_renderer, _circleX-off_y, _circleY+off_x);
            SDL_RenderDrawPoint(_renderer, _circleX-off_x, _circleY-off_y);
            SDL_RenderDrawPoint(_renderer, _circleX+off_y, _circleY-off_x);
        }
        _circleR--;
    }
}

/*
 * Shows some information about the game:
 * - Goals of the game
 * - Number of moves
 * \param _renderer renderer to handle all rendering in a window
 * \param _font font to display the scores
 * \param _goals goals of the game
 * \param _ncolors number of colors
 * \param _moves number of moves remaining
 */
 //UTILIZAÇÃO: Utilizada para aparecer no tabuleiro os objetivos e as jogadas em pequenos retangulos
 //QUANDO É UTILIZADA:No final do loop
 //param renderer renderer to handle all rendering in a window
 //param font font to display the scores
 //param ncolors number of colors
 //param goals goals of the game
 //param moves number os moves
void RenderStats( SDL_Renderer *_renderer, TTF_Font *_font, int _goals[], int _ncolors, int _moves)
{   int salto=150;
    char objetivos[STRING_SIZE];
    char cores[STRING_SIZE];
    SDL_SetRenderDrawColor(_renderer,242,225,212,255);
    SDL_Color black = { 0, 0, 0 };


    sprintf(objetivos,"%d",_moves);

    SDL_Rect reta ={65,65,100,60};
    SDL_RenderFillRect( _renderer, &reta);
    RenderText(110,68, objetivos,_font, &black,_renderer);

    for(int i=0;i<_ncolors;i++){
        SDL_SetRenderDrawColor(_renderer,242,225,212,255);
        sprintf(cores,"%d",_goals[i]);
        SDL_Rect retang={salto+65,65,100,60};
        SDL_RenderFillRect(_renderer,&retang);
        filledCircleRGBA(_renderer,salto+86,90,14,colors[0][i],colors[1][i],colors[2][i]);
        RenderText(salto+110,68,cores,_font,&black,_renderer);
        salto+=120;
    }




}

/*
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _board_size_px size of the board
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderTable( int _board_pos_x, int _board_pos_y, int _board_size_px[],
        TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer,TTF_Font *_fontnome)
{
    SDL_Color black = { 0, 0, 0 }; // nome
    SDL_Color light = { 247, 225, 212};
    SDL_Color dark = { 186, 89, 33};
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, board, board_square;
    int height, board_size, square_size_px, max_pos;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE;
    tableDest.h = TABLE_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName, _fontnome, &black, _renderer);

    // this renders the student number
    RenderText(TABLE_SIZE+3*MARGIN, height, myNumber, _fontnome, &black, _renderer);

    // compute and adjust the size of the table and squares
    max_pos = MAX(_board_pos_x, _board_pos_y);
    board_size = (int)(BOARD_SIZE_PER*TABLE_SIZE);
    square_size_px = (board_size - (max_pos+1)*SQUARE_SEPARATOR) / max_pos;
    _board_size_px[0] = _board_pos_x*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;
    _board_size_px[1] = _board_pos_y*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;

    // renders the entire board background
    SDL_SetRenderDrawColor(_renderer, dark.r, dark.g, dark.b, dark.a );
    board.x = (TABLE_SIZE - _board_size_px[0]) >> 1;
    board.y = (TABLE_SIZE - _board_size_px[1] - 15);
    board.w = _board_size_px[0];
    board.h = _board_size_px[1];
    SDL_RenderFillRect(_renderer, &board);

    // renders the squares where the numbers will appear
    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a );

    // iterate over all squares
    for ( int i = 0; i < _board_pos_x; i++ )
    {
        for ( int j = 0; j < _board_pos_y; j++ )
        {
            board_square.x = board.x + (i+1)*SQUARE_SEPARATOR + i*square_size_px;
            board_square.y = board.y + (j+1)*SQUARE_SEPARATOR + j*square_size_px;
            board_square.w = square_size_px;
            board_square.h = square_size_px;
            SDL_RenderFillRect(_renderer, &board_square);
        }
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
    // return for later use
    return square_size_px;
}

/**
 * RenderLogo function: Renders the IST logo on the app window
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer,TTF_Font **_fontnome,TTF_Font **_fontinst, TTF_Font **_fontresultado)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 30);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    //fontes criadas
    *_fontnome = TTF_OpenFont("OpenSans.ttf", 18);
    if(!*_fontnome)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
      *_fontinst = TTF_OpenFont("OpenSans.ttf", 14);
    if(!*_fontnome)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
      *_fontresultado = TTF_OpenFont("OpenSans.ttf", 100);
    if(!*_fontresultado)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "ISTDOTS-ALUNO:RAFAEL CORDEIRO ist90171", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}
 // UTILIZAÇÃO:Esta função vai identificar  o numero de pontos e numero de jogadas bem com o numero de pontos
 // QUANDO É UTILIZADA: Esta função é utilizada sempre no inicio do jogo
void FuncaoDeLeitura( int *numerocores, int numeropontos[], int *numerojogadas){
    char string[STRING_SIZE]={0}; //string onde são introduzidos os três parâmetros
    int b=0,i=0;  // variável que  confirma que todas os parâmetros foram introduzidos
    //Recolhe cada uma das variáveis e só sai de cada um dos ciclos se a variável estiver bem
    //Para cada uma das variáveis, ele vai colocá-las numa string(fgets) e, de seguida, vai transferi-las para a respetiva variável, se o valor estiver correto
         do{

            if( *numerocores>MINSIZE){
                printf("Introduziu incorretamente os valores, por favor coloque de novo:\n");
                fgets(string,STRING_SIZE,stdin);
                sscanf(string,"%d",numerocores);
       }
            if(*numerocores==0){
                printf("Introduza o numero de cores do tabuleiro(1 a 5):\n");
                fgets (string,STRING_SIZE,stdin);
                sscanf(string,"%d",numerocores);
       }
       }while(*numerocores<1 || *numerocores>MINSIZE);

       do{

            if( *numerojogadas>99){
                printf("Introduziu incorretamente os valores, por favor coloque de novo:\n");
                fgets(string,STRING_SIZE,stdin);
                sscanf(string,"%d",numerojogadas);
       }
            if(*numerojogadas==0){
                printf("Introduza o numero de jogadas do tabuleiro(1 a 99):\n");
                fgets (string,STRING_SIZE,stdin);
                sscanf(string,"%d",numerojogadas);
       }
       }while(*numerojogadas<1 || *numerojogadas>99);
//para cada cor que o utilizador colocou, o programa vai pedir um certo numero de pontos
    for(i=0;i<*numerocores;i++){
        printf("Coloque o número que pontos para as %d que definiu, um a seguir ao outro:\n",*numerocores);
        fgets(string,STRING_SIZE,stdin);
        sscanf(string,"%d",&numeropontos[i]);
        for(b=0;numeropontos[i]>MAXNUMBERPONTOS|| numeropontos[i]==0;b++){
            printf("O número que digitou é incorreto, por favor digite um número entre 1 e 99:\n");
            fgets(string,STRING_SIZE,stdin);
            sscanf(string,"%d",&numeropontos[i]);}
             }

             }

//UTILIZAÇÃO:Esta função vai identificar os parâmetros, como as medidas do tabuleiro
 // QUANDO É UTILIZADA: Esta função é utilizada sempre no inicio do jogo
 //param fcomptamanho define o númerode linhas que o tabuleiro tem
 //param flargtamanho define o numero de coluna
void Leituradosparametrosdotabuleiro(int *flargtamanho, int *fcomptamanho){
  char string[STRING_SIZE]={0};
  do{

            if((*flargtamanho<MINSIZE && *flargtamanho>0) || *flargtamanho>MAXSIZE){
                printf("Introduziu incorretamente os valores de largura, por favor coloque de novo:\n");
                fgets(string,STRING_SIZE,stdin);
                sscanf(string,"%d",flargtamanho);
       }
            if(*flargtamanho==0){
                printf("Introduza a largura do tabuleiro(5 a 15):\n");
                fgets (string,STRING_SIZE,stdin);
                sscanf(string,"%d",flargtamanho);
       }
       }while(*flargtamanho<MINSIZE || *flargtamanho>MAXSIZE);

        do{

            if((*fcomptamanho<MINSIZE && *fcomptamanho>0) || *fcomptamanho>MAXSIZE){
                printf("Introduziu incorretamente os valores, por favor coloque de novo:\n");
                fgets(string,STRING_SIZE,stdin);
                sscanf(string,"%d",fcomptamanho);
       }
            if(*fcomptamanho==0){
                printf("Introduza o comprimento do tabuleiro(5 a 15):\n");
                fgets (string,STRING_SIZE,stdin);
                sscanf(string,"%d",fcomptamanho);
       }


       }while(*fcomptamanho<MINSIZE || *fcomptamanho>MAXSIZE);
}
//UTILIZAÇÃO:Esta função vai identificar o nome do utilizador
 // QUANDO É UTILIZADA: Esta função é utilizada sempre no inicio do jogo
 //param nome string que guarda o nome do utilizador
void Leituradonome(char nome[]){
    char string[STRING_SIZE]={0};
    do{

            if(strlen(nome)>8){
                printf("Introduziu incorretamente os valores, por favor coloque de novo\n");
                fgets(string,STRING_SIZE,stdin);
                sscanf(string,"%s",nome);
       }
            if(strlen(nome)==0){
                printf("Introduza o nome:\n");
                fgets (string,STRING_SIZE,stdin);
                sscanf(string,"%s",nome);
       }
    }while(strlen(nome)>8|| strlen(nome)==0);

}

//UTILIZAÇÃO: Esta função vai gerar cores aleatórias para as posições dos pontos iniciais
// A função vem em complemento da funcao limpezadotabuleiro, pois a primeiro vai colocar todos as peças a preto,enquanto a funcao gerar cores,
//vai detetar as posições que estão a preto e vai colocá-las com um cor gerada aleatoriamente, do vetor colors defenido no inicio da programa
//QUANDO É UTILIZADA: É utilizada no inicio do jogo, juntamente com a funcao limpeza do tabuleiro, para gerar novas cores em cada jogada(em complemento com
//a função colocarpontosnovos
//param numerodecores numero de cores colocadas pelo utilizador
//param fcomprimento comprimento do tabuleiro
//param flargura largura do tabuleiro
//param boardtable matriz do tabuleiro
void gerarcores(int *numerodecores,int *fcomprimento, int *flargura,int boardtable[][MAXSIZE], int vetorintersecao[3],int cordoquadrado){
    int i,j, random=0;

    for(i=0;i<*fcomprimento;i++)
    {// gero duas funções for para percorrer a matriz na íntegra,que vai sucessivamente fixar uma linha e percorrer todas as colunas;
        for(j=0;j<*flargura;j++)
        {
            if(boardtable[j][i]==black)
            {// neste if estabelecemos que a função só atribuí uma cor se a bola nesse ponto estiver a preto
                if(vetorintersecao[0]==1)
                {
                    do{
                        random=((int)(rand() % *numerodecores));// gera uma cor diferente do quadrado,se o utilizador o fizer
                        boardtable[j][i]=random;

                    }while(random==cordoquadrado);

                }else{
                    random=((int)(rand() % *numerodecores));// gera uma cor entre 0 e o número de cores atribuído pelo utilizador
                    boardtable[j][i]=random;
                    }

            }
         }
     }
}




//UTILIZAÇÃO:Ao iniciar o jogo, esta função vai colocar todos os valores a preto, para depois ser atribuída, uma cor qualquer.
//QUANDO É UTILIZADA:É utilizada tanto no início do jogo, como para a funcionalidade de gerar um novo jogo
//param fcomprimento comprimento do tabuleiro
//param flargura largura do tabuleiro
//param boardtable matriz do tabuleiro
void limpezadotabuleiro(int *fcomprimento, int *flargura, int boardtable[][MAXSIZE]){
int i,j=0;
    for(i=0;i<*fcomprimento;i++){// percorre todas as linhas e colunas e coloca-as com a cor preto
        for(j=0;j<*flargura;j++){
                boardtable[j][i]=black;
                                }

                            }
}
//UTILIZAÇÃO: A função validar a sequência serve para determinar se a sequência de pontos feita pelo utilizador é válida
//QUANDO É UTILIZADA: É utilizada em cada movimento do rato, para conferir se as corer das bolas estão corretas
//param corinicial representa a cor da sequencia
//param coordenadax e y, posição da bola
//param boardtable matriz do tabuleiro
int  validarsequencia( int corinicial, int coordenadax, int coordenaday, int boardtable[][MAXSIZE]){

    int valordaposicao=boardtable[coordenadax][coordenaday];//coloca numa variável o valor da nova posição do rato
    if(corinicial== valordaposicao){// se o seu conteúdo(cor) for identico à cor da posição inicial, então ele retorna 1
        return 1;
    }else{
        return 0;
    }

}
//UTILIZAÇÃO: A função vai procurar as posições que foram sequenciadas e colocá-las a preto para depois utilizar as funções
// colocarpontosnovos, que permite a colocar os pontos a preto, encontrados na função searchvalues,no topo do tabuleiro
//para depois gerarnovascores
//QUANDO É UTILIZADA:É utilizada para cada sequência de pontos válida
//param boardtable matriz do tabuleiro
//param valuetable matriz com 0 e 1,cujas posições a 1 correspondem a pontos a eliminar
void searchvalues( int boardtable[][MAXSIZE], int valuetable[][MAXSIZE]){
    int i,j=0;
    for(i=0;i<MAXSIZE;i++){
        for(j=0;j<MAXSIZE;j++){//percorre todos os pontos  da matriz criada com as posições da sequência
            if(valuetable[j][i]>0){// caso ela encontre um ponto da matriz >0(correspondem aos pontos da sequência)
                boardtable[j][i]=black;// a função vai colocar a matriz original, no mesmo ponto, a preto
                valuetable[j][i]=0;// de seguido como forma de não guardar esses pontos para futuras jogadas, coloca-mos de novo a matriz a 0
            }

        }

    }

}
//UTILIZAÇÃO:Esta função serve para identificar os pontos do tabuleiro que estão a preto(pontos da minha sequência
// e coloca-os no topo do tabuleiro, proporcionando um efeito em cascata, onde os pontos da mesma coluna,vão descendo
//sucessivamente  e o ponto preto da sequência vai subindo até ao topo
//QUANDO É UTILIZADA:A função é utilizada para processsar cada sequência válida de pontos
//param boardtable matriz do tabuleiro
//param fcomprimento comprimento do tabuleiro
//param flargura largura do tabuleiro
void colocarpontosnovos(int boardtable[][MAXSIZE], int *fcomprimento, int *flargura){

    int i,k,j=0;

     for(i=0;i<*fcomprimento;i++){
        for(j=0;j<*flargura;j++){
          if(boardtable[j][i]==black){
            for(k=i;k>0;k--){
                boardtable[j][k]=boardtable[j][k-1]; // ele vai trocando sucessivamente, os valores da mesma coluna até chegar à linha 0 saindo do ciclo
                            }
            boardtable[j][0]=black;// depois de chegar à linha 0, o valor que está nessa coluna vai ficar a preto
                                     }

                                }

                            }
}
//UTILIZAÇÃO:Como forma de tornar o meu programa idêntico ao jogo original, tive de resolver o problema de retroceder uma sequência sem ele apagar todos os pontos que eu passei
//logo, criei uma função que comparava consecutivamente o intervalo de dois pontos(o ponto de "engano" e o ponto que o rato fica),assim se houver duas coordenadas
//idênticas ele so vai apagar até à coordenada que ele ficou(realizei isto através de uma condição no main)
//QUANDO É UTILIZADA:É utilizada sucessivamente, ao longo da sequência de pontos feita pelo utilizador
//param boardtable matriz que contém todos os pontos válidos da seuqência feita
//param _posicao corresponde ao numero de pontos da sequencia válida até ao momento
int retroceder(int boardtable[][MAXIMUMMOVE], int *_posicao){
    //  A função reconhece se existem cores semelhantes, como foi explicado anteriormente, se houver a função retorna 0,caso não existam valores semelhantes, retorna1
    if((boardtable[0][*_posicao]==boardtable[0][*_posicao-2]) && (boardtable[1][*_posicao]== boardtable[1][*_posicao-2]))
    {
        if(*_posicao>=2){
            return 0;
        }

    }
    return 1;
}
//UTILIZAÇÃO:Serve para reconhecer se o programa está em condições de fazer shuffle ou não.
//Assim, eu tive de dividir a função em 3 partes para evitar analisar pontos fora do tabuleiro.
//Parte1-analiso toda a região excepto a primeira linha e última coluna
//Parte2-Analiso a primeira linha
//Parte3-Analiso a última coluna
//QUANDO É UTILIZADA:É utilizada depois de cada jogada para ver se existe ou não possíveis jogadas
//param boardtable matriz do tabuleiro
//param fcomprimento comprimento do tabuleiro
//param flargura largura do tabuleiro
int shuffle(int boardtable[][MAXSIZE], int *fcomprimento, int *flargura){
    int i=0,j=0;
    for(i=1;i<*fcomprimento;i++){
        for(j=0;j<*flargura-1;j++){
            if(boardtable[j+1][i]!=boardtable[j][i] && boardtable[j][i-1]!=boardtable[j][i]){//analisa para a direita e para cima

            }else{
                return 0;
            }
        }
    }
     for(i=0;i<1;i++){
        for(j=0;j<*flargura-1;j++){
            if(boardtable[j+1][i]!=boardtable[j][i]){//analisa apenas para a direita
            }else{
                return 0;
            }
        }
    }
     for(i=0;i<*fcomprimento-1;i++){
            if(boardtable[*flargura-1][i+1]!=boardtable[*flargura-1][i]){//analisa para baixo

            }else{
                return 0;
            }
        }

    //Ao longo da função,caso ela enconte uma sequência possível,faz imediatamente return 0,caso contrário, return 1

    return 1;
}
//UTILIZAÇÃO:Depois de ser reconhecido que não existem jogadas possíveis, pela função anterior, esta função vai realizar o shuffle propriamente dito;
// Tendo a preocupação de manter todas as peças e redistibuí-las, eu fui primeiro calcular quantas peças existem para cada cor e coloquei num vetor
// De seguida, foi gerar cores aleatórias do vetor, para cada posição
// QUANDO É UTILIZADA: É utilizada sempre que a função shuffle retornar 1,isto é, se não existirem mais jogadas possíveis
//param boardtable matriz do tabuleiro
//param fvetorsoma contém o número de vezes em que um dado cor aparece
//param numerodecores corresponde ao número de cores
//param fcomprimento comprimento do tabuleiro
//param flargura largura do tabuleiro
void shufflemove( int boardtable[][MAXSIZE], int fvetorsoma[100], int numerodecores, int fcomprimento, int flargura){
    int k,i,j,cor=0;
        for(k=0;k<numerodecores;k++)// vai fixar uma cor e somar ao vetorsoma,na posição igual à cor, sempre que for encontrada essa cor no tabuleiro
        {
            for(i=0;i<fcomprimento;i++)
            {
                for(j=0;j<flargura;j++)
                {
                    if(boardtable[j][i]==k)
                    {
                        fvetorsoma[k]+=1;

                      }
                }

            }

        }

        for(i=0;i<fcomprimento;i++){//percorre todas as posições e coloca uma cor aleatória
            for(j=0;j<flargura;j++){
                do{

                    cor=(int)(rand()%numerodecores);// Como se usou o ciclo do,while a função vai sempre entrar no ciclo para gerar uma cor
                                                //mas se o valor no vetorsoma dessa cor for 0, isto é, já foram distribuidos todas os pontos para essa cor
                                                //fica no ciclo até haver uma posição não nula do vetor, ou seja, um ponto,de uma cor qualquer, que não tenha sido colocado
                }while((fvetorsoma[cor]==0));
                boardtable[j][i]=cor;//se houverem pontos, para essa cor, a respetiva posição do tabuleiro passa a ter essa cor
                fvetorsoma[cor]--;// sempre que for colocada uma cor, subtraí-se um ponto a essa cor,pois já foi colocado

            }


        }

}
//UTILIZAÇÃO:Esta função serve para identificar de foi feita uma forma fechada, como um quadrado
//QUANDO É UTILIZADA:É utilizada em todas as jogadas para identificar quadrados
//param positiontable matriz que contém todos os pontos da sequência válida
//param fpt_x e fpt_y corresponde as coordenadas do moussemotion
//param dimensao número de pontos que existem na sequência
//param veotrdeinterseção guarda na primeira posição se o existe a formação de uma forma fechada e nas outras as coordenadas do ponto de interseção
void validarquadrado( int positiontable[][MAXIMUMMOVE], int fpt_x, int fpt_y, int dimensao, int vetordeintersecao[3]){
    int j=0;

    for(j=0;j<dimensao;j++)// como a dimensão só é atulizada a seguir à chamada desta função,não existe o risco de calhar na posição onde este foi armazenado
    {
        if(positiontable[0][j]==fpt_x && positiontable[1][j]==fpt_y){//Vai analisar se existem posições iguais
            vetordeintersecao[0]=1;//caso seja, criei um vetor para guardar a validação do quadrado na posição 0
            vetordeintersecao[1]=fpt_x;//e na primeiro e segunda posição guardei as coordenadas do ponto de interseção
            vetordeintersecao[2]=fpt_y;
            break;

        }

    }

}


//UTILIZAÇÃO:Serve para eliminar todos os pontos da mesma cor que o quadrado
//QUANDO É UTILIZADA:Sempre que for validado um quadrado
//param cordoquadrado corresponde a cor do primeiro ponto selecionado para formar o quadrado
//param erraseboard matriz com 0 e 1, onde as posições a 1 correspondem a pontos a eliminar na função searchvalues
//param fcomprimento comprimento do tabuleiro
//param flargura largura do tabuleiro
void processarquadrado(int cordoquadrado, int boardtable[][MAXSIZE], int erraseboard[][MAXSIZE], int fcomprimento, int flargura){
    int i,j=0;
    for(i=0;i<fcomprimento;i++)
    {   for(j=0;j<flargura;j++)
      {      if(boardtable[j][i]==cordoquadrado)// percorre todas as posições do tabuleiro e coloca a 1 a errasematrix caso a cor dessa posição seja igual à do quadrado
        {
                erraseboard[j][i]=1;//Volto a salientar que a errasematrix é utilizada

        }
      }
    }
}

//UTILIZAÇÃO:Esta função converte todas as posições colocadas na positiontable(contém todas os pontos da sequência efetuada)
// e coloca essas posições a 1 na errasematrix(matrix que contém, nas posições com valor 1 os pontos a eliminar),para depois serem eliminados.
//QUANDO É UTILIZADA:É utilizada sempre que se termina uma sequência válida para atualizar a matriz que elimina os pontos
//param positiontable contém todos os pontos da sequência efetuada
//param controlador contém o número de pontos válidos sequenciados
//param boardtable matriz com 0 e 1, onde as posições a 1 correspondem a posições a eliminar na função searchvalues
void convertecoordenadas(int positiontable[2][MAXIMUMMOVE], int controlador, int boardtable[][MAXSIZE]){
   int i=0;
    for(i=0;i<controlador;i++){
        boardtable[positiontable[0][i]][positiontable[1][i]]=1;

    }
}

//UTILIZAÇÃO:Tal como o nome indica serve para eliminar os pontos que estão dentro do quadrado
//Antes desta função é utilizada a função que elimina os pontos da sequencia(searchvalues),colocando-os todosa preto.
//Esta função vai analisar todos os pontos do tabuleiro e se encontrar algum ponto cujo ponto diretamente à esquerda e diretamente acima sejam pretos
//Assim, se encontrar algum ponto com essas condições coloca-o imediatamente a preta, para que o próximo ponto que esteja dentro da forma fechada sejam também abrangido
//QUANDO É UTILIZADA:Sempre que é detetada uma forma fechada
//param boardtable matriz do tabuleiro
//param fcomprimento comprimento do tabuleiro
//param flargura largura do tabuleiro
void eliminarospontosdentro(int boardtable[][MAXSIZE], int fcomprimento, int flargura){
    int i,j=0;
    for(i=0;i<fcomprimento;i++){
        for(j=0;j<flargura;j++){
            if(boardtable[j-1][i]==black && boardtable[j][i-1]==black){
                boardtable[j][i]=black;
                                   }
                                 }
                               }
}
//UTILIZAÇÃO:Faz a diferença entre o numero de pontos de cada cor antes de colocar a sequência a preto ,para ser eliminada, com o numero de cores depois da sequência
//para saber quantos pontos de cada cor foram eliminados-
//QUANDO É UTILIZADA:Esta função é utilizada depois de cada jogada para atualizar os objetivos,isto é, o número de pontos que já foram atingidos
//param vecsomafinal vai armazenar o numero de pontos para cada cor depois dos pontos da sequencia feita serem colocados a preto
//param fnumerodecores contém o numero de cores
//param vecsomainicial vai armazenar o numero de pontos para cada cor antes de ser processada a jogada
//param vetorjogadas contém os objetivos para cada cor atualizados com a nova joagda feita
//param vecsomatotal contém a diferença entre o numero de pontos iniciais e finais para depois serem atualizados os objetivos
void decrementodejogadas( int vecsomainicial[100], int vecsomafinal[100],int vecsomatotal[100], int vetorjogadas[100], int fnumerodecores){

    for(int i=0;i<fnumerodecores;i++){
      vecsomatotal[i]=vecsomainicial[i]-vecsomafinal[i];
      vetorjogadas[i]=vetorjogadas[i]-vecsomatotal[i];
    }
    for(int j=0;j<5;j++){
      vecsomatotal[j]=0; // eliminar a diferença da jogada anterior
    }




}
//UTILIZAÇÃO:Conta os pontos para cada cor,depois de ter colocado a sequència feita a preto
//QUANDO É UTILIZADA: Esta função é utilizada depois da função searchvalues,isto é, depois de colocar os pontos a preto
//param boardtable matriz do tabuleiro
//param fvetorsomafinal vai armazenar o numero de pontos para cada cor depois dos pontos da sequencia feita serem colocados a preto
//param fcomprimento contém o comprimento
//param flargura contém o largura
//param fnumerodecores contém o numero de cores
void contarpontosfinais(int boardtable[][MAXSIZE], int fvetorsomafinal[100], int fcomprimento, int flargura, int fnumerodecores){

        int k,i,j=0;

           for(k=0;k<fnumerodecores;k++)//fixa uma cor e analisa todas as posições do tabuleiro,se encontrar um ponto com essa cor,soma 1;
        {
            for(i=0;i<fcomprimento;i++)
            {
                for(j=0;j<flargura;j++)
                {
                    if(boardtable[j][i]==k)
                    {
                        fvetorsomafinal[k]+=1;

                      }

                }

            }

        }
}
//UTILIZAÇÃO:Esta função conta os pontos para cada cor, antes de se colocar as bolas sequenciadas a preto
//QUANDO DE UTILIZADA:É utilizada sempre no inicio do buttonup, para que antes de processar a seuqencia,seja possível ler o numero de pontos para cada cor
//param boardtable matriz do tabuleiro
//param fvetorsomainicial vai armazenar o numero de pontos para cada cor antes de ser processada a jogada
//param fcomprimento contém o comprimento
//param flargura contém o largura
//param fnumerodecores contém o numero de cores
void contarpontosiniciais(int boardtable[][MAXSIZE], int fvetorsomainicial[100], int fcomprimento, int flargura, int fnumerodecores){

        int k,i,j=0;

           for(k=0;k<fnumerodecores;k++)//fixa uma cor e analisa todas as posições do tabuleiro,se encontrar um ponto com essa cor,soma 1;
        {
            for(i=0;i<fcomprimento;i++)
            {
                for(j=0;j<flargura;j++)
                {
                    if(boardtable[j][i]==k)
                    {
                        fvetorsomainicial[k]+=1;

                      }

                }

            }

        }
}




//UTILIZAÇÃO:Esta função serve para verificar depois de cada jogada se já foram cumpridos os objetivos, isto é,
//se já foram sequenciados todos os pontos definidos inicialmente.
//QUANDO É UTILIZADA:Sempre que se acaba de processar uma jogada, esta função analisa se já foram atingidos todos os pontos,
//caso o utilizador tenha conseguido, a função retorna 1
//param vetorjogadas contém os objetivos para cada cor atualizados em cada jogada
//param numerodecores contém o numero de cores
int verificarobjetivos( int vetorjogadas[100], int numerodecores){
    int i=0;
    for(i=0;i<numerodecores;i++)
    { // se o utilizador chegar ao objetivo, tenho de fixar essa cor a 0,pois não são necessários mais pontos
        if(vetorjogadas[i]<0)
        {
            vetorjogadas[i]=0;
         }
     }
    if((vetorjogadas[0]==0) && (vetorjogadas[1]==0) && (vetorjogadas[2]==0) && (vetorjogadas[3]==0) && (vetorjogadas[4]==0)){// caso ele cumpra os objetivos,retorna 1
        return 1;// GANHOU!!

    }

return 0;
}

//UTILIZAÇÃO:Serve para eliminar os valores dos vetores que guardam o numero de pontos para cada cor
//QUANDO É UTILIZADA:Depois de concluir todas as oprações de uma jogada normal
//param vetorsomainicial guarda os pontos para cada cor antes do processamento da sequencia
//param vetorsomafinal guarda os pontos para cada cor depois de colocar a sequencia a preto
//param vetorsomashuffle guarda os pontos para cada cor antes da operação do shuffle
void apagarvaloresanteriores( int vetorsomainicial[100], int vetorsomafinal[100], int vetorsomashuffle[100]){
    for(int i=0;i<5;i++){
        vetorsomainicial[i]=0;

    }
    for(int j=0;j<5;j++){
        vetorsomafinal[j]=0;

    }
    for(int j=0;j<5;j++){
        vetorsomashuffle[j]=0;

    }

}

//UTILIZAÇÃO:Serve para guardar o tabuleiro da jogada anterior
//QUANDO É UTILIZADA:É utilizada para a função undo,para ser possivel atualizar a matriz "anterior" sempre que se pressione a tecla u
//param boardtable matriz do tabuleiro
//param fcomprimento comprimento do tabuleiro
//param flargura largura do tabuleiro
//param saveboard nova matriz que guarda os pontos até agora
void SaveJogada( int boardtable[][MAXSIZE], int fcomprimento, int flargura, int saveboard[][MAXSIZE]){

    for(int i=0;i<fcomprimento;i++){
        for(int j=0; j<flargura;j++){
            saveboard[j][i]=boardtable[j][i];
        }
    }
}
//UTILIZAÇÃO:Serve para realizar a opreação de undo.Para cada posição do tabuleiro eu passo a guardar o que estava no tabuleiro anterior.
//QUANDO É UTILIZADA:É utilizada sempre que se pressiona a tecla u
//param boardtable matriz do tabuleiro
//param fcomprimento comprimento do tabuleiro
//param flargura largura do tabuleiro
//param saveboard matriz que contem os pontos da jogada anterior
void undo(int boardtable[][MAXSIZE], int fcomprimento, int flargura, int saveboard[][MAXSIZE]){
    for(int i=0;i<fcomprimento;i++){
        for(int j=0;j<flargura;j++){
            boardtable[j][i]=saveboard[j][i];
        }
    }
}
//UTILIZAÇÃO:Vai guardar o número de jogadas e numero de pontos da jogada anterior, para o undo
//QUANDO É UTILIZADA:Antes de eu processar uma nova jogada
//param numerodejogadas contém as jogadas até ao momento
//param *jogadasanteriores guarda o numerode jogadas anterior
//param numerodecores contém o número de cores inicial
//param vetorpontosanteriores guarda o numero de pontos para cada cor da jogada até ao momento
//param vetorauxiliar vai guardar o numero de pontos para cada cor da jogada anterior
void guardarvaloresanteriores(int numerodejogadas, int *jogadasanteriores, int numerodecores, int vetorpontosanteriores[STRING_SIZE], int vetorauxiliar[STRING_SIZE]){
    for(int i=0;i<numerodecores;i++){
        vetorauxiliar[i]=vetorpontosanteriores[i];//guarda num vetor o numero de pontos, para cada cor, do tabuleiro anterior
    }

    *jogadasanteriores=numerodejogadas;// guarda o numero de jogadas anterior
}





//UTILIZAÇÃO:Caso o utilizador queira fazer um jogo novo, o numero de jogadas e os objetivos têm de ser iguais aos tabelados inicialmente pelo utilizador,logo esta
//função vai guardar o número de jogadas e os objetivos para depois serem utilizado para um novo jogo
//QUANDO É UTILIZADA:Logo a seguir à função leitura dos parâmetro,para poder guardar os valores iniciais
//param vetorauxiliar  vai guardar o numero de pontos iniciais para cada cor
//param numerodecores contém o número de cores colocadas pelo utilizador
//param vetorpontosiniciais contém o número de pontos colocados pelo utilizador
//param guardajogadainicial está como ponteiro para o seu valor ser alterado para as jogadas que o utilizador colocou
//param numerodejogadas tem o numero de jogadas colocado pelo utilizador
void PontosParaNewGame( int vetorauxiliar[STRING_SIZE], int numerodecores, int vetorpontosiniciais[STRING_SIZE], int *guardajogadainicial, int numerodejogadas){
    for(int i=0;i<numerodecores;i++){
        vetorauxiliar[i]=vetorpontosiniciais[i];//guarda num vetor os objetivos para cada cor
    }
    *guardajogadainicial=numerodejogadas;//guarda nume variável o número de jogadas
}

//UTILIZAÇÃO:Sempre que é terminado o programa, são guardadas as estatísticas do utilizador no ficheiro estatisticas,que contém a seguinte informação:
//Nome do jogador;
//Se o jogador ganhou ou perdeu;
//Quantas jogadas fez para ganhar ou perder;
//Nesta função entra uma matriz que tem como conteúdo o numero de jogadas(linha 0) e o resultado(linha 1)
//QUANDO É UTILIZADA:Sempre que o utilizador pressiona a tecla q para sair do programa
//param nome contém o nome do utilizador
//param matrixstats matriz que contem na linha 0 o numero de jogadas e na linha 1 o resultado
//param numerodejogos contém o número total de jogos que o utilizador fez
void Ficheiros(char nome[STRING_SIZE], int matrixstats[2][MAXIMUMMOVE],int numerodejogos){
    int somajogos=0;
    int somavit=0;
    int somader=0;
    FILE *file;
    file=fopen("stats.txt","w");
    if(file==NULL){// validação,caso o ficheiro não abra, o prgrama vai fechar
        printf("Não foi possível abrir o ficheiro");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "Estatisticas do jogo\n");
    //para cada jogo vai mostrar as suas respetivas estatisticas
    for(int i=0;i<numerodejogos;i++){
        fprintf(file, ">>O jogador: %s\n", nome);
        if(matrixstats[1][i]==1){
            fprintf(file,">>O jogador GANHOU!!\n");
            somajogos+=1;
            somavit+=1;
    }
        if(matrixstats[1][i]==0){
            fprintf(file,">>O jogador perdeu!!\n");
             somajogos+=1;
            somader+=1;
    }
        if(matrixstats[1][i]==1){
        fprintf(file,">>Em %d jogadas\n",matrixstats[0][i]);
        }
        fprintf(file,"\n\n");

    }
    fprintf(file,"O jogador realizou no total %d jogos\n",somajogos);
    fprintf(file,"O jogador perdeu %d vezes\n",somader);
    fprintf(file,"O jogador ganhou %d vezes\n",somavit);


    fclose(file);
}
//UTILIZAÇÃO:Serve para guardar numa matriz o resultado e o número de jogadas para o atingir
//QUANDO É UTILIZADA:Sempre que se termina um jogo.
//param numerodejogadas contém o número de jogadas com que o jogo acabou
//param matrixestatisticas vai passar a conter o resultado e o numero de jogadas
//param resultado se ganhou(1) ou perdeu(0)
//param numerodejogos, numero de jogos realizados
//param numerodejogosinicial contém o número definido pelo utilizador para as  jogadas
void guardaestatisticasdojogo(int numerodejogadas,int matrixestatisticas[2][MAXIMUMMOVE], int resultado,int numerodejogos, int numerodejogadasinicial){
    matrixestatisticas[0][numerodejogos]=numerodejogadasinicial-numerodejogadas;//como quero o número de jogasdas "gastas" subtraiu ao numerode jogadas estabelecidas o numero de jogadas com que o jogo acabou
    matrixestatisticas[1][numerodejogos]=resultado;

}



//UTILIZAÇÃO:Esta função conta os pontos para cada cor, antes de ser efetuado o shuffle
//QUANDO DE UTILIZADA:É utilizada antes da verificação do shuffle
//param boardtable matriz do tabuleiro
//param fvetorsomashuffle vai armazenar o numero de pontos para cada cor antes de ser processado o shuffle
//param fcomprimento contém o comprimento
//param flargura contém o largura
//param fnumerodecores contém o numero de cores
void contarpontosshuffle(int boardtable[][MAXSIZE], int fvetorsomashuffle[100], int fcomprimento, int flargura, int fnumerodecores){

        int k,i,j=0;

           for(k=0;k<fnumerodecores;k++)//fixa uma cor e analisa todas as posições do tabuleiro,se encontrar um ponto com essa cor,soma 1;
        {
            for(i=0;i<fcomprimento;i++)
            {
                for(j=0;j<flargura;j++)
                {
                    if(boardtable[j][i]==k)
                    {
                        fvetorsomashuffle[k]+=1;

                      }

                }

            }

        }


}
//UTILIZAÇÃO:Serve para limpar as matriz com as coordenadas dos pontos da sequencia feita
//QUANDO É UTILIZADA:No final de cada jogada
//param boardtable matriz que contém os pontos da sequencia feita
//param controlador numero de pontos válidos selecionados
void apagarmatrixvalidacao(int boardtable[2][MAXIMUMMOVE],int limite){
    for(int i=0;i<limite;i++){
        boardtable[0][i]=-1;
        boardtable[1][i]=-1;
               }
}
//UTILIZAÇÃO:Caso seja pressionada a tecla n para um novo jogo,esta função vai atualizar os objetivos com aqueles que o utilizador colocou inicialmente
//QUANDO É UTILIZADA:Sempre que se pressiona na tecla n
//param _numerodecores numero de cores colocado inicialmente
//param _numerodepontos numero de bolas necessárias para ganhar
//param _vetortransf contém os objetivos estabelecidos inicialmente pelo utilizador
void retornapontosiniciais(int _numerodecores,int _numerodepontos[STRING_SIZE], int _vetortransf[STRING_SIZE]){
    for(int i=0;i<_numerodecores;i++){
        _numerodepontos[i]=_vetortransf[i];}
}
//UTILIZAÇÃO:Caso seja pressionada a tecla u para voltar à jogada anterior, esta função vai atualizar os objetivos com os da jogada anterior
//QUANDO É UTILIZADA: Sempre que se pressiona a tecla u
//param numerodecores numero de cores colocado inicialmente
//param numerodepontos numero de bolas necessárias para ganhar
//param vetorpontosanteriores contém os objetivos estabelecidos inicialmente pelo utilizador
void retornapontosanteriores(int _numerodecores, int _numerodepontos[STRING_SIZE], int _vetorpontosanteriores[STRING_SIZE]){
    for(int i=0;i<_numerodecores;i++){
        _numerodepontos[i]=_vetorpontosanteriores[i];
}
}

//UTILIZAÇÃO:Serve para imprimir nos boardtable as instruções do jogo
void instrucoes( SDL_Renderer *_renderer, TTF_Font *_font){

    char texto[STRING_SIZE]="BEM VINDO!!";
    char texto1[STRING_SIZE]=">O jogo consiste em unir pontos para cumprir com os objetivos que predefiniu";
    char texto2[1000]=">Se ganhar ou perder, o jogo bloqueia ate carregar em q ou n";
    char texto3[STRING_SIZE]=">Pode carregar em n para um novo jogo com os mesmos parametros ou u para voltar";
    SDL_SetRenderDrawColor(_renderer,242,225,212,255);
    SDL_Color black = { 0, 0, 0 };

    SDL_Rect reta ={60,140,780,90};
    SDL_RenderFillRect( _renderer, &reta);
    RenderText(65,145, texto,_font, &black,_renderer);
    RenderText(65,165, texto1,_font, &black,_renderer);
    RenderText(65,185, texto2,_font, &black,_renderer);
    RenderText(65,205, texto3,_font, &black,_renderer);
}
// as próximas funções servem para imprimir uma imagem com um acontecimento.
void RenderWin(SDL_Renderer *_renderer, TTF_Font *_font){
    char texto[STRING_SIZE]="VITORIA!!!!";
    SDL_SetRenderDrawColor(_renderer,242,225,212,255);
    SDL_Color black = { 0, 0, 0 };

    SDL_Rect reta ={200,340,500,140};
    SDL_RenderFillRect( _renderer, &reta);
    RenderText(210,350, texto,_font, &black,_renderer);
}
void RenderLOSE(SDL_Renderer *_renderer, TTF_Font *_font){
    char texto[STRING_SIZE]="PERDEU!!!!";
    SDL_SetRenderDrawColor(_renderer,242,225,212,255);
    SDL_Color black = { 0, 0, 0 };

    SDL_Rect reta ={200,340,500,140};
    SDL_RenderFillRect( _renderer, &reta);
    RenderText(210,350, texto,_font, &black,_renderer);
}
void RenderSHUFFLE(SDL_Renderer *_renderer, TTF_Font *_font){
    char texto[STRING_SIZE]="SHUFFLE!!";
    SDL_SetRenderDrawColor(_renderer,242,225,212,255);
    SDL_Color black = { 0, 0, 0 };

    SDL_Rect reta ={200,340,500,140};
    SDL_RenderFillRect( _renderer, &reta);
    RenderText(210,350, texto,_font, &black,_renderer);
}
