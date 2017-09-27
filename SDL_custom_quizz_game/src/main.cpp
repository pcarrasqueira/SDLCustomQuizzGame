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

#include <CLTexture.h>
#include <CLWindow.h>

using namespace std;

//Font Sizes
const int QUESTION_FONT_SIZE = 36;
const int ANSWARE_FONT_SIZE = 24;

//Globally used font
TTF_Font *gFont = NULL;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

bool loadQuestion();

//Frees media and shuts down SDL
void close();

//Our custom window
CLWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
CLTexture gSceneTexture;
CLTexture gSceneTextureA;
CLTexture gSceneTextureB;
CLTexture gSceneTextureC;
CLTexture gSceneTextureD;

//Scene textures
CLTexture gQuestionText;
CLTexture gAnswareA;
CLTexture gAnswareB;
CLTexture gAnswareC;
CLTexture gAnswareD;




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
	if (!gSceneTexture.loadFromFile("data\\graphics\\ui\\Question.png", gRenderer))
	{
		printf("Failed to load window texture!\n");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureA.loadFromFile("data\\graphics\\ui\\A.png", gRenderer))
	{
		printf("Failed to load window texture!\n");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureB.loadFromFile("data\\graphics\\ui\\B.png", gRenderer))
	{
		printf("Failed to load window texture!\n");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureC.loadFromFile("data\\graphics\\ui\\C.png", gRenderer))
	{
		printf("Failed to load window texture!\n");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureD.loadFromFile("data\\graphics\\ui\\D.png", gRenderer))
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
	gFont = TTF_OpenFont("data//font//arial.ttf", QUESTION_FONT_SIZE);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if (!gQuestionText.loadFromRenderedText("Qual destas é a resposta certa?", textColor, gFont, gRenderer))
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

		if (!gAnswareA.loadFromRenderedText("Resposta A", textColor, gFont, gRenderer))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
		else if (!gAnswareB.loadFromRenderedText("Resposta B", textColor, gFont, gRenderer))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
		else if (!gAnswareC.loadFromRenderedText("Resposta C", textColor, gFont, gRenderer))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
		else if (!gAnswareD.loadFromRenderedText("Resposta D", textColor, gFont, gRenderer))
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
	bool bGameStart = false;

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
					else if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE))
					{
						if (!bGameStart)
						{
							//load Question
							//Only draw when not minimized
							if (!gWindow.isMinimized())
							{
								bGameStart = true;

								//Clear screen
								SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
								SDL_RenderClear(gRenderer);

								//Render text textures
								int myWidth = gWindow.getWidth();
								int myHeight = 3 * QUESTION_FONT_SIZE;

								if (myHeight > gWindow.getHeight() / 4.0)
								{
									myHeight = gWindow.getHeight() / 4.0;
								}

								//Question
								gSceneTexture.render(gRenderer, (gWindow.getWidth() - myWidth) / 2.0, (gWindow.getHeight()) / 4.0, myWidth, myHeight);

								int nQBorderX = 20;
								int nQBorderY = gSceneTexture.getHeight() / 2.0;

								gQuestionText.render(gRenderer, nQBorderX + ((gWindow.getWidth() - myWidth) / 2.0) + 50, nQBorderY - (QUESTION_FONT_SIZE / 2.0) + ((gWindow.getHeight()) / 4.0));

								int ndelta = 15;
								int myWidthA = (gWindow.getWidth() / 2.0) - ndelta;
								int myHeightA = (gWindow.getHeight() / 8.0) / 2.0;

								//Answare A
								gSceneTextureA.render(gRenderer, ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);

								int nBorderX = 20;
								int nBorderY = gSceneTextureA.getHeight() / 2.0;

								gAnswareA.render(gRenderer, nBorderX + ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, nBorderY - (ANSWARE_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0));

								//Answare C
								gSceneTextureC.render(gRenderer, ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);
								gAnswareC.render(gRenderer, nBorderX + ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, nBorderY - (ANSWARE_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0));

								//Answare B
								gSceneTextureB.render(gRenderer, gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);
								gAnswareB.render(gRenderer, nBorderX + gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), nBorderY - (ANSWARE_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0));

								//Answare D
								gSceneTextureD.render(gRenderer, gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);
								gAnswareD.render(gRenderer, nBorderX + gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), nBorderY - (ANSWARE_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0));

								//Update screen
								SDL_RenderPresent(gRenderer);
							}
						}
					}
					//Handle window events
					gWindow.handleEvent(e);
				}

				//Only draw when not minimized
				if (!gWindow.isMinimized())
				{
					//Initial screen
					if (!bGameStart)
					{
						//Clear screen
						SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
						SDL_RenderClear(gRenderer);

						CLTexture gIntroText;
						SDL_Color textColor = { 0, 0, 0 };

						TTF_Font *lFont = TTF_OpenFont("data//font//kenvector_future_thin.ttf", QUESTION_FONT_SIZE);
						if (lFont == NULL)
						{
							printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
						}
						else
						{
							if (!gIntroText.loadFromRenderedText("Press space to load demo question", textColor, lFont, gRenderer))
							{
								printf("Failed to render text texture!\n");
							}
							else
							{
								int nQBorderX = 0;
								gIntroText.render(gRenderer, nQBorderX + ((gWindow.getWidth()) / 2.0) - (gIntroText.getWidth() / 2.0), ((gWindow.getHeight()) / 2.0) - (QUESTION_FONT_SIZE / 2.0));
							}
						}

					}
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