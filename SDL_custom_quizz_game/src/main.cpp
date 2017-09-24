/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, strings, and string streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Font Sizes
const int QUESTION_FONT_SIZE = 48;
const int ANSWARE_FONT_SIZE = 24;

//Globally used font
TTF_Font *gFont = NULL;

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

#ifdef _SDL_TTF_H
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, int Width = NULL, int Height = NULL, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

class LWindow
{
public:
	//Intializes internals
	LWindow();

	//Creates window
	bool init();

	//Creates renderer from internal window
	SDL_Renderer* createRenderer();

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();

	//Window focii
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

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
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

bool loadQuestion();

//Frees media and shuts down SDL
void close();

//Our custom window
LWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gSceneTexture;
LTexture gSceneTextureA;
LTexture gSceneTextureB;
LTexture gSceneTextureC;
LTexture gSceneTextureD;

//Scene textures
LTexture gQuestionText;
LTexture gAnswareA;
LTexture gAnswareB;
LTexture gAnswareC;
LTexture gAnswareD;


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, int Width, int Height, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad;
	if (Width != NULL && Height != NULL)
	{
		renderQuad = { x, y, Width, Height };
		mWidth = Width;
		mHeight = Height;
	}
	else
	{
		renderQuad = { x, y, mWidth, mHeight };
	}

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LWindow::LWindow()
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

bool LWindow::init()
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

	mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (mWindow != NULL)
	{
		mMouseFocus = true;
		mKeyboardFocus = true;
		mWidth = SCREEN_WIDTH;
		mHeight = SCREEN_HEIGHT;
	}

	return mWindow != NULL;
}

SDL_Renderer* LWindow::createRenderer()
{
	return SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void LWindow::handleEvent(SDL_Event& e)
{
	//Window event occured
	if (e.type == SDL_WINDOWEVENT)
	{
		//Caption update flag
		bool updateCaption = false;

		switch (e.window.event)
		{
			//Get new dimensions and repaint on window size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			mWidth = e.window.data1;
			mHeight = e.window.data2;
			SDL_RenderPresent(gRenderer);
			break;

			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(gRenderer);
			break;

			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			mMouseFocus = true;
			updateCaption = true;
			break;

			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			mMouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			mKeyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			mKeyboardFocus = false;
			updateCaption = true;
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

		//Update window caption with new data
		if (updateCaption)
		{
			std::stringstream caption;
			caption << "SDL Tutorial - MouseFocus:" << ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off");
			SDL_SetWindowTitle(mWindow, caption.str().c_str());
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

void LWindow::free()
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

int LWindow::getWidth()
{
	return mWidth;
}

int LWindow::getHeight()
{
	return mHeight;
}

bool LWindow::hasMouseFocus()
{
	return mMouseFocus;
}

bool LWindow::hasKeyboardFocus()
{
	return mKeyboardFocus;
}

bool LWindow::isMinimized()
{
	return mMinimized;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		if (!gWindow.init())
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = gWindow.createRenderer();
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load scene texture
	if (!gSceneTexture.loadFromFile("data\\graphics\\ui\\glassPanel.png"))
	{
		printf("Failed to load window texture!\n");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureA.loadFromFile("data\\graphics\\ui\\glassPanel.png"))
	{
		printf("Failed to load window texture!\n");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureB.loadFromFile("data\\graphics\\ui\\glassPanel.png"))
	{
		printf("Failed to load window texture!\n");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureC.loadFromFile("data\\graphics\\ui\\glassPanel.png"))
	{
		printf("Failed to load window texture!\n");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureD.loadFromFile("data\\graphics\\ui\\glassPanel.png"))
	{
		printf("Failed to load window texture!\n");
		success = false;
	}

	return success;
}

bool loadQuestion()
{

	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("data//font//kenvector_future_thin.ttf", QUESTION_FONT_SIZE);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if (!gQuestionText.loadFromRenderedText("Pergunta ?????", textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	return success;
}

bool loadAnswares()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("data//font//kenvector_future_thin.ttf", ANSWARE_FONT_SIZE);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };

		if (!gAnswareA.loadFromRenderedText("Resposta A", textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
		else if (!gAnswareB.loadFromRenderedText("Resposta B", textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
		else if (!gAnswareC.loadFromRenderedText("Resposta C", textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
		else if (!gAnswareD.loadFromRenderedText("Resposta D", textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	return success;
}

void close()
{
	//Free loaded images
	gSceneTexture.free();
	gSceneTextureA.free();
	gSceneTextureB.free();
	gSceneTextureC.free();
	gSceneTextureD.free();

	gQuestionText.free();
	gAnswareA.free();
	gAnswareB.free();
	gAnswareC.free();
	gAnswareD.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	//Quit SDL subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadQuestion())
		{
			printf("Failed to load question!\n");
		}
		else if (!loadAnswares())
		{
			printf("Failed to load answares!\n");
		}
		//Load media
		else if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle window events
					gWindow.handleEvent(e);
				}

				//Only draw when not minimized
				if (!gWindow.isMinimized())
				{
					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					//Render text textures
					int myWidth = gWindow.getWidth();
					int myHeight = 60;

					if (myHeight > gWindow.getHeight() / 4.0)
					{
						myHeight = gWindow.getHeight() / 4.0;
					}

					//Question
					gSceneTexture.render((gWindow.getWidth() - myWidth) / 2.0, (gWindow.getHeight()) / 4.0, myWidth, myHeight);
					
					int nQBorderX = 20;
					int nQBorderY = gSceneTexture.getHeight() / 2.0;

					gQuestionText.render(nQBorderX + ((gWindow.getWidth() - myWidth) / 2.0) + 50, nQBorderY - (QUESTION_FONT_SIZE / 2.0) + ((gWindow.getHeight()) / 4.0));

					int myWidthA = (gWindow.getWidth() / 2.0);
					int myHeightA = (gWindow.getHeight() / 8.0) / 2.0;

					//Answare A
					gSceneTextureA.render(((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);
					
					int nBorderX = 20;
					int nBorderY = gSceneTextureA.getHeight() / 2.0;

					gAnswareA.render(nBorderX + ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, nBorderY - (ANSWARE_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0));
					
					//Answare B
					gSceneTextureB.render(((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);
					gAnswareB.render(nBorderX + ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, nBorderY - (ANSWARE_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0));

					//Answare C
					gSceneTextureC.render(gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);
					gAnswareC.render(nBorderX + gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), nBorderY - (ANSWARE_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0));

					//Answare D
					gSceneTextureD.render(gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);
					gAnswareD.render(nBorderX + gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), nBorderY - (ANSWARE_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0));

					//Update screen
					SDL_RenderPresent(gRenderer);
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}