#define _ROWS            40 // 40
#define _COLUMNS         75 // 50
#define _SQUARESIZE      10
#define _SCREEN_WIDTH    800
#define _SCREEN_HEIGHT   600
#define _DEAD             0
#define _ALIVE            1
#define _DELAY          500 

#include <iostream>
#include <fstream>
#include <string>
#include <SDL_ttf.h>

using namespace std;

const string CONST_FONT = "./fonts/FromCartoonBlocks.ttf";


void save_flat_file_matriz(string  file_name, uint16_t(&matriz)[_ROWS][_COLUMNS]) {
	ofstream myfile;
	myfile.open(file_name);
	string line = "";

	for (uint16_t f = 0; f < _ROWS; f++)
	{
		for (uint16_t c = 0; c < _COLUMNS; c++)
		{
			auto str = to_string(matriz[f][c]);
			line.append(str + " ");
		}
		myfile << line + "\n";
		line = "";
	}

	myfile.close();
}

void save_info(string data) {
	ofstream myfile;
	myfile.open("data.txt", ios_base::app);
	myfile << data + "\n";
	myfile.close();
}

void init_matriz(uint16_t(&matriz)[_ROWS][_COLUMNS]) {

	for (uint16_t f = 0; f < _ROWS; f++)
	{
		for (uint16_t c = 0; c < _COLUMNS; c++)
		{
			matriz[f][c] = _DEAD;
		}
	}
}

void draw_square_filled(SDL_Renderer* m_window_renderer, uint16_t f, uint16_t c) {
	SDL_Rect rect;
	rect.x = _SQUARESIZE + c;
	rect.y = _SQUARESIZE + f;
	rect.w = _SQUARESIZE;
	rect.h = _SQUARESIZE;

	// Set the color WHITE  255,255,255 
	SDL_SetRenderDrawColor(m_window_renderer, 255, 255, 255, 255);
	// Draw Rect
	SDL_RenderDrawRect(m_window_renderer, &rect);
	// Fill Rect 
	SDL_RenderFillRect(m_window_renderer, &rect);
	SDL_RenderPresent(m_window_renderer);
}

void draw_square_black(SDL_Renderer* m_window_renderer, uint16_t f, uint16_t c) {
	SDL_Rect rect;
	rect.x = _SQUARESIZE + c;
	rect.y = _SQUARESIZE + f;
	rect.w = _SQUARESIZE;
	rect.h = _SQUARESIZE;

	// Set the color used  Black 0, 0, 0
	SDL_SetRenderDrawColor(m_window_renderer, 0, 0, 0, 255);
	// Draw Rect
	SDL_RenderDrawRect(m_window_renderer, &rect);
	// Fill Rect 
	SDL_RenderFillRect(m_window_renderer, &rect);
	//SDL_RenderPresent(m_window_renderer);
}


void draw_grid_squares(SDL_Renderer* renderer) {

	uint16_t pos = (_SCREEN_WIDTH / 2) - ((_COLUMNS * _SQUARESIZE) / 2);
	for (uint16_t f = 0; f < _ROWS; f++)
	{
		for (uint16_t c = 0; c < _COLUMNS; c++)
		{
			SDL_Rect rect;

			rect.x = _SQUARESIZE + (c * _SQUARESIZE) + pos;
			rect.y = _SQUARESIZE + (f * _SQUARESIZE);
			rect.w = _SQUARESIZE;
			rect.h = _SQUARESIZE;

			// Set the color used  Black 0, 0, 0
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			// Draw Rect
			SDL_RenderDrawRect(renderer, &rect);
			SDL_RenderPresent(renderer);
		}
	}
	// update screen .. refresh
	SDL_RenderPresent(renderer);
}

void draw_grid(SDL_Renderer* renderer, uint16_t(&matriz)[_ROWS][_COLUMNS]) {

	int span = 10;

	for (uint16_t f = 0; f < _ROWS; f++)
	{
		for (uint16_t c = 0; c < _COLUMNS; c++)
		{
			if (matriz[f][c] == _ALIVE)
				draw_square_filled(renderer, f * _SQUARESIZE + span, c * _SQUARESIZE + span);
			else
				draw_square_black(renderer, f * _SQUARESIZE + span, c * _SQUARESIZE + span);
		}
	}
	// update screen .. refresh 
	SDL_RenderPresent(renderer);
}

