#include "CPlayers.h"

CPlayers::CPlayers()
{

}

CPlayers::~CPlayers()
{
	m_vecPlayers.empty();
}

void CPlayers::InitializeLogger(shared_ptr<spdlog::logger> &gLogger)
{
	m_sdpLogger = gLogger;
}


void CPlayers::AddPlayer(int n_joystick_index, int n_buzz_index)
{
	Player PlayerToAdd;
	//Default values
	PlayerToAdd.bPlayed = false;
	PlayerToAdd.AnswerGiven = BUZZ_RED;
	PlayerToAdd.nBuzzControllerNumber = n_buzz_index;
	PlayerToAdd.nJoyStickNumber = n_joystick_index;
	PlayerToAdd.strPlayerName = "Player" + to_string(n_buzz_index);
	PlayerToAdd.lScore = 0;

	m_vecPlayers.push_back(PlayerToAdd);
}

int CPlayers::GetNumPlayers()
{
	return m_vecPlayers.size();
}

void CPlayers::ClearAnswers()
{
	for (std::vector<Player>::iterator it = m_vecPlayers.begin(); it != m_vecPlayers.end(); ++it)
	{
		(it)->AnswerGiven = BUZZ_RED;
		(it)->bPlayed = false;
	}
}

bool CPlayers::PlayerExists(int n_buzz_index)
{
	bool bret = false;

	for (std::vector<Player>::iterator it = m_vecPlayers.begin(); it != m_vecPlayers.end(); ++it)
	{
		if ((it)->nBuzzControllerNumber == n_buzz_index)
		{
			bret = true;
			break;
		}
	}
	return bret;
}

bool CPlayers::HasEveryoneAnswered()
{
	bool bret = true;

	for (std::vector<Player>::iterator it = m_vecPlayers.begin(); it != m_vecPlayers.end(); ++it)
	{
		if ((it)->bPlayed == false || (it)->AnswerGiven == BUZZ_RED)
		{
			bret = false;
			break;
		}
	}
	return bret;
}

int CPlayers::GetNumberOfAnswares()
{
	int nret = 0;

	for (std::vector<Player>::iterator it = m_vecPlayers.begin(); it != m_vecPlayers.end(); ++it)
	{
		if ((it)->bPlayed == true || (it)->AnswerGiven != BUZZ_RED)
		{
			nret++;
		}
	}
	return nret;
}

void CPlayers::SavePlayerAnswer(int n_num_player, BuzzButton Answer)
{
	for (std::vector<Player>::iterator it = m_vecPlayers.begin(); it != m_vecPlayers.end(); ++it)
	{
		if (it->nBuzzControllerNumber == n_num_player)
		{
			//save answer
			it->AnswerGiven = Answer;
			it->bPlayed = true;
			break;
		}
	}
}

void CPlayers::UpdatePlayersScores(BuzzButton CorrectAnswer)
{
	for (std::vector<Player>::iterator it = m_vecPlayers.begin(); it != m_vecPlayers.end(); ++it)
	{
		if ((it)->AnswerGiven == CorrectAnswer)
		{
			(it)->lScore += CORRECT_ANSWER_POINTS;
			m_sdpLogger->debug("Player {} got it right, added {} points | Total points : {}", it->nBuzzControllerNumber, CORRECT_ANSWER_POINTS, it->lScore);
		}
		else
		{
			(it)->lScore -= INCORRECT_ANSWER_POINTS;
			m_sdpLogger->debug("Player {} got it wrong, removed {} points | Total points : {}", it->nBuzzControllerNumber, INCORRECT_ANSWER_POINTS, it->lScore);
		}
	}
}

long CPlayers::GetPlayerScore(int n_player_number)
{
	for (std::vector<Player>::iterator it = m_vecPlayers.begin(); it != m_vecPlayers.end(); ++it)
	{
		if (it->nBuzzControllerNumber == n_player_number)
		{
			return it->lScore;
		}
	}
}