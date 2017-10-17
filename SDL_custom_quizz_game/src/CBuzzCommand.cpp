#include "CBuzzCommand.h"
#include "CQuizzGameEngine.h"

CBuzzCommand::CBuzzCommand()
{
	m_bIsBuzzInitialized = false;
	sdlGameController = NULL;
	m_strNameBuzzControllers = "Buzz";
}

CBuzzCommand::~CBuzzCommand()
{

}

bool CBuzzCommand::InitializeBuzzControllers(shared_ptr<spdlog::logger> &gLogger)
{
	bool breturn = false;

	if (gLogger != NULL)
	{
		m_sdpLogger = gLogger;
	}
	
	//Check for joysticks
	int nNumJoysticks = SDL_NumJoysticks();
	if (nNumJoysticks < 1)
	{
		m_sdpLogger->warn("Warning: No joysticks connected!");
	}
	else
	{
		for (int i = 0; i < nNumJoysticks; i++)
		{
			const char* cNameJoystick = SDL_JoystickNameForIndex(i);
			if (cNameJoystick != NULL)
			{
				string strNameJoystick(cNameJoystick);
				if (strNameJoystick.find(m_strNameBuzzControllers) >= 0)
				{
					//Load joystick
					sdlGameController = SDL_JoystickOpen(0);
					if (sdlGameController != NULL)
					{
						breturn = true;
						m_bIsBuzzInitialized = true;
						m_sdpLogger->info("Found a Buzz controller");
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

void CBuzzCommand::HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e)
{
	//switch beetween states
	switch (QuizzGameEngine->GetCurrentState())
	{
	case MENU_STATE:
		if (e.type == SDL_JOYBUTTONDOWN)
		{
			SDL_JoyButtonEvent JoystickEvent = e.jbutton;

			//One joystick has 4 buzzers, will get the joystick number
			unsigned int nJoystickIndex = (int)JoystickEvent.which;
			unsigned int nBuzzCommandNumber = (nJoystickIndex * 4 + IdentifyBuzzer(JoystickEvent.button)) + 1;

			switch (IdentifyButton(JoystickEvent.button))
			{
			case (BUZZ_RED) :
			{
				if (!QuizzGameEngine->QuizzPlayers.PlayerExists(nBuzzCommandNumber))
				{
					QuizzGameEngine->QuizzPlayers.AddPlayer(nJoystickIndex, nBuzzCommandNumber);
					m_sdpLogger->debug("Added Player | joystick : {} | buzzer : {} ", nJoystickIndex, nBuzzCommandNumber);

					string caption = "Number of players : " + to_string(QuizzGameEngine->QuizzPlayers.GetNumPlayers());
					SDL_SetWindowTitle(QuizzGameEngine->gWindow.GetSDLWindow(), caption.c_str());
				}
				break;
			}
			}
		}
		break;
	case QUESTION_STATE:

		string caption = "Number of players that answered : " + to_string(QuizzGameEngine->QuizzPlayers.GetNumberOfAnswares());
		SDL_SetWindowTitle(QuizzGameEngine->gWindow.GetSDLWindow(), caption.c_str());

		if (e.type == SDL_JOYBUTTONDOWN)
		{
			SDL_JoyButtonEvent JoystickEvent = e.jbutton;

			//One joystick has 4 buzzers, will get the joystick number
			unsigned int nJoystickIndex = (int)JoystickEvent.which;
			unsigned int nBuzzCommandNumber = (nJoystickIndex * 4 + IdentifyBuzzer(JoystickEvent.button)) + 1;

			BuzzButton ButtonPressed = IdentifyButton(JoystickEvent.button);

			if (ButtonPressed != BUZZ_RED && QuizzGameEngine->QuizzPlayers.PlayerExists(nBuzzCommandNumber))
			{
				if (!QuizzGameEngine->QuizzPlayers.HasEveryoneAnswered())
				{
					QuizzGameEngine->QuizzPlayers.SavePlayerAnswer(nBuzzCommandNumber, ButtonPressed);
					m_sdpLogger->debug("Player nBuzzCommandNumber answered : {} ", ButtonPressed);
				}
			}
		}

		break;
	}

}