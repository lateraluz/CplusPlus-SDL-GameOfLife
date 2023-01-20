#pragma once
#include <string>
#include <SDL_gesture.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace std;

#define _ROWS             30
#define _COLUMNS          50
#define _SQUARESIZE       15
#define _SCREEN_WIDTH    780
#define _SCREEN_HEIGHT   630
#define _DEAD              0
#define _ALIVE             1
#define _FOOD              2
#define _ROWS_POSITIONS   25
#define _COLS_POSITIONS    2
#define _INITIAL_SLEEP   180
#define _DECREASE_SLEEP   15 
#define _FEED_AMOUNT       7
#define _FONT_SIZE        55

const string  _FONT_NAME = "font.ttf";
const string _EFFECT_EAT = "./Wavs/low.wav";
const string _NEXT_LEVEL = "./Wavs/next_level.wav";
const string _WIN = "./Wavs/win.wav";
const string _LOST = "./Wavs/lost.wav";
const string _BACKGROUND_MUSIC = "./wavs/KomikuBicycle.wav";
const string _IMAGE_GAME_OVER = "./images/gameover.png";
const string _IMAGE_SNAKE = "./images/snake.png";


enum ENUM_LASTKEY { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, NONE = -1 };

enum ENUM_GAMESTATUS { PLAYING = 0, LOST = -1, WIN = 1 };

struct Node
{
	int row;
	int col;
	struct Node* next;
};

void draw_snake_square_empty(SDL_Renderer* renderer, const uint16_t f, const uint16_t c) {

	// White 255, 255, 255, 255
	// Black 0,0,0,255
	SDL_Rect rect;
	rect.x = _SQUARESIZE + c;
	rect.y = _SQUARESIZE + f;
	rect.w = _SQUARESIZE;
	rect.h = _SQUARESIZE;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderFillRect(renderer, &rect);
	// Show like grid
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//SDL_RenderDrawRect(renderer, &rect);

}


void draw_snake_square_filled(SDL_Renderer* renderer, const uint16_t f, const uint16_t c) {

	// White 255, 255, 255, 255
	// Black 0,0,0,255
	SDL_Rect rect;
	rect.x = _SQUARESIZE + c;
	rect.y = _SQUARESIZE + f;
	rect.w = _SQUARESIZE;
	rect.h = _SQUARESIZE;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderFillRect(renderer, &rect);
}

void draw_snake_square_food(SDL_Renderer* renderer, const uint16_t row, const uint16_t col) {

	// White 255, 255, 255, 255
	// Black 0,0,0,255
	SDL_Rect rect;
	rect.x = _SQUARESIZE + col;
	rect.y = _SQUARESIZE + row;
	rect.w = _SQUARESIZE;
	rect.h = _SQUARESIZE;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_RenderFillRect(renderer, &rect);
}

void draw_snake_grid(SDL_Renderer* renderer, uint16_t(&main_matriz)[_ROWS][_COLUMNS]) {

	for (uint16_t f = 0; f < _ROWS; f++)
	{
		for (uint16_t c = 0; c < _COLUMNS; c++)
		{
			if (main_matriz[f][c] == _ALIVE)
				draw_snake_square_filled(renderer, f * _SQUARESIZE, c * _SQUARESIZE);
			if (main_matriz[f][c] == _DEAD)
				draw_snake_square_empty(renderer, f * _SQUARESIZE, c * _SQUARESIZE);
			if (main_matriz[f][c] == _FOOD)
				draw_snake_square_food(renderer, f * _SQUARESIZE, c * _SQUARESIZE);
		}
	}
	// update screen .. refresh
	SDL_RenderPresent(renderer);
}


void clear_matriz(uint16_t(&main_matriz)[_ROWS][_COLUMNS]) {

	for (uint16_t f = 0; f < _ROWS; f++)
	{
		for (uint16_t c = 0; c < _COLUMNS; c++)
		{
			if (main_matriz[f][c] != _FOOD)
				main_matriz[f][c] = _DEAD;
		}
	}
}


void print_list(struct Node* p_head) {
	struct Node* current;
	uint16_t length = 0;
	for (current = p_head; current != NULL; current = current->next) {
		cout << current->row << " " << current->col << endl;
		length++;
	}

	cout << "Cantidad " << length << endl;
	cout << "------------------------------------------" << endl;
}


