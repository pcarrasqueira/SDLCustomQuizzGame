#include "CBuzzCommand.h"

CBuzzCommand::CBuzzCommand()
{
	mbIsBuzzInitialized = false;
	sdlGameController = NULL;
	strNameBuzzControllers = "Buzz";
}

CBuzzCommand::~CBuzzCommand()
{

}

void CBuzzCommand::InitializeLogger(shared_ptr<spdlog::logger> &gLogger)
{
	sdpLogger = gLogger;
}

bool CBuzzCommand::InitializeBuzzControllers()
{
	bool breturn = false;

	//Check for joysticks
	int nNumJoysticks = SDL_NumJoysticks();
	if (nNumJoysticks < 1)
	{
		sdpLogger->warn("Warning: No joysticks connected!");
	}
	else
	{
		for (int i = 0; i < nNumJoysticks; i++)
		{
			const char* cNameJoystick = SDL_JoystickNameForIndex(i);
			if (cNameJoystick != NULL)
			{
				string strNameJoystick(cNameJoystick);
				if (strNameJoystick.find(strNameBuzzControllers) >= 0)
				{
					//Load joystick
					sdlGameController = SDL_JoystickOpen(0);
					if (sdlGameController != NULL)
					{
						breturn = true;
						mbIsBuzzInitialized = true;
						sdpLogger->info("Found a Buzz controller");
					}
				}
			}
		}
	}
	return breturn;
}

uint8_t CBuzzCommand::IdentifyBuzzer(uint8_t givenButton)
{
	return givenButton / 5;
}

BuzzButton CBuzzCommand::IdentifyButton(uint8_t givenButton)
{
	return (BuzzButton)(givenButton % 5);
}

void CBuzzCommand::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_JOYBUTTONDOWN)
	{
		SDL_JoyButtonEvent JoystickEvent = e.jbutton;

		//One joystick has 4 buzzers, will get the joystick number
		unsigned int nJoystickIndex = (int)JoystickEvent.which;
		unsigned int nBuzzerJoystickIndex = nJoystickIndex * 4 + IdentifyBuzzer(JoystickEvent.button);

		switch (IdentifyButton(JoystickEvent.button))
		{
			case (BUZZ_RED) : 
			{
				sdpLogger->debug("Pressed Red button");
				break;
			}
			case (BUZZ_YELLOW) :
			{
				sdpLogger->debug("Pressed yellow button");
				break;
			}

			case (BUZZ_GREEN) :
			{
				sdpLogger->debug("Pressed green button");
				break;
			}
			case (BUZZ_ORANGE) :
			{
				sdpLogger->debug("Pressed orande button");
				break;
			}
			case (BUZZ_BLUE) :
			{
				sdpLogger->debug("Pressed blue button");
				break;
			}
			default:
			{
				sdpLogger->debug("Pressed none button");
				break;
			}
		}

	}
}