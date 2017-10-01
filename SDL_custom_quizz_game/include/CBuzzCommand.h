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

	CBuzzCommand();
	~CBuzzCommand();
	bool InitializeBuzzControllers();
	void handleEvent(SDL_Event& e);
	uint8_t IdentifyBuzzer(uint8_t JoyButton);
	BuzzButton IdentifyButton(uint8_t JoyButton);
	void InitializeLogger(shared_ptr<spdlog::logger> &gLogger);
	SDL_Joystick* sdlGameController;

private:
	bool mbIsBuzzInitialized;
	string strNameBuzzControllers;
	shared_ptr<spdlog::logger> sdpLogger;

};