#include "CMenuState.h"
#include "CLTexture.h"
#include "CQuestionsState.h"

CMenuState CMenuState::m_MenuState;

bool CMenuState::Init(CQuizzGameEngine* QuizzGameEngine)
{
	bool bret = true;

	m_eState = CQuizzGameStates::MENU_STATE;

	textColor = {0,0,0};

	TTF_Font *lFont = TTF_OpenFont("data//font//kenvector_future_thin.ttf", QUESTION_FONT_SIZE);
	if (lFont == NULL)
	{
		QuizzGameEngine->gMyLogger->error("Failed to load lazy font! SDL_ttf Error: {}", TTF_GetError());
		bret = false;
	}
	else
	{
		if (!gIntroText.loadFromRenderedText("Press space to load demo question", textColor, lFont, QuizzGameEngine->gRenderer))
		{
			QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
			bret = false;
		}
	}
	return bret;
}

void CMenuState::Cleanup()
{
	gIntroText.free();
}

void CMenuState::Pause()
{
	
}

void CMenuState::Resume()
{
	
}

void CMenuState::HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e)
{
	switch (e.type) {
		case SDL_QUIT:
			QuizzGameEngine->Quit();
			break;

		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_SPACE:
				QuizzGameEngine->ChangeState(CQuestionsState::Instance());
				break;
			}
			break;
		}
}

void CMenuState::Update(CQuizzGameEngine* QuizzGameEngine)
{
}

void CMenuState::Render(CQuizzGameEngine* QuizzGameEngine)
{
	//Only draw when not minimized
	if (!QuizzGameEngine->gWindow.isMinimized())
	{
		//Initial screen

		//Clear screen
		SDL_SetRenderDrawColor(QuizzGameEngine->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(QuizzGameEngine->gRenderer);

		

		TTF_Font *lFont = TTF_OpenFont("data//font//kenvector_future_thin.ttf", QUESTION_FONT_SIZE);
		if (lFont == NULL)
		{
			QuizzGameEngine->gMyLogger->error("Failed to load lazy font! SDL_ttf Error: {}", TTF_GetError());
		}
		else
		{
			if (!gIntroText.loadFromRenderedText("Press space to load demo question", textColor, lFont, QuizzGameEngine->gRenderer))
			{
				QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
			}
			else
			{
				int nQBorderX = 0;
				gIntroText.render(QuizzGameEngine->gRenderer, nQBorderX + ((QuizzGameEngine->gWindow.getWidth()) / 2.0) - (gIntroText.getWidth() / 2.0), ((QuizzGameEngine->gWindow.getHeight()) / 2.0) - (QUESTION_FONT_SIZE / 2.0));
			}
		}
	}
}

CQuizzGameStates CMenuState::GetStateEnum()
{
	return m_eState;
}