#include "CKeyboardCommad.h"
#include "CQuizzGameEngine.h"
#include "CMenuState.h"

CKeyboardCommad::CKeyboardCommad()
{
}

CKeyboardCommad::~CKeyboardCommad()
{
}

bool CKeyboardCommad::InitializeKeyboard(shared_ptr<spdlog::logger> &gLogger)
{
	if (gLogger != NULL) {
		m_sdpLogger = gLogger;
	}

	return true;
}

void CKeyboardCommad::HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e)
{
	int nTotalPlayers = QuizzGameEngine->QuizzPlayers.GetNumPlayers();
	string caption;

	//switch beetween states
	switch (QuizzGameEngine->GetCurrentState())
	{
	case MENU_STATE:
		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_PLUS:

				if (nTotalPlayers == 2){
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
						"Error",
						"Max number of player withou buzz controllers reached       ",
						NULL);
					m_sdpLogger->debug("Max number of player withou buzz controllers reached");
					break;
				}

				QuizzGameEngine->QuizzPlayers.AddPlayer(1, nTotalPlayers + 1);
				m_sdpLogger->debug("Added Player | keyboard : {} | player number : {} ", 1, nTotalPlayers + 1);

				caption = "Number of players : " + to_string(QuizzGameEngine->QuizzPlayers.GetNumPlayers());
				SDL_SetWindowTitle(QuizzGameEngine->gWindow.GetSDLWindow(), caption.c_str());
				break;

				//remove players
			case SDLK_MINUS:
				QuizzGameEngine->QuizzPlayers.RemovePlayer(nTotalPlayers);
				m_sdpLogger->debug("Removed Player | keyboard : {} | player number : {} ", 1, nTotalPlayers + 1);

				caption = "Number of players : " + to_string(QuizzGameEngine->QuizzPlayers.GetNumPlayers());
				SDL_SetWindowTitle(QuizzGameEngine->gWindow.GetSDLWindow(), caption.c_str());
				break;
			}
		}
		break;
	case QUESTION_STATE:

		caption = "Number of players that answered : " + to_string(QuizzGameEngine->QuizzPlayers.GetNumberOfAnswares());
		SDL_SetWindowTitle(QuizzGameEngine->gWindow.GetSDLWindow(), caption.c_str());

		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
			case SDLK_a:
			case SDLK_b:
			case SDLK_c:
			case SDLK_d:
				if (QuizzGameEngine->QuizzPlayers.PlayerExists(1))
				{
					if (!QuizzGameEngine->QuizzPlayers.HasEveryoneAnswered())
					{
						QuizzGameEngine->QuizzPlayers.SavePlayerAnswer(1, IdentifyAnswaer(e.key.keysym.sym));
						m_sdpLogger->debug("Player 1 answered : {} using keyboard", IdentifyAnswaer(e.key.keysym.sym));
					}
				}
				break;
			case SDLK_1:
			case SDLK_2:
			case SDLK_3:
			case SDLK_4:
				if (QuizzGameEngine->QuizzPlayers.PlayerExists(2))
				{
					if (!QuizzGameEngine->QuizzPlayers.HasEveryoneAnswered())
					{
						QuizzGameEngine->QuizzPlayers.SavePlayerAnswer(2, IdentifyAnswaer(e.key.keysym.sym));
						m_sdpLogger->debug("Player 2 answered : {} using keyboard", IdentifyAnswaer(e.key.keysym.sym));
					}
				}
				break;
			}
		}
		break;
	case RESULTS_STATE:
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_r) {
				QuizzGameEngine->QuizzPlayers.ClearAnswers();
				QuizzGameEngine->QuizzPlayers.ClearPlayers();
				QuizzGameEngine->ChangeState(CMenuState::Instance());
			}
		}
		break;
	}
}

BuzzButton CKeyboardCommad::IdentifyAnswaer(uint8_t key) {

		if (key == SDLK_a || key == SDLK_1) {
			return BuzzButton::BUZZ_BLUE;
		}
		else if (key == SDLK_b || key == SDLK_2) {
			return BuzzButton::BUZZ_ORANGE;
		}
		else if (key == SDLK_c || key == SDLK_3) {
			return BuzzButton::BUZZ_GREEN;
		}
		else if (key == SDLK_d || key == SDLK_4) {
			return BuzzButton::BUZZ_YELLOW;
		}
	}