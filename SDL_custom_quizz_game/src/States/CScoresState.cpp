#include "CScoresState.h"
#include "CLTexture.h"

CScoresState CScoresState::m_ScoresState;

const int MAX_PLAYERS = 4;

bool CScoresState::Init(CQuizzGameEngine* QuizzGameEngine)
{
	bool bret = true;

	m_eState = CQuizzGameStates::RESULTS_STATE;

	SDL_Color textColor = { 0, 0, 0 };

	TTF_Font *lFont = TTF_OpenFont("data//font//Ubuntu-R.ttf", QUESTION_FONT_SIZE);
	if (lFont == NULL)
	{
		QuizzGameEngine->gMyLogger->error("Failed to load lazy font! SDL_ttf Error: {}", TTF_GetError());
		bret = false;
	}
	else
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			if (QuizzGameEngine->QuizzPlayers.PlayerExists(i + 1))
			{
				string strText = "Player " + to_string(i + 1) + ": " + to_string(QuizzGameEngine->QuizzPlayers.GetPlayerScore(i + 1));
				
				if (i == 0 && !gScoresPlayer1Text.loadFromRenderedText(strText, textColor, lFont, QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth()))
				{
					QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
					bret = false;
				}
				else if (i == 1 && !gScoresPlayer2Text.loadFromRenderedText(strText, textColor, lFont, QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth()))
				{
					QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
					bret = false;
				}
				else if (i == 2 && !gScoresPlayer3Text.loadFromRenderedText(strText, textColor, lFont, QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth()))
				{
					QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
					bret = false;
				}
				else if (i == 3 && !gScoresPlayer4Text.loadFromRenderedText(strText, textColor, lFont, QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth()))
				{
					QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
					bret = false;
				}
			}
		}

	}
	return bret;
}

void CScoresState::Cleanup()
{
	gScoresPlayer1Text.free();
	gScoresPlayer2Text.free();
	gScoresPlayer3Text.free();
	gScoresPlayer4Text.free();
}

void CScoresState::Pause()
{
	
}

void CScoresState::Resume()
{
	
}

void CScoresState::HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e)
{
	QuizzGameEngine->BuzzCommand.HandleEvents(QuizzGameEngine, e);
	switch (e.type) {
		case SDL_QUIT:
			QuizzGameEngine->Quit();
			break;
		}
}

void CScoresState::Update(CQuizzGameEngine* QuizzGameEngine)
{
}

void CScoresState::Render(CQuizzGameEngine* QuizzGameEngine)
{
	//Only draw when not minimized
	if (!QuizzGameEngine->gWindow.isMinimized())
	{
		//Initial screen

		//Clear screen
		SDL_SetRenderDrawColor(QuizzGameEngine->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(QuizzGameEngine->gRenderer);

		int nLasty = ((QuizzGameEngine->gWindow.getHeight()) / 3.0) - (QUESTION_FONT_SIZE / 3.0);

		if (gScoresPlayer1Text.getWidth() > 0)
		{
			gScoresPlayer1Text.render(QuizzGameEngine->gRenderer, ((QuizzGameEngine->gWindow.getWidth()) / 2.0) - (gScoresPlayer1Text.getWidth() / 2.0), nLasty);
			nLasty += gScoresPlayer1Text.getHeight();
		}
		if (gScoresPlayer2Text.getWidth() > 0)
		{
			gScoresPlayer2Text.render(QuizzGameEngine->gRenderer, ((QuizzGameEngine->gWindow.getWidth()) / 2.0) - (gScoresPlayer2Text.getWidth() / 2.0), nLasty);
			nLasty += gScoresPlayer2Text.getHeight();
		}
		if (gScoresPlayer3Text.getWidth() > 0)
		{
			gScoresPlayer3Text.render(QuizzGameEngine->gRenderer, ((QuizzGameEngine->gWindow.getWidth()) / 2.0) - (gScoresPlayer3Text.getWidth() / 2.0), nLasty);
			nLasty += gScoresPlayer3Text.getHeight();
		}
		if (gScoresPlayer4Text.getWidth() > 0)
		{
			gScoresPlayer4Text.render(QuizzGameEngine->gRenderer, ((QuizzGameEngine->gWindow.getWidth()) / 2.0) - (gScoresPlayer4Text.getWidth() / 2.0), nLasty);
			nLasty += gScoresPlayer4Text.getHeight();
		}	
	}
}

CQuizzGameStates CScoresState::GetStateEnum()
{
	return m_eState;
}