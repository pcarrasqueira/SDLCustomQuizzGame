#include <SDL.h>
#include "spdlog/spdlog.h"

using namespace std;

//Texture wrapper class

typedef enum {
	BUZZ_RED = 0,
	BUZZ_YELLOW = 1,
	BUZZ_GREEN = 2,
	BUZZ_ORANGE = 3,
	BUZZ_BLUE = 4
} BuzzButton;

class CBuzzCommand
{
public:
	//Functions
	CBuzzCommand();
	~CBuzzCommand();
	bool InitializeBuzzControllers(shared_ptr<spdlog::logger> &gLogger);
	void handleEvent(SDL_Event& e);
	uint8_t IdentifyBuzzer(uint8_t JoyButton);
	BuzzButton IdentifyButton(uint8_t JoyButton);
	//Variables
	SDL_Joystick* sdlGameController;

private:
	bool m_bIsBuzzInitialized;
	string m_strNameBuzzControllers;
	shared_ptr<spdlog::logger> m_sdpLogger;

};