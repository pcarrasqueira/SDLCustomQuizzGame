#ifndef QUESTIONS_H
#define QUESTIONS_H

#include "spdlog/spdlog.h"
#include <time.h>
#include "CBuzzCommand.h"

using namespace std;

enum QuestionType
{
	TEXT_QUESTION = 1,
	IMAGE_QUESTION
};

struct Question{
	string strQuestion;
	string strAnswerA;
	string strAnswerB;
	string strAnswerC;
	string strAnswerD;
	BuzzButton CorrectAnswerIndex;
	QuestionType Type;
};


class CQuestions
{
public:
	CQuestions();
	~CQuestions();
	void InitializeLogger(shared_ptr<spdlog::logger> &gLogger);
	bool LoadQuestionsFromXML(string strXMLPath);
	Question GetQuestionData(bool brand = false);
	int GetNumberQuestions();

private:
	vector<Question> m_vecQuestions;
	shared_ptr<spdlog::logger> m_sdpLogger;
};
#endif