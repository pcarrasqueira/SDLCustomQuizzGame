#ifndef QUIZZ_GAME_ENGINE_H
#define QUIZZ_GAME_ENGINE_H

#include <vector>
#include "spdlog/spdlog.h"
#include <CLWindow.h>
#include <SDL_ttf.h>
#include "CPlayers.h"
#include "CBuzzCommand.h"

using namespace spdlog;
using namespace std;

//Global Variables
static const int QUESTION_FONT_SIZE = 36;
static const int ANSWER_FONT_SIZE = 24;

typedef enum {
	INVALID_STATE = -1,
	MENU_STATE = 0,
	QUESTION_STATE = 1,
	RESULTS_STATE = 2
} CQuizzGameStates;


class CGameState;

class CQuizzGameEngine
{
public:
	bool Init();
	void Cleanup();

	void ChangeState(CGameState* state);
	void PushState(CGameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Render();
	void SetCurrentState();

	//inline functions
	bool Running() { return m_bRunning; }
	void Quit() { m_bRunning = false; }
	CQuizzGameStates GetCurrentState() { return m_eCurrentState; }

	//Buzz Controller
	CBuzzCommand BuzzCommand;

	//Quizz Players
	CPlayers QuizzPlayers;

	//Globally used logger
	shared_ptr<spdlog::logger> gMyLogger;
	//Globally used font
	TTF_Font *gFont = NULL;
	//Globally used Window
	CLWindow gWindow;
	//Globally used Renderer
	SDL_Renderer* gRenderer = NULL;


protected:
	//Initialize global Logger
	void InitializeLogger(shared_ptr<spdlog::logger> &logger);

	// the stack of states
	vector<CGameState*> m_vecStates;
	bool m_bRunning;
	CQuizzGameStates m_eCurrentState;
};
#endif