void add_oscillators_blinker(uint16_t(&matriz)[_ROWS][_COLUMNS], uint16_t rows = 5, uint16_t cols = 5) {
	/*
				X
		XXXXX   X   XXXXX
				X
	*/

	matriz[rows][cols] = _ALIVE;
	matriz[rows][(cols)+1] = _ALIVE;
	matriz[rows][(cols)+2] = _ALIVE;
}

void add_oscillators_toad(uint16_t(&matriz)[_ROWS][_COLUMNS], uint16_t rows = 10, uint16_t cols = 5) {


	matriz[rows][cols] = _ALIVE;
	matriz[rows + 1][cols] = _ALIVE;
	matriz[rows + 2][(cols)+1] = _ALIVE;
	matriz[rows + 2][cols + 1] = _ALIVE;
	matriz[rows - 1][cols + 2] = _ALIVE;
	matriz[rows][cols + 3] = _ALIVE;
	matriz[rows + 1][cols + 3] = _ALIVE;
}

void add_oscillators_beacom(uint16_t(&matriz)[_ROWS][_COLUMNS], uint16_t rows = 15, uint16_t cols = 5) {


	matriz[rows][cols] = _ALIVE;
	matriz[rows][cols + 1] = _ALIVE;
	matriz[rows + 1][cols] = _ALIVE;
	matriz[rows + 1][cols + 1] = _ALIVE;

	matriz[rows + 2][cols + 2] = _ALIVE;
	matriz[rows + 2][cols + 2 + 1] = _ALIVE;
	matriz[rows + 3][cols + 2] = _ALIVE;
	matriz[rows + 3][cols + 2 + 1] = _ALIVE;
}

void add_oscillators_pentadecation(uint16_t(&matriz)[_ROWS][_COLUMNS], uint16_t rows = 25, uint16_t cols = 7) {


	for (size_t f = 0; f < 8; f++)
	{
		for (size_t c = 0; c < 3; c++)
		{
			matriz[rows + f][cols + c] = _ALIVE;
		}
	}
}


void add_spaceships_glider(uint16_t(&matriz)[_ROWS][_COLUMNS], uint16_t rows = 5, uint16_t cols = 15) {


	// Left to Right
	matriz[rows][cols] = _ALIVE;
	matriz[rows + 1][cols + 1] = _ALIVE;
	matriz[rows + 1][(cols)+2] = _ALIVE;
	matriz[rows - 1][cols + 2] = _ALIVE;
	matriz[rows][cols + 2] = _ALIVE;
	// Right to left
	rows = 5;
	cols = _COLUMNS - 5;

	matriz[rows][cols] = _ALIVE;
	matriz[rows + 1][cols] = _ALIVE;
	matriz[rows + 1][cols + 2] = _ALIVE;
	matriz[rows + 2][cols] = _ALIVE;
	matriz[rows + 2][cols + 1] = _ALIVE;

}


void add_spaceships_lightweight(uint16_t(&matriz)[_ROWS][_COLUMNS], uint16_t rows = 25, uint16_t cols = 15) {


	matriz[rows + 1][cols - 1] = _ALIVE;
	matriz[rows + 3][cols - 1] = _ALIVE;
	matriz[rows][cols] = _ALIVE;
	matriz[rows][cols + 1] = _ALIVE;
	matriz[rows][cols + 2] = _ALIVE;
	matriz[rows][cols + 3] = _ALIVE;
	matriz[rows + 1][cols + 3] = _ALIVE;
	matriz[rows + 2][cols + 3] = _ALIVE;
	matriz[rows + 3][cols - 1] = _ALIVE;

	matriz[rows + 3][cols + 2] = _ALIVE;
}

