#include <iostream>
#include <string>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"

SDL_Renderer *ren = NULL;
std::string townPng = "./resources/town.png";
std::string indoorPng = "./resources/indoor.png";
//const int SCR_WIDTH = 704;
const int SCR_WIDTH = 176;
//const int SCR_HEIGTH = 768;
const int SCR_HEIGTH = 192;

bool nullCheck(void* param, std::string msg);
void cleanUpAndExit();
void printSDLError(std::string);
SDL_Texture* loadTexture(std::string fileName);
void applySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend);

int main (int argc, char** argv)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *win = NULL;
	win = SDL_CreateWindow("Hi Jerkus", 100, 100, SCR_WIDTH, SCR_HEIGTH, SDL_WINDOW_SHOWN);
	if (nullCheck(win, "SDL_CreateWindow failed")) {
		return 1;
	}

	ren = SDL_CreateRenderer(win, -1,
 		SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC);


	if (nullCheck(ren, "SDL_CreateRenderer failed")) {
		return 1;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		std::cout << "IMG_Init failed" << std::endl;
	}

	SDL_Texture *background = loadTexture(indoorPng);
	SDL_Texture *foreground = loadTexture(townPng);

	SDL_RenderClear(ren);
	
	// Draw from bottom to top
	applySurface(0, 0, background, ren);

	int fW, fH;
	SDL_QueryTexture(foreground, NULL, NULL, &fW, &fH);
	applySurface(SCR_WIDTH / 2 - fW/2, SCR_HEIGTH / 2 - fH/2, foreground, ren);
	
	SDL_RenderPresent(ren);
        SDL_Delay(2000);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(foreground);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

SDL_Texture* loadTexture(std::string fileName) {
	SDL_Surface* surface = IMG_Load(fileName.c_str());
	if (nullCheck(surface, "IMG_Load failed for " + fileName)){
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
	if (nullCheck(texture, "SDL_CreateTextureFromSurface failed for " + fileName)){
		return NULL;
	}
	return texture;
}

void applySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend){
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;

	// Get the width and height of the texture and set pos.w and pos.h to them
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);

	// Copy in the texture at the position and size specified by pos.
	SDL_RenderCopy(rend, tex, NULL, &pos);
}
void cleanUpAndExit(SDL_Texture* tex, SDL_Renderer* ren, SDL_Window* win) {
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

bool nullCheck(void* param, std::string msg)
{
	if (param == NULL) {
		printSDLError(msg);
		return true;
	}
	return false;
}

void printSDLError()
{
	printSDLError("");
}

void printSDLError(std::string msg)
{
	std::cout << msg << SDL_GetError() << std::endl;
}
