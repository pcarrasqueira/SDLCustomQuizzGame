#ifndef KEYBOARD_COMMAND_H
#define KEYBOARD_COMMAND_H
#include <SDL.h>
#include "spdlog/spdlog.h"
#include "BuzzButtons.h"

using namespace std;

class CQuizzGameEngine;

class CKeyboardCommad
{
public:
	//Functions
	CKeyboardCommad();
	~CKeyboardCommad();
	bool InitializeKeyboard(shared_ptr<spdlog::logger> &gLogger);
	void HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e);

private:
	shared_ptr<spdlog::logger> m_sdpLogger;
	BuzzButton IdentifyAnswaer(uint8_t key);
};

#endif