/// <summary>
/// Nace: Si una célula muerta tiene exactamente 3 células vecinas 
///       vivas "nace" (es decir, al turno siguiente estará viva).
/// </summary>
void alive_cell(uint16_t(&matriz)[_ROWS][_COLUMNS],
	uint16_t(&matriz_temporal)[_ROWS][_COLUMNS]) {

	uint16_t count_alive_cells = 0;

	for (uint16_t f = 0; f < _ROWS; f++)
	{
		for (uint16_t c = 0; c < _COLUMNS; c++)
		{
			// Found Alive cell
			if (matriz[f][c] == _DEAD) {

				// Count alive cells
				// Head
				count_alive_cells += matriz[f - 1][c - 1] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f - 1][c] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f - 1][c + 1] == _ALIVE ? 1 : 0;
				// Mid
				count_alive_cells += matriz[f][c - 1] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f][c + 1] == _ALIVE ? 1 : 0;
				// Tail
				count_alive_cells += matriz[f + 1][c - 1] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f + 1][c] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f + 1][c + 1] == _ALIVE ? 1 : 0;

				if (count_alive_cells == 3) {

					matriz_temporal[f][c] = _ALIVE;
				}
				else {
					matriz_temporal[f][c] = matriz[f][c];
				}
				count_alive_cells = 0;
			}
		}
	}
}

/// <summary>
/// Muere: Una célula viva puede morir por uno de 2 casos:
///    1- Sobrepoblación: Si tiene más de tres vecinos al rededor
///    2- Aislamiento : Si tiene solo un vecino alrededor.
/// </summary>
void dead_cell(uint16_t(&matriz)[_ROWS][_COLUMNS],
	uint16_t(&matriz_temporal)[_ROWS][_COLUMNS]) {

	uint16_t count_alive_cells = 0;

	for (uint16_t f = 0; f < _ROWS; f++)
	{
		for (uint16_t c = 0; c < _COLUMNS; c++)
		{
			// 0 0 0 0 0
			// 0 1 1 1 0
			// 0 0 0 0 0

			// Found Alive cell
			if (matriz[f][c] == _ALIVE) {

				// Cound alive cells
				// Head
				count_alive_cells += matriz[f - 1][c - 1] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f - 1][c] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f - 1][c + 1] == _ALIVE ? 1 : 0;
				// Mid
				count_alive_cells += matriz[f][c - 1] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f][c + 1] == _ALIVE ? 1 : 0;
				// Tail
				count_alive_cells += matriz[f + 1][c - 1] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f + 1][c] == _ALIVE ? 1 : 0;
				count_alive_cells += matriz[f + 1][c + 1] == _ALIVE ? 1 : 0;

				/// Muere: Una célula viva puede morir por uno de 2 casos:
				///    1- Sobrepoblación: Si tiene más de tres vecinos al rededor
				///    2- Aislamiento : Si tiene solo un vecino alrededor.

				//if (count_dead_cells > 1 && count_alive_cells < 2) {
				if (count_alive_cells == 0 || count_alive_cells == 1 || count_alive_cells > 3) {
					matriz_temporal[f][c] = _DEAD;
				}
				else {
					matriz_temporal[f][c] = matriz[f][c];
				}
				count_alive_cells = 0;
			}
		}
	}
}


void update_dead_alive_matriz(uint16_t(&matriz)[_ROWS][_COLUMNS],
	uint16_t(&matriz_temporal)[_ROWS][_COLUMNS]) {

	for (size_t f = 0; f < _ROWS; f++)
	{
		for (size_t c = 0; c < _COLUMNS; c++)
		{
			matriz[f][c] = matriz_temporal[f][c];
		}
	}
}

void draw_layout(SDL_Renderer* renderer) {

	SDL_Rect rect;

	rect.x = 10;
	rect.y = 10;
	rect.w = _SCREEN_WIDTH - 20;
	rect.h = _SCREEN_HEIGHT - 20;

	// Set the color used  Black 0, 0, 0
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// Draw Rect
	//SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, 10, 420, _SCREEN_WIDTH - 10, 420);
	SDL_RenderDrawRect(renderer, &rect);
	// update screen .. refresh
	SDL_RenderPresent(renderer);
}


void show_text(SDL_Renderer* render, string text, uint16_t x, uint16_t y) {

	TTF_Font* font = TTF_OpenFont(CONST_FONT.c_str(), 30);
	SDL_Color textColor = { 255,255, 255, 0 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, textSurface);
	int text_width = textSurface->w;
	int text_height = textSurface->h;
	SDL_FreeSurface(textSurface);
	SDL_Rect renderQuad = { x, y, text_width, text_height };
	SDL_RenderCopy(render, texture, NULL, &renderQuad);
	TTF_CloseFont(font);
	SDL_DestroyTexture(texture);
}