void snake_initial_pos(uint16_t(&main_matriz)[_ROWS][_COLUMNS], struct Node*& p_head) {

	uint16_t f, c = 0;
	f = 10;
	c = 10;

	// Snake' initial positions
	uint16_t posiciones[_ROWS_POSITIONS][_COLS_POSITIONS] = {
		{10,24},
		{10,23},
		{10,22},
		{10,21},
		{10,20},
		{10,19 },
		{10,18 },
		{10,17 },
		{10,16 },
		{10,15 },
		{10,14 },
		{10,13 },
		{9,13 },
		{8,13 },
		{7,13 },
		{7,12 },
		{7,11 },
		{7,10 }
	};

	for (size_t r = 0; r < _ROWS_POSITIONS; r++)
	{
		if (posiciones[r][0] > 0 && posiciones[r][1] > 0) {
			Uint16 rr = posiciones[r][0];
			Uint16 cc = posiciones[r][1];
			main_matriz[rr][cc] = _ALIVE;
		}
	}

	for (int i = _ROWS_POSITIONS - 1; i >= 0; i--)
	{
		if (posiciones[i][0] > 0 && posiciones[i][1] > 0) {
			struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
			new_node->row = posiciones[i][0];
			new_node->col = posiciones[i][1];
			new_node->next = p_head;
			p_head = new_node;
		}
	}
}

void add_new_snake_slot(struct Node*& p_head, const int16_t row, const int16_t col) {

	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
	new_node->row = row;
	new_node->col = col;
	new_node->next = p_head;
	p_head = new_node;
}

void update_matriz_and_snake_positions(uint16_t(&main_matriz)[_ROWS][_COLUMNS], int16_t& col, int16_t& row, struct Node*& p_head) {

	int16_t count = 0;

	// Boundaries
	if (col >= _COLUMNS)
		col = 0;

	if (col < 0)
		col = _COLUMNS - 1;

	if (row >= _ROWS)
		row = 0;

	if (row < 0)
		row = _ROWS - 1;

	// Store snake positions
	// add new one as Head
	struct Node* new_node;
	new_node = (struct Node*)malloc(sizeof(struct Node));

	if (new_node == NULL) {
		SDL_Log("Error malloc new_node");
		return;
	}

	// C++ Node* new_node = new Node();
	new_node->col = col;
	new_node->row = row;
	new_node->next = p_head;
	p_head = new_node;

	// delete the last one
	struct Node* temp;
	temp = p_head;

	while (temp->next->next != NULL)
		temp = temp->next;

	// Delete last node
	delete(temp->next);
	temp->next = NULL;

	// clear the matriz
	clear_matriz(main_matriz);

	// fill main matriz with new positions 
	struct Node* ptr;
	ptr = p_head;
	while (ptr != NULL) {
		uint16_t row_temp = ptr->row;
		uint16_t col_temp = ptr->col;
		main_matriz[row_temp][col_temp] = _ALIVE;
		ptr = ptr->next;
	}
}


void draw_level_score(SDL_Renderer* renderer, int score, int level) {

	// clear last score with black square
	SDL_Rect renderQuad;
	renderQuad.x = 15;
	renderQuad.y = 470;
	renderQuad.h = 30 + 20;
	renderQuad.w = 100 + 200;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &renderQuad);
	SDL_RenderFillRect(renderer, &renderQuad);

	// Draw new level & score 
	std::string score_text = "Level: " + to_string(level) + " Score: " + std::to_string(score);
	string font_name = "./Fonts/" + _FONT_NAME;
	TTF_Font* font = TTF_OpenFont(font_name.c_str(), _FONT_SIZE);
	SDL_Color textColor = { 255,  0,  0, 0 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, score_text.c_str(), textColor);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
	SDL_RenderCopy(renderer, text, NULL, &renderQuad);
	SDL_DestroyTexture(text);
	TTF_CloseFont(font);

	//SDL_RenderPresent(renderer);
}


