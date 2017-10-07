#include "spdlog/spdlog.h"

using namespace std;

struct Question{
	string strQuestion;
	string strAnswerA;
	string strAnswerB;
	string strAnswerC;
	string strAnswerD;
	string strImageBase64;
};


class CQuestions
{
public:
	CQuestions();
	~CQuestions();
	void InitializeLogger(shared_ptr<spdlog::logger> &gLogger);
	bool LoadQuestionsFromXML(string strXMLPath);

private:
	vector<Question> vecQuestions;
	shared_ptr<spdlog::logger> sdpLogger;
};