#include "CQuizzGame.h"
#include "CQuizzGameEngine.h"
#include "CMenuState.h"

void CQuizzGame::Run()
{
	CQuizzGameEngine QuizzGameEngine;

	// initialize the game engine
	if (QuizzGameEngine.Init())
	{
		// load the first state (Menu)
		QuizzGameEngine.ChangeState(CMenuState::Instance());

		// main loop
		while (QuizzGameEngine.Running())
		{
			QuizzGameEngine.HandleEvents();
			QuizzGameEngine.Update();
			QuizzGameEngine.Render();
		}

		// cleanup the game engine
		QuizzGameEngine.Cleanup();
	}
}