void draw_screen_frame(SDL_Renderer* renderer) {

	// Draw rectangule
	SDL_Rect rect;
	rect.x = _SQUARESIZE - 1;
	rect.y = _SQUARESIZE - 1;
	rect.w = _SQUARESIZE * _COLUMNS + 2;
	rect.h = _SQUARESIZE * _ROWS + 2;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &rect);

	// Draw Images 
	SDL_Surface* snakeImagen = nullptr;
	SDL_Texture* snakeImagen_texture = nullptr;
	snakeImagen = IMG_Load(_IMAGE_SNAKE.c_str());
	if (snakeImagen == NULL) {
		std::cout << "Error loading image: " << IMG_GetError();
		return;
	}

	// Load texture
	snakeImagen_texture = SDL_CreateTextureFromSurface(renderer, snakeImagen);
	if (snakeImagen_texture == NULL) {
		std::cout << "Error creating texture";
		return;
	}

	// free pointer  after load texture
	SDL_FreeSurface(snakeImagen);

	for (size_t i = 0; i < 5; i++)
	{
		SDL_Rect texture_destinaton;
		texture_destinaton.x = 10 + (i * 150);
		texture_destinaton.y = 520;
		texture_destinaton.h = 100;
		texture_destinaton.w = 100;
		SDL_RenderCopy(renderer, snakeImagen_texture, NULL, &texture_destinaton);
	}
	SDL_DestroyTexture(snakeImagen_texture);

}

// avoid overlap with the last position.
bool is_overlap_with_head(int16_t row, uint16_t col, struct Node* p_head) {

	bool ret = false;
	struct Node* current = p_head;

	for (current = p_head; current != NULL; current = current->next) {
		if (p_head->next->row == row and p_head->next->col == col) {
			ret = true;
		}
	}
	return ret;
}

bool is_overlap_with_snake_body(const int16_t row, const uint16_t col, struct Node* p_head) {

	bool ret = false;
	struct Node* temp = p_head;

	while (temp->next != NULL) {
		if (row == temp->row && col == temp->col) {
			SDL_Log("Overlap");
			ret = true;
			break;
		}
		temp = temp->next;
	}
	return ret;
}

void feed_snake(uint16_t(&matriz)[_ROWS][_COLUMNS], int8_t account) {

	// Providing a seed value, BEWARE: declared outside "while loop" to avoid duplicates
	srand((unsigned)time(NULL));
	const int columns = _COLUMNS;
	const int rows = _ROWS;
	while (account >= 0) {

		// Get a random number  
		int col = (rand() % columns - 1);
		int row = (rand() % rows - 1);

		if (row % 2 == 0) {
			col = col + 1 >= columns - 1 ? col - 2 : col;

			if (matriz[row][col] == _FOOD || matriz[row][col + 1] == _ALIVE) {
				SDL_Log("Already matriz cell is busy");
				SDL_Delay(1);
				continue;
			}
			matriz[row][col] = _FOOD;
			matriz[row][col + 1] = _FOOD;

		}
		else {

			row = row + 1 >= rows - 1 ? row - 2 : row;
			if (matriz[row + 1][col] == _FOOD || matriz[row][col] == _ALIVE) {
				SDL_Log("Already matriz cell is busy");
				SDL_Delay(1);
				continue;
			}
			matriz[row][col] = _FOOD;
			matriz[row + 1][col] = _FOOD;
		}

		account--;


		//col = col >= columns ? col-- : col;
		//row = row >= rows ? row-- : row;
		/*
		// Valid matriz cell
		if (matriz[row][col] == _FOOD || matriz[row][col] == _ALIVE) {
			SDL_Log("Already matriz cell is busy");
			SDL_Delay(1);
			continue;
		}
		else {

			if (row % 2 == 0) {
				col = col + 1 >= columns - 1 ? col - 2 : col;
				matriz[row][col] = _FOOD;
				matriz[row][col + 1] = _FOOD;
			}
			else {
				row = row + 1 >= rows - 1 ? row - 2 : row;
				matriz[row][col] = _FOOD;
				matriz[row + 1][col] = _FOOD;
			}
			account--;
		}
		*/
	}
}

void set_inicial_food(uint16_t(&matriz)[_ROWS][_COLUMNS]) {

	// Inicial food
	/*
	matriz[15][15] = _FOOD;
	matriz[17][35] = _FOOD;
	matriz[17][38] = _FOOD;
	matriz[17][40] = _FOOD;
	*/
	matriz[20][25] = _FOOD;
	matriz[20][26] = _FOOD;
	matriz[20][27] = _FOOD;
	matriz[20][28] = _FOOD;
	matriz[20][29] = _FOOD;
	matriz[20][30] = _FOOD;
	matriz[20][31] = _FOOD;
	matriz[20][32] = _FOOD;
	matriz[20][33] = _FOOD;
	matriz[20][34] = _FOOD;
	matriz[20][35] = _FOOD;
	matriz[20][36] = _FOOD;
	matriz[20][37] = _FOOD;
	matriz[20][38] = _FOOD;
	matriz[20][39] = _FOOD;
	matriz[20][34] = _FOOD;
}

