#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "CQuizzGameEngine.h"


class CGameState
{
public:
	virtual bool Init(CQuizzGameEngine* QuizzGameEngine) = 0;
	virtual void Cleanup() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e) = 0;
	virtual void Update(CQuizzGameEngine* QuizzGameEngine) = 0;
	virtual void Render(CQuizzGameEngine* QuizzGameEngine) = 0;

	virtual CQuizzGameStates GetStateEnum() = 0;

	void ChangeState(CQuizzGameEngine* QuizzGameEngine, CGameState* State) {
		QuizzGameEngine->ChangeState(State);
	}


protected:
	CGameState() { }
};

#endif