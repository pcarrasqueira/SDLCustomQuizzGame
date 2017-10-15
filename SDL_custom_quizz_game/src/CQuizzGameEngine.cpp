#include "CQuizzGameEngine.h"
#include "CGameState.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>


void CQuizzGameEngine::InitializeLogger(shared_ptr<spdlog::logger> &logger)
{
	//gMyLogger = basic_logger_mt("basic_logger", "logs/CustomQuizz.log");
	logger = basic_logger_mt("basic_logger", "C:/Users/pcarrasqueira/Documents/Visual Studio 2013/Projects/SDL_custom_quizz_game/Release/logs/CustomQuizz.log", true);
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

//Start up SDL and create window
bool CQuizzGameEngine::Init()
{
	//Initialize random seed
	srand(time(NULL));

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

	m_bRunning = true;
	return success;
}

void CQuizzGameEngine::Cleanup()
{
	// cleanup the all states
	while (!m_vecStates.empty()) {
		m_vecStates.back()->Cleanup();
		m_vecStates.pop_back();
	}

	//release all loggers
	drop_all();


	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	gWindow.free();

	//Quit SDL subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void CQuizzGameEngine::ChangeState(CGameState* state)
{
	// cleanup the current state
	if (!m_vecStates.empty()) {
		m_vecStates.back()->Cleanup();
		m_vecStates.pop_back();
	}

	// store and init the new state
	m_vecStates.push_back(state);

	if (m_vecStates.back()->Init(this))
	{

		//update current state
		m_eCurrentState = state->GetStateEnum();
	}
	else
	{
		m_bRunning = false;
		gMyLogger->error("Failed to Initialize state number :  {}", state->GetStateEnum());
	}
}

void CQuizzGameEngine::PushState(CGameState* state)
{
	// pause current state
	if (!m_vecStates.empty()) {
		m_vecStates.back()->Pause();
	}
	// store and init the new state
	m_vecStates.push_back(state);

	if (m_vecStates.back()->Init(this))
	{

		//update current state
		m_eCurrentState = state->GetStateEnum();
	}
	else
	{
		m_bRunning = false;
		gMyLogger->error("Failed to Initialize state number :  {}", state->GetStateEnum());
	}
}

void CQuizzGameEngine::PopState()
{
	// cleanup the current state
	if (!m_vecStates.empty()) {
		m_vecStates.back()->Cleanup();
		m_vecStates.pop_back();
	}
	// resume previous state
	if (!m_vecStates.empty()) {
		m_vecStates.back()->Resume();
	}
}

void CQuizzGameEngine::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		//handle hindow events
		gWindow.HandleEvent(event);
		// let the state handle events
		m_vecStates.back()->HandleEvents(this,event);
	}
}

void CQuizzGameEngine::Update()
{
	// let the state update the game
	m_vecStates.back()->Update(this);
}

void CQuizzGameEngine::Render()
{
	// let the state draw the screen
	m_vecStates.back()->Render(this);
	//Update screen
	SDL_RenderPresent(gRenderer);
}