void sound_snake_eat() {
	Mix_Chunk* sound = Mix_LoadWAV(_EFFECT_EAT.c_str());
	int result = Mix_PlayChannel(-1, sound, 2);
}

void sound_next_level() {
	Mix_Chunk* sound = Mix_LoadWAV(_NEXT_LEVEL.c_str());
	int result = Mix_PlayChannel(-1, sound, 0);
	sound = NULL;
}

void sound_win() {
	Mix_Chunk* sound = Mix_LoadWAV(_WIN.c_str());
	int result = Mix_PlayChannel(-1, sound, 0);
	sound = NULL;
}

void sound_lost() {
	Mix_Chunk* sound = Mix_LoadWAV(_LOST.c_str());
	int result = Mix_PlayChannel(-1, sound, 0);
	sound = NULL;
}

void update(uint16_t(&matriz)[_ROWS][_COLUMNS], SDL_Event e, SDL_Renderer* renderer, int16_t& row, int16_t& col, struct Node*& p_head, bool& is_first_time_key, ENUM_LASTKEY& lastKey, bool& quit, ENUM_GAMESTATUS& gameStatus, int& score, int& level, int& sleep) {

	if (e.type == SDL_KEYDOWN || true)
	{
		//draw_level_score(renderer, score, level);

		if (e.type == SDL_QUIT) {
			SDL_Log("Bye SDL_QUIT !");
			quit = true;
			return;
		}

		if (e.key.keysym.sym == SDLK_F1) {
			feed_snake(matriz, _FEED_AMOUNT);
			draw_snake_grid(renderer, matriz);
			return;
		}

		// ESC  = Quit
		if (e.key.keysym.sym == SDLK_ESCAPE) {
			SDL_Log("SDLK_ESCAPE .. bye bye");
			quit = true;
			return;
		}

		// Determinate continue first route
		if ((e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_LEFT ||
			e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_DOWN)
			&& is_first_time_key) {

			is_first_time_key = false;

			if (e.key.keysym.sym == SDLK_RIGHT)
				lastKey = RIGHT;
			if (e.key.keysym.sym == SDLK_LEFT)
				lastKey = LEFT;
			if (e.key.keysym.sym == SDLK_UP)
				lastKey = UP;
			if (e.key.keysym.sym == SDLK_DOWN)
				lastKey = DOWN;
		}


		if (e.key.keysym.sym == SDLK_RIGHT || lastKey == RIGHT)
		{
			lastKey = RIGHT;
			// Avoid overlap
			if (is_overlap_with_head(row, col + 1, p_head)) {
				lastKey = LEFT;
				return;
			}

			++col;

			if (is_overlap_with_snake_body(row, col, p_head)) {
				gameStatus = LOST;
				return;
			}

			if (matriz[row][col] == _FOOD) {
				score++;
				sound_snake_eat();

				if (score % 10 == 0 && score > 0) {
					sound_next_level();
					feed_snake(matriz, _FEED_AMOUNT);
					level++;
					sleep = sleep - _DECREASE_SLEEP > 10 ? sleep - _DECREASE_SLEEP : 10;

				}
				draw_level_score(renderer, score, level);
				add_new_snake_slot(p_head, row, col);
				matriz[row][col] = _ALIVE;
			}
			else {
				update_matriz_and_snake_positions(matriz, col, row, p_head);
			}
			draw_snake_grid(renderer, matriz);
		}

		if (e.key.keysym.sym == SDLK_LEFT || lastKey == LEFT)
		{
			lastKey = LEFT;
			// Avoid overlap
			if (is_overlap_with_head(row, col - 1, p_head)) {
				lastKey = RIGHT;
				return;
			}

			--col;

			if (is_overlap_with_snake_body(row, col, p_head)) {
				//SDL_Log("Lost");
				gameStatus = LOST;
				return;
			}


			if (matriz[row][col] == _FOOD) {
				score++;
				sound_snake_eat();
				if (score % 10 == 0 && score > 0) {
					sound_next_level();
					feed_snake(matriz, _FEED_AMOUNT);
					sleep = sleep - _DECREASE_SLEEP > 10 ? sleep - _DECREASE_SLEEP : 10;
					level++;
				}
				draw_level_score(renderer, score, level);
				add_new_snake_slot(p_head, row, col);
				matriz[row][col] = _ALIVE;
			}
			else {
				update_matriz_and_snake_positions(matriz, col, row, p_head);
			}

			draw_snake_grid(renderer, matriz);

		}

		if (e.key.keysym.sym == SDLK_UP || lastKey == UP)
		{
			lastKey = UP;
			// Avoid overlap
			if (is_overlap_with_head(row - 1, col, p_head)) {
				lastKey = DOWN;
				return;
			}

			--row;
			if (is_overlap_with_snake_body(row, col, p_head)) {
				gameStatus = LOST;
				return;
			}


			if (matriz[row][col] == _FOOD) {
				score++;
				sound_snake_eat();
				if (score % 10 == 0 && score > 0) {
					sound_next_level();
					feed_snake(matriz, _FEED_AMOUNT);
					sleep = sleep - _DECREASE_SLEEP > 10 ? sleep - _DECREASE_SLEEP : 10;
					level++;
				}
				draw_level_score(renderer, score, level);
				add_new_snake_slot(p_head, row, col);
				matriz[row][col] = _ALIVE;
			}
			else {
				update_matriz_and_snake_positions(matriz, col, row, p_head);
			}
			draw_snake_grid(renderer, matriz);

		}

		if (e.key.keysym.sym == SDLK_DOWN || lastKey == DOWN)
		{
			lastKey = DOWN;
			// Avoid overlap
			if (is_overlap_with_head(row + 1, col, p_head)) {
				lastKey = UP;
				return;
			}

			++row;

			if (is_overlap_with_snake_body(row, col, p_head)) {
				gameStatus = LOST;
				return;
			}

			if (matriz[row][col] == _FOOD) {
				score++;
				sound_snake_eat();
				if (score % 10 == 0 && score > 0) {
					sound_next_level();
					feed_snake(matriz, _FEED_AMOUNT);
					sleep = sleep - _DECREASE_SLEEP > 10 ? sleep - _DECREASE_SLEEP : 10;
					level++;
				}

				draw_level_score(renderer, score, level);
				add_new_snake_slot(p_head, row, col);
				matriz[row][col] = _ALIVE;
			}
			else {
				update_matriz_and_snake_positions(matriz, col, row, p_head);
			}
			draw_snake_grid(renderer, matriz);
		}

	}
}

