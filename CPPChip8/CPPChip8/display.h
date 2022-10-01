#pragma once

class Display {
public:
	Display();
	~Display();

	void Init();
	void Render(int8_t arr[64*32]);
	int HandleEvents();
	void Close();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};
