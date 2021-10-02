#ifndef PLAYERS_H
#define PLAYERS_H

#include <string>
#include <vector>
#include "spdlog/spdlog.h"
#include "CBuzzCommand.h"

using namespace std;

static const int CORRECT_ANSWER_POINTS = 10;
static const int INCORRECT_ANSWER_POINTS = 5;

struct Player{
	string strPlayerName;
	int nJoyStickNumber;
	int nBuzzControllerNumber; //aka playerNumber
	BuzzButton AnswerGiven; // 0 - Invalid | 1- D | 2 - C | 3 - B | 4 - A
	bool bPlayed;
	long lScore;
};

class CPlayers
{
public:
	CPlayers();
	~CPlayers();

	void AddPlayer(int n_joystick_index, int n_buzz_index);
	void RemovePlayer(int n_num_player);
	bool PlayerExists(int n_buzz_index);
	void ClearPlayers();
	int GetNumPlayers();

	void ClearAnswers();
	bool HasEveryoneAnswered();
	void SavePlayerAnswer(int n_num_player, BuzzButton Answer);
	int GetNumberOfAnswares();

	void UpdatePlayersScores(BuzzButton CorrectAnswer);
	long GetPlayerScore(int n_player_number);

	void InitializeLogger(shared_ptr<spdlog::logger> &gLogger);

private:
	shared_ptr<spdlog::logger> m_sdpLogger;
	vector<Player> m_vecPlayers;
};
#endif