void game_over(SDL_Renderer* renderer) {

	SDL_Surface* image = nullptr;
	SDL_Texture* imagen_texture = nullptr;

	// load image
	image = IMG_Load(_IMAGE_GAME_OVER.c_str());
	if (image == NULL) {
		std::cout << "Error loading image: " << IMG_GetError();
		return;
	}

	// Load texture
	imagen_texture = SDL_CreateTextureFromSurface(renderer, image);
	if (imagen_texture == NULL) {
		std::cout << "Error creating texture";
		return;
	}
	// free pointer  after load texture
	SDL_FreeSurface(image);
	//_SCREEN_WIDTH, _SCREEN_HEIGHT
	SDL_Rect texture_destinaton;
	texture_destinaton.x = 10;
	texture_destinaton.y = 10;
	texture_destinaton.h = 500;
	texture_destinaton.w = _SCREEN_WIDTH;
	SDL_RenderCopy(renderer, imagen_texture, NULL, &texture_destinaton);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(imagen_texture);

}


void win(SDL_Renderer* renderer) {

	SDL_Surface* image = nullptr;
	SDL_Texture* imagen_texture = nullptr;

	// load image
	image = IMG_Load("images/win1.png");
	if (image == NULL) {
		std::cout << "Error loading image: " << IMG_GetError();
		return;
	}

	// Load texture
	imagen_texture = SDL_CreateTextureFromSurface(renderer, image);
	if (imagen_texture == NULL) {
		std::cout << "Error creating texture";
		return;
	}
	// free pointer  after load texture
	SDL_FreeSurface(image);
	//_SCREEN_WIDTH, _SCREEN_HEIGHT
	SDL_Rect texture_destinaton;
	texture_destinaton.x = 10;
	texture_destinaton.y = 10;
	texture_destinaton.h = 500;
	texture_destinaton.w = _SCREEN_WIDTH;
	SDL_RenderCopy(renderer, imagen_texture, NULL, &texture_destinaton);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(imagen_texture);

}


