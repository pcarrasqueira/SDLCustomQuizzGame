#ifndef BUZZ_COMMAND_H
#define BUZZ_COMMAND_H
#include <SDL.h>
#include "spdlog/spdlog.h"
#include "BuzzButtons.h"

using namespace std;


class CQuizzGameEngine;

class CBuzzCommand
{
public:
	//Functions
	CBuzzCommand();
	~CBuzzCommand();
	bool InitializeBuzzControllers(shared_ptr<spdlog::logger> &gLogger);
	void HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e);
	uint8_t IdentifyBuzzer(uint8_t JoyButton);
	BuzzButton IdentifyButton(uint8_t JoyButton);
	//Variables
	SDL_Joystick* sdlGameController;

private:
	bool m_bIsBuzzInitialized;
	string m_strNameBuzzControllers;
	shared_ptr<spdlog::logger> m_sdpLogger;
};

#endif