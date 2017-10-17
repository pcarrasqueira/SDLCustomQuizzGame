#ifndef QUESTIONS_STATE_H
#define QUESTIONS_STATE_H

#include "CGameState.h"
#include "CLTexture.h"
#include "CQuestions.h"

class CQuestionsState : public CGameState
{
public:
	bool Init(CQuizzGameEngine* QuizzGameEngine);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e);
	void Update(CQuizzGameEngine* QuizzGameEngine);
	void Render(CQuizzGameEngine* QuizzGameEngine);

	static CQuestionsState* Instance() {
		return &m_QuestionsState;
	}

	CQuizzGameStates GetStateEnum();

protected:
	CQuestionsState() { }

private:
	static CQuestionsState m_QuestionsState;
	CQuizzGameStates m_eState;

	//Image textures for question and answeres
	CLTexture m_ImageQuestion;
	CLTexture m_ImageAnswerA;
	CLTexture m_ImageAnswerB;
	CLTexture m_ImageAnswerC;
	CLTexture m_ImageAnswerD;

	//Text for question and answeres
	CLTexture m_TextQuestion;
	CLTexture m_TextAnswerA;
	CLTexture m_TextAnswerB;
	CLTexture m_TextAnswerC;
	CLTexture m_TextAnswerD;

	bool bIsToRender;
	bool bIsToGetNewQuestion;

	Question ActualQuestion;

	bool LoadMedia(CQuizzGameEngine* QuizzGameEngine);
	bool loadQuestion(CQuizzGameEngine* QuizzGameEngine, string strQuestionText);
	bool loadAnswers(CQuizzGameEngine* QuizzGameEngine, string strAnswerA, string strAnswerB, string strAnswerC, string strAnswerD);

	CQuestions Questions;
};
#endif