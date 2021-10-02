#include "CMenuState.h"
#include "CLTexture.h"
#include "CQuestionsState.h"

CMenuState CMenuState::m_MenuState;

bool CMenuState::Init(CQuizzGameEngine* QuizzGameEngine)
{
	bool bret = true;

	m_eState = CQuizzGameStates::MENU_STATE;

	SDL_Color textColor = { 0, 0, 0 };

	TTF_Font *lFont = TTF_OpenFont("data//font//Ubuntu-R.ttf", QUESTION_FONT_SIZE);
	if (lFont == NULL)
	{
		QuizzGameEngine->gMyLogger->error("Failed to load lazy font! SDL_ttf Error: {}", TTF_GetError());
		bret = false;
	}
	else
	{
		if (QuizzGameEngine->gUsingKeyboard) {
			if (!gIntroText.loadFromRenderedText("Press + button to add player, - to remove player and press space to start quizz", textColor, lFont, QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth() - 2 * (QuizzGameEngine->gWindow.getWidth() / 7.0)))
			{
				QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
				bret = false;
			}
		}
		else {
			if (!gIntroText.loadFromRenderedText("Press red button to add player and press space to start quizz", textColor, lFont, QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth() - 2 * (QuizzGameEngine->gWindow.getWidth() / 7.0)))
			{
				QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
				bret = false;
			}
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
	QuizzGameEngine->BuzzCommand.HandleEvents(QuizzGameEngine, e);
	QuizzGameEngine->KeyboardCommand.HandleEvents(QuizzGameEngine, e);

	switch (e.type) {
		case SDL_QUIT:
			QuizzGameEngine->Quit();
			break;

		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_SPACE:
				if (QuizzGameEngine->QuizzPlayers.GetNumPlayers() > 0)
				{
					QuizzGameEngine->ChangeState(CQuestionsState::Instance());
				}
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
		//Clear screen
		SDL_SetRenderDrawColor(QuizzGameEngine->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(QuizzGameEngine->gRenderer);

		//use TTF_SizeUTF8 to get size of string after rendered
		int nQBorderX = QuizzGameEngine->gWindow.getWidth() / 7.0;
		gIntroText.render(QuizzGameEngine->gRenderer, nQBorderX /*+ ((QuizzGameEngine->gWindow.getWidth()) / 2.0) - (gIntroText.getWidth() / 2.0)*/, ((QuizzGameEngine->gWindow.getHeight()) / 2.0) - (QUESTION_FONT_SIZE / 2.0));
	}
}

CQuizzGameStates CMenuState::GetStateEnum()
{
	return m_eState;
}