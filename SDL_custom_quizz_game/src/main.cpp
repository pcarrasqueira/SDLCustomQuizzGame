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
#include "spdlog/spdlog.h"
#include "CBuzzCommand.h"
#include "CQuestions.h"

using namespace spdlog;
using namespace std;

//Font Sizes
const int QUESTION_FONT_SIZE = 36;
const int ANSWER_FONT_SIZE = 24;

const string strNameBuzzControllers = "Buzz";

shared_ptr<spdlog::logger> gMyLogger;

//Globally used font
TTF_Font *gFont = NULL;

//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia();
bool loadQuestion();
//Frees media and shuts down SDL
void close();
//Initialize global Logger
void InitializeLogger(shared_ptr<spdlog::logger> &logger);

//Our custom window
CLWindow gWindow;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Buzz Controller
CBuzzCommand BuzzCommand;


//Questions
CQuestions Questions;

//Scene textures
CLTexture gSceneTexture;
CLTexture gSceneTextureA;
CLTexture gSceneTextureB;
CLTexture gSceneTextureC;
CLTexture gSceneTextureD;

//Scene textures
CLTexture gQuestionText;
CLTexture gAnswerA;
CLTexture gAnswerB;
CLTexture gAnswerC;
CLTexture gAnswerD;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize logger
	InitializeLogger(gMyLogger);

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		gMyLogger->error("SDL could not initialize! SDL Error: {}", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"))
		{
			gMyLogger->error("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		if (!gWindow.init())
		{
			gMyLogger->error("Window could not be created! SDL Error: {}", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = gWindow.createRenderer();
			if (gRenderer == NULL)
			{
				gMyLogger->error("Renderer could not be created! SDL Error: {}", SDL_GetError());
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
					gMyLogger->error("SDL_image could not initialize! SDL_image Error: {}", IMG_GetError());
					success = false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					gMyLogger->error("SDL_ttf could not initialize! SDL_ttf Error: {}", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void InitializeLogger(shared_ptr<spdlog::logger> &logger)
{
	//gMyLogger = basic_logger_mt("basic_logger", "logs/CustomQuizz.log");
	logger = basic_logger_mt("basic_logger", "C:/Users/pcarrasqueira/Documents/Visual Studio 2013/Projects/SDL_custom_quizz_game/Release/logs/CustomQuizz.log",true);
	logger->set_level(level::level_enum::debug);
	if (logger)
	{
		set_pattern("[%Hh:%Mm:%Ss] %v");
		logger->info("****************************************************");
		logger->info("**	   New instance of CustomQuizz Game          **");
		logger->info("****************************************************");
		logger->info("");
		set_pattern("[%Hh:%Mm:%Ss][T:%t][P:%P][%l] -  %v");
	}
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load scene texture
	if (!gSceneTexture.loadFromFile("data\\graphics\\ui\\Question.png", gRenderer))
	{
		gMyLogger->error("Failed to load window texture!");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureA.loadFromFile("data\\graphics\\ui\\A.png", gRenderer))
	{
		gMyLogger->error("Failed to load window texture!");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureB.loadFromFile("data\\graphics\\ui\\B.png", gRenderer))
	{
		gMyLogger->error("Failed to load window texture!");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureC.loadFromFile("data\\graphics\\ui\\C.png", gRenderer))
	{
		gMyLogger->error("Failed to load window texture!");
		success = false;
	}
	//Load scene texture
	if (!gSceneTextureD.loadFromFile("data\\graphics\\ui\\D.png", gRenderer))
	{
		gMyLogger->error("Failed to load window texture!");
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
		gMyLogger->error("Failed to load lazy font! SDL_ttf Error: {}", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if (!gQuestionText.loadFromRenderedText("Qual destas é a resposta certa?", textColor, gFont, gRenderer))
		{
			gMyLogger->error("Failed to render text texture!");
			success = false;
		}
	}

	return success;
}

bool loadAnswers()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("data//font//kenvector_future_thin.ttf", ANSWER_FONT_SIZE);
	if (gFont == NULL)
	{
		gMyLogger->error("Failed to load lazy font! SDL_ttf Error: {}", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };

		if (!gAnswerA.loadFromRenderedText("Resposta A", textColor, gFont, gRenderer))
		{
			gMyLogger->error("Failed to render text texture!");
			success = false;
		}
		else if (!gAnswerB.loadFromRenderedText("Resposta B", textColor, gFont, gRenderer))
		{
			gMyLogger->error("Failed to render text texture!");
			success = false;
		}
		else if (!gAnswerC.loadFromRenderedText("Resposta C", textColor, gFont, gRenderer))
		{
			gMyLogger->error("Failed to render text texture!");
			success = false;
		}
		else if (!gAnswerD.loadFromRenderedText("Resposta D", textColor, gFont, gRenderer))
		{
			gMyLogger->error("Failed to render text texture!");
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
	gAnswerA.free();
	gAnswerB.free();
	gAnswerC.free();
	gAnswerD.free();

	//Close game controller
	SDL_JoystickClose(BuzzCommand.sdlGameController);
	BuzzCommand.sdlGameController = NULL;

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
		gMyLogger->error("Failed to initialize!");
	}
	else
	{

		Questions.InitializeLogger(gMyLogger);

		Questions.LoadQuestionsFromXML("C:/Users/pcarrasqueira/Documents/Visual Studio 2013/Projects/SDL_custom_quizz_game/SDL_custom_quizz_game/data/xml/Questions/questions.xml");

		Question myQuestion = Questions.GetQuestionData();

		if (!BuzzCommand.InitializeBuzzControllers(gMyLogger))
		{
			gMyLogger->error("Failed to initialize Buzz controllers");
		}
		else if (!loadQuestion())
		{
			gMyLogger->error("Failed to load question!");
		}
		else if (!loadAnswers())
		{
			gMyLogger->error("Failed to load answers!");
		}
		//Load media
		else if (!loadMedia())
		{
			gMyLogger->error("Failed to load media!");
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

								//Answer A
								gSceneTextureA.render(gRenderer, ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);

								int nBorderX = 20;
								int nBorderY = gSceneTextureA.getHeight() / 2.0;

								gAnswerA.render(gRenderer, nBorderX + ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, nBorderY - (ANSWER_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0));

								//Answer C
								gSceneTextureC.render(gRenderer, ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);
								gAnswerC.render(gRenderer, nBorderX + ((gWindow.getWidth() / 2.0) - myWidthA) / 2.0, nBorderY - (ANSWER_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0));

								//Answer B
								gSceneTextureB.render(gRenderer, gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);
								gAnswerB.render(gRenderer, nBorderX + gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), nBorderY - (ANSWER_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0));

								//Answer D
								gSceneTextureD.render(gRenderer, gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);
								gAnswerD.render(gRenderer, nBorderX + gWindow.getWidth() / 2.0 + (((gWindow.getWidth() / 2.0) - myWidthA) / 2.0), nBorderY - (ANSWER_FONT_SIZE / 2.0) + ((gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0));

								//Update screen
								SDL_RenderPresent(gRenderer);
							}
						}
					}
					//Handle window events
					gWindow.handleEvent(e);
					//Handle BuzzCommand Events
					BuzzCommand.handleEvent(e);
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
							gMyLogger->error("Failed to load lazy font! SDL_ttf Error: {}", TTF_GetError());
						}
						else
						{
							if (!gIntroText.loadFromRenderedText("Press space to load demo question", textColor, lFont, gRenderer))
							{
								gMyLogger->error("Failed to render text texture!");
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