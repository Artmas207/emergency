#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <zconf.h>

#define SIZE 100
int board[SIZE][SIZE];
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

void step();

//Frees media and shuts down SDL
//void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load splash image
    gHelloWorld = SDL_LoadBMP( "hello_world.bmp" );
    if( gHelloWorld == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
        success = false;
    }

    return success;
}

/*void close()
{
    //Deallocate surface
    SDL_FreeSurface( gHelloWorld );
    gHelloWorld = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
*/
void graphic(){
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Apply the image
            SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );

            //Update the surface
            SDL_UpdateWindowSurface( gWindow );

            SDL_Renderer* renderer;
            renderer = SDL_CreateRenderer(gWindow, -1, 0);

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, gWindow);

            SDL_Rect rect;
            rect.x = 10;
            rect.y = 10;
            rect.w = 10;
            rect.h = 10;

            while(0 == 0){
                SDL_RenderFillRect(renderer, &rect);
                for (int i = 0; i < SIZE; ++i) {
                    for (int j = 0; j < SIZE; ++j) {
                        if(board[i][j] == 1){
                            rect.x = 10 * j;
                            rect.y = 10 * i;
                            SDL_RenderFillRect(renderer, &rect);
                        }
                    }
                }
                SDL_RenderPresent(renderer);
                step();

                //Wait two seconds
                SDL_Delay( 500 );
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, gWindow);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, gWindow);
          }
        }
    }

    //Free resources and close SDL
    //close();
}



int get_neighbours(int i, int j){
    int neighbors = 0;
    if(board[i - 1][j - 1] != 0){
        neighbors++;
    }
    if(board[i - 1][j] != 0){
        neighbors++;
    }
    if(board[i - 1][j + 1] != 0){
        neighbors++;
    }
    if(board[i][j + 1] != 0){
        neighbors++;
    }
    if(board[i + 1][j + 1] != 0){
        neighbors++;
    }
    if(board[i + 1][j] != 0){
        neighbors++;
    }
    if(board[i + 1][j - 1] != 0){
        neighbors++;
    }
    if(board[i][j - 1] != 0){
        neighbors++;
    }

    return neighbors;
}

#define clear() printf("\033[H\033[J")


void step(){
    int buffer_board[SIZE][SIZE];
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                buffer_board[i][j] = board[i][j];
            }
        }

        // LOGIC

        for (int i = 1; i < SIZE; ++i) {
            for (int j = 1; j < SIZE; ++j) {
                int neighbours = get_neighbours(i, j);
                if(board[i][j] != 0){
                    if(neighbours < 2){
                        buffer_board[i][j] = 0; //Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
                    }
                }
            }
        }

        for (int i = 1; i < SIZE; ++i) {
            for (int j = 1; j < SIZE; ++j) {
                int neighbours = get_neighbours(i, j);
                if(board[i][j] != 0){
                    if(neighbours > 3) {
                        buffer_board[i][j] = 0;
                    }//Any live cell with more than three live neighbours dies, as if by overpopulation.
                }
            }
        }

        for (int i = 1; i < SIZE; ++i) {
            for (int j = 1; j < SIZE; ++j) {
                int neighbours = get_neighbours(i, j);
                if(board[i][j] == 0){
                    if(neighbours > 2){
                        buffer_board[i][j] = 1;
                    }
                }
            }
        }

        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                board[i][j] = buffer_board[i][j];
            }
        }
}

int main( int argc, char* args[] )
{
    //graphic();
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = 0;
        }
    }

    board[5][5] = 1;
    board[5][6] = 1;
    board[8][5] = 1;
    board[8][6] = 1;
    board[6][4] = 1;
    board[6][7] = 1;
    board[7][4] = 1;
    board[7][7] = 1;

    board[11][5] = 1;
    board[11][6] = 1;
    board[14][5] = 1;
    board[14][6] = 1;
    board[12][4] = 1;
    board[12][7] = 1;
    board[13][4] = 1;
    board[13][7] = 1;

    for (int k = 0; k < 1000; ++k) {
        int x = rand() % 100;
        int y = rand() % 100;

        board[x][y] = 1;

    }

    bool game = true;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            printf("%d", board[i][j]);
        }printf("\n");
    }

//    while(game){
//
//
//
//
//    }

        graphic();


    return 0;
}

struct cells{
    int x;
    int y;
    bool isMarked;
};

struct cells cell[10];

void cells_init(){

}