void draw_init_patterns(uint16_t(&matriz)[_ROWS][_COLUMNS]) {
	// draw oscillator 
	add_oscillators_pentadecation(matriz);
	add_oscillators_blinker(matriz);
	add_oscillators_toad(matriz);
	add_oscillators_beacom(matriz);
	// draw spaceships
	add_spaceships_glider(matriz);
	add_spaceships_lightweight(matriz);

	add_oscillators_pentadecation(matriz, 25, 60);
	add_oscillators_blinker(matriz, 5, 60);
	add_oscillators_toad(matriz, 10, 60);
	add_oscillators_beacom(matriz, 15, 60);

	add_spaceships_glider(matriz);
	add_spaceships_glider(matriz, 5, 35);
	add_spaceships_glider(matriz, 10, 15);
	add_spaceships_lightweight(matriz, 15, 40);
}



/// <summary>
/// Main loop
/// </summary>
void game_of_life_main_loop() {

	uint16_t matriz[_ROWS][_COLUMNS];
	uint16_t matriz_temporal[_ROWS][_COLUMNS];
	uint16_t delay = _DELAY;
	uint8_t result = 0;
	string text = "";
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;
	SDL_Renderer* renderer = NULL; 
	SDL_Event e;
	bool quit = false;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	//Create window
	SDL_Window* window = SDL_CreateWindow("Game of Life Conway",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		_SCREEN_WIDTH, _SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Get window surface
		screenSurface = SDL_GetWindowSurface(window);

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL error SDL_INIT_VIDEO! SDL_Error: %s\n", SDL_GetError());
			return;
		}

		// Init font 
		if (TTF_Init() < 0) {
			printf("SDL error TTF_Init! SDL_Error: %s\n", SDL_GetError());
			return;
		}

		draw_layout(renderer); 

		show_text(renderer, "F1 : Clean", 20, 425);
		show_text(renderer, "F2 : Add spaceships glider", 20, 450);
		show_text(renderer, "F3 : Add spaceships lightweight", 20, 475);
		show_text(renderer, "F4 : Add oscilators", 20, 500);
		show_text(renderer, " + : Increase delay", 20, 525);
		show_text(renderer, " - : Decrease delay", 20, 550);
		 

		// init/clear matriz with cero
		init_matriz(matriz);
		init_matriz(matriz_temporal); 
		draw_init_patterns(matriz); 
		// draw grid
		draw_grid(renderer, matriz);
		SDL_Delay(delay);

		while (!quit) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_KEYDOWN)
				{

					if (e.type == SDL_QUIT) {
						quit = true;
					}

					if (e.key.keysym.sym == SDLK_ESCAPE) {
						SDL_Log("SDLK_ESCAPE .. bye bye");
						quit = true;
						return;
					}

					// Clear matriz
					if (e.key.keysym.sym == SDLK_F1) {
						init_matriz(matriz);
						init_matriz(matriz_temporal);
					}

					if (e.key.keysym.sym == SDLK_F2) {
						add_spaceships_glider(matriz);
						add_spaceships_glider(matriz, 5, 35);
					}

					if (e.key.keysym.sym == SDLK_F3) {
						add_spaceships_lightweight(matriz);
					}
					if (e.key.keysym.sym == SDLK_F4) {
						// draw oscillator 
						add_oscillators_pentadecation(matriz);
						add_oscillators_blinker(matriz);
						add_oscillators_toad(matriz);
						add_oscillators_beacom(matriz);

						add_oscillators_pentadecation(matriz, 25, 55);
						add_oscillators_blinker(matriz, 5, 55);
						add_oscillators_toad(matriz, 10, 60);
						add_oscillators_beacom(matriz, 15, 60);

					}

					if (e.key.keysym.sym == SDLK_KP_PLUS) {
						delay += 50;
						cout << "Delay value (+) " << delay << endl;
					}

					if (e.key.keysym.sym == SDLK_KP_MINUS) {

						if (delay - 50 >= 50) {
							delay = delay - 50;
							cout << "Delay value (-) " << delay << endl;
						}
						else
							cout << "Minimal delay reached :" << delay << endl; 
					}
				} 
			}

			// *******************************************

			alive_cell(matriz, matriz_temporal);
			dead_cell(matriz, matriz_temporal);
			update_dead_alive_matriz(matriz, matriz_temporal);
			draw_grid(renderer, matriz);
			init_matriz(matriz_temporal);
			SDL_Delay(delay);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderPresent(renderer); 
		}
	}

	SDL_FreeSurface(screenSurface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

}

