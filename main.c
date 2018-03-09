#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <zconf.h>
#include <time.h>

#define SIZE 1000
int board[SIZE][SIZE];
int buffer_board[SIZE][SIZE];

bool game;
//Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

void step();

//Frees media and shuts down SDL
//void close_window();

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
    game = true;

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

void close_window()
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

void graphic(){
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
        {
            int CELL_SIZE = 1;

            //Apply the image
            SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );

            //Update the surface
            SDL_UpdateWindowSurface( gWindow );

            SDL_Renderer* renderer;
            renderer = SDL_CreateRenderer(gWindow, -1, 0);

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, gWindow);

            SDL_Rect rect;
            rect.x = CELL_SIZE;
            rect.y = CELL_SIZE;
            rect.w = 10;
            rect.h = 10;

            SDL_Event event;

            while(game){

                while(SDL_PollEvent(&event) != 0){
                    if(event.type == SDL_QUIT){
                        game = false;
                    }
                }

                SDL_RenderFillRect(renderer, &rect);
                for (int i = 0; i < SIZE; ++i) {
                    for (int j = 0; j < SIZE; ++j) {
                        if(board[i][j] == 1){
                            rect.x = CELL_SIZE * j;
                            rect.y = CELL_SIZE * i;
                            SDL_RenderFillRect(renderer, &rect);
                        }
                    }
                }
                SDL_RenderPresent(renderer);
                step();

                //Wait two seconds
                SDL_Delay( 1 );
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, gWindow);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, gWindow);
            }
        }

    //Free resources and close_window SDL
    close_window();
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

void set_buffer_board(){
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            buffer_board[i][j] = board[i][j];
        }
    }
}

void get_buffer_board(){
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = buffer_board[i][j];
        }
    }
}

void kill_cells_underpopulation(){
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
}

void kill_cells_overpopulation(){
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
}

void reproduction_cells(){
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
}


void step(){
    set_buffer_board();
    kill_cells_overpopulation();
    kill_cells_underpopulation();
    reproduction_cells();
    get_buffer_board();
}

void init_board(){
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = 0;
        }
    }
}

void add_rand_cells(){
    srand(time(NULL));
    for (int k = 0; k < 10000; ++k) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;

        board[x][y] = 1;

    }
}

int main( int argc, char* args[] )
{
    //graphic();
    init_board();
    add_rand_cells();
    
    graphic();


    return 0;
}