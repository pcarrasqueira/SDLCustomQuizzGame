#ifndef SCORES_STATE_H
#define SCORES_STATE_H

#include "CGameState.h"
#include "CLTexture.h"
#include <vector>

class CScoresState : public CGameState
{
public:
	bool Init(CQuizzGameEngine* QuizzGameEngine);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e);
	void Update(CQuizzGameEngine* QuizzGameEngine);
	void Render(CQuizzGameEngine* QuizzGameEngine);

	static CScoresState* Instance() {
		return &m_ScoresState;
	}

	CQuizzGameStates GetStateEnum();

protected:
	CScoresState() { }

private:
	static CScoresState m_ScoresState;
	CQuizzGameStates m_eState;

	CLTexture gScoresPlayer1Text;
	CLTexture gScoresPlayer2Text;
	CLTexture gScoresPlayer3Text;
	CLTexture gScoresPlayer4Text;
};
#endif