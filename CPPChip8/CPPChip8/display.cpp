#include "main.h"

Display::Display() {}
Display::~Display() {}

void Display::Init() {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Chip-8 Emulator CPP", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64 * 5, 32 * 5, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, 0, -1);

	std::cout << "Display Initalized" << std::endl;
}

void Display::Close() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "Display Closed" << std::endl;
}

void Display::Render(uint8_t arr[64*32]) {
	SDL_RenderClear(renderer);
	int byteNumber = 0;
	int c = -1;
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {

			c++;

			//int c = x + (y * 64);

			//int div = c % 8;
			int8_t bit = (arr[byteNumber] >> (8-c)) & 0x1;//(arr[c / 8] >> (8 - div)) & 0x1;



			if (bit == 0) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}

			SDL_Rect *tempRect = new SDL_Rect();
			tempRect->x = x * 5;
			tempRect->y = y * 5;
			tempRect->w = 5;
			tempRect->h = 5;
			SDL_RenderFillRect(renderer, tempRect);
			delete tempRect;

			if (c == 8) {
				c = 0;
				byteNumber++;
			}
		}
	}
	SDL_RenderPresent(renderer);
}

int Display::HandleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		return 1;
	default:
		break;
	}
	return 0;
}