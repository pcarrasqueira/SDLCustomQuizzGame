#include <CLWindow.h>
#include <string>
#include <sstream>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

using namespace std;

CLWindow::CLWindow()
{
	//Initialize non-existant window
	mWindow = NULL;
	mMouseFocus = false;
	mKeyboardFocus = false;
	mFullScreen = false;
	mMinimized = false;
	mWidth = 0;
	mHeight = 0;
}

bool CLWindow::init()
{
	//Create window
	int windowWidth = SCREEN_WIDTH;
	int windowHeight = SCREEN_HEIGHT;

	// Declare display mode structure to be filled in.
	SDL_DisplayMode current;

	SDL_Init(SDL_INIT_VIDEO);

	// Get current display mode of all displays.
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
	{
		int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);
		if (should_be_zero != 0)
			// In case of error...
			SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
		else
		{
			// On success, print the current display mode.
			SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", i, current.w, current.h, current.refresh_rate);
			windowWidth = current.w;
			windowHeight = current.h;
		}

	}
	mWindow = SDL_CreateWindow("Number of players : 0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
	if (mWindow != NULL)
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = windowWidth;
		mHeight = windowHeight;
	}

	return mWindow != NULL;
}

SDL_Renderer* CLWindow::createRenderer()
{
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	return mRenderer;
}

void CLWindow::HandleEvent(SDL_Event& e)
{
	//Window event occured
	if (e.type == SDL_WINDOWEVENT)
	{
		switch (e.window.event)
		{
			//Get new dimensions and repaint on window size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SDL_RenderPresent(mRenderer);
			break;

			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(mRenderer);
			break;

			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			break;

			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			mMinimized = true;
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			mMinimized = false;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			mMinimized = false;
			break;
		}
	}
	//Enter exit full screen on return key
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
	{
		if (mFullScreen)
		{
			SDL_SetWindowFullscreen(mWindow, SDL_FALSE);
			mFullScreen = false;
		}
		else
		{
			SDL_SetWindowFullscreen(mWindow, SDL_TRUE);
			mFullScreen = true;
			mMinimized = false;
		}
	}
	
}

void CLWindow::free()
{
	if (mWindow != NULL)
	{
		SDL_DestroyWindow(mWindow);
	}

	mMouseFocus = false;
	mKeyboardFocus = false;
	mWidth = 0;
	mHeight = 0;
}

int CLWindow::getWidth()
{
	return mWidth;
}

int CLWindow::getHeight()
{
	return mHeight;
}

bool CLWindow::hasMouseFocus()
{
	return mMouseFocus;
}

bool CLWindow::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool CLWindow::isMinimized()
{
	return mMinimized;
}

SDL_Window* CLWindow::GetSDLWindow()
{
	return mWindow;
}