void not_used_sound() {
	//The music that will be played
	Mix_Music* gMusic = NULL;

	//The sound effects that will be used
	Mix_Chunk* gScratch = NULL;
	Mix_Chunk* gHigh = NULL;
	Mix_Chunk* gMedium = NULL;
	Mix_Chunk* gLow = NULL;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		//success = false;
	}


	gMusic = Mix_LoadMUS("./Wavs/beat.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());

	}

	//Load sound effects
	gScratch = Mix_LoadWAV("./Wavs/scratch.wav");
	if (gScratch == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());

	}

	gHigh = Mix_LoadWAV("./Wavs/high.wav");
	if (gHigh == NULL)
	{
		printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());

	}

	gMedium = Mix_LoadWAV("./Wavs/medium.wav");
	if (gMedium == NULL)
	{
		printf("Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}

	gLow = Mix_LoadWAV("./Wavs/low.wav");
	if (gLow == NULL)
	{
		printf("Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	}

	Mix_PlayChannel(-1, gHigh, 0);
	//x_PlayMusic(gMusic, 5);


	//Free the sound effects
	Mix_FreeChunk(gScratch);
	Mix_FreeChunk(gHigh);
	Mix_FreeChunk(gMedium);
	Mix_FreeChunk(gLow);
	gScratch = NULL;
	gHigh = NULL;
	gMedium = NULL;
	gLow = NULL;

	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
}


/// <summary>
/// Main
/// </summary>
void snake_main_loop() {

	int  sleep = _INITIAL_SLEEP;
	uint16_t matriz[_ROWS][_COLUMNS];
	int16_t row = 0;
	int16_t col = 0;
	int event = 0;
	int score = 0;
	int level = 1;
	// declare pointer
	struct Node* p_head = nullptr;
	bool quit = false;
	bool is_first_time_key = true;
	bool lost = false;
	SDL_Renderer* renderer = nullptr;
	SDL_Event e;
	int contador = 0;
	ENUM_LASTKEY lastKey = NONE;
	ENUM_GAMESTATUS gameStatus = PLAYING;
	Mix_Music* music = NULL;


	// Initialization the  pointer that will store snake body positions
	p_head = (struct Node*)malloc(sizeof(struct Node));
	p_head = NULL;

	// clear the matriz
	clear_matriz(matriz);

	// Inicial Pos
	snake_initial_pos(matriz, p_head);

	// Set initial food	
	feed_snake(matriz, _FEED_AMOUNT);

	// row & col pos
	row = p_head->row;
	col = p_head->col;


	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("Error SDL_INIT_VIDEO! SDL_Error: %s\n", SDL_GetError());
		return;
	}
	//Create window
	SDL_Window* window = SDL_CreateWindow("Snake",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		_SCREEN_WIDTH,
		_SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		//Update the surface
		SDL_UpdateWindowSurface(window);
		// Draw Snake
		draw_snake_grid(renderer, matriz);
		// Draw frame  
		draw_screen_frame(renderer);
		// Draw Level & Score
		draw_level_score(renderer, 0, 0);
		// Update screen
		//SDL_RenderPresent(renderer); 

		// Can load a PNG image?
		// flags IMG_INIT_JPG, IMG_INIT_PNG, IMG_INIT_TIF, IMG_INIT_WEBP
		if (IMG_Init(IMG_INIT_PNG) == 0) {
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				return;
			}
		}

		if (TTF_Init() < 0) {
			printf("SDL_image could not initialize! SDL_image Error:  \n");
			return;
		}

		//Update the surface
		SDL_UpdateWindowSurface(window);
		SDL_RenderPresent(renderer);

		// Init audio
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
			//success = false;
		}

		// Load background music
		music = Mix_LoadMUS(_BACKGROUND_MUSIC.c_str());
		// -1 means loop
		Mix_PlayMusic(music, -1);

		// Main loop
		while (((event = SDL_PollEvent(&e)) || true) && quit == false) {

			//cout << "Contador :" << ++contador << endl;
			update(matriz, e, renderer, row, col, p_head, is_first_time_key, lastKey, quit, gameStatus, score, level, sleep);

			// Lost
			if (gameStatus == LOST) {
				game_over(renderer);
				sound_lost();				
				SDL_Delay(10000);
				quit = true;
			}
			// Win!
			if (gameStatus == WIN) {
				sound_win();
				win(renderer);
				SDL_Delay(10000);
				quit = true;
			}

			SDL_Delay(sleep);
		}
	}

	
	delete p_head;

	//TTF_CloseFont(font);
	Mix_FreeMusic(music);
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	// Free resources 
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}


