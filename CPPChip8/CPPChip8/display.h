#pragma once

class Display {
public:
	Display();
	~Display();

	void Init();
	void Render(uint8_t arr[64*32]);
	int HandleEvents();
	void Close();
	int keyStates[16];


private:
	SDL_Window* window;
	SDL_Renderer* renderer;


};
