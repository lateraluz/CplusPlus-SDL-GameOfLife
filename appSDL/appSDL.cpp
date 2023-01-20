
// Must copy Sdl.dll /debug
#include <iostream> 
#include <SDL.h>
#include "GameOfLife.h" 

using namespace std;

int main(int argc, char* args[])
{ 
	game_of_life_main_loop(); 
	return 0;
}

