#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "CGameState.h"
#include "CLTexture.h"

class CMenuState : public CGameState
{
public:
	bool Init(CQuizzGameEngine* QuizzGameEngine);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e);
	void Update(CQuizzGameEngine* QuizzGameEngine);
	void Render(CQuizzGameEngine* QuizzGameEngine);

	static CMenuState* Instance() {
		return &m_MenuState;
	}

	CQuizzGameStates GetStateEnum();

protected:
	CMenuState() { }

private:
	static CMenuState m_MenuState;
	CQuizzGameStates m_eState;

	CLTexture gIntroText;
};
#endif