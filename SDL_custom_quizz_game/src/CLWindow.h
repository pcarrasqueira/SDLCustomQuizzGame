#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

class CLWindow
{
public:
	//Intializes internals
	CLWindow();

	//Creates window
	bool init();

	//Creates renderer from internal window
	SDL_Renderer* createRenderer();

	//Handles window events
	void HandleEvent(SDL_Event& e);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();


	//Get sdl window object
	SDL_Window* GetSDLWindow();

private:
	//Window data
	SDL_Window* mWindow;

	//Window dimensions
	int mWidth;
	int mHeight;

	//Window focus
	bool mMouseFocus;
	bool mKeyboardFocus;
	bool mFullScreen;
	bool mMinimized;

	SDL_Renderer* mRenderer;
};

#endif