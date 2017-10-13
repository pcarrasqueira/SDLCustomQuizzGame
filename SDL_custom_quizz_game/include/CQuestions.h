#include "spdlog/spdlog.h"

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
	int nCorrectAnswerIndex;
	QuestionType Type;
	bool bAlreadyDone;
};


class CQuestions
{
public:
	CQuestions();
	~CQuestions();
	void InitializeLogger(shared_ptr<spdlog::logger> &gLogger);
	bool LoadQuestionsFromXML(string strXMLPath);
	Question GetQuestionData();

private:
	vector<Question> m_vecQuestions;
	shared_ptr<spdlog::logger> m_sdpLogger;
};