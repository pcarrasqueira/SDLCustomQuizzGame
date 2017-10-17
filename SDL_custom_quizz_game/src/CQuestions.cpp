#include "CQuestions.h"
#include "pugixml.hpp"

using namespace pugi;

//XML nodes
const char* NODE_QUESTIONS = "Questions";
const char* NODE_QUESTION = "Question";
const char* ATTR_TYPE = "Type";
const char* NODE_QUESTION_TEXT = "QuestionText";
const char* NODE_ANSWER_A = "AnswerA";
const char* NODE_ANSWER_B = "AnswerB";
const char* NODE_ANSWER_C = "AnswerC";
const char* NODE_ANSWER_D = "AnswerD";
const char* NODE_CORRECT_ANSWER = "CorrectAnswer";

CQuestions::CQuestions()
{

}

CQuestions::~CQuestions()
{
	m_vecQuestions.empty();
}

void CQuestions::InitializeLogger(shared_ptr<spdlog::logger> &gLogger)
{
	m_sdpLogger = gLogger;
}

bool CQuestions::LoadQuestionsFromXML(string strXMLPath)
{
	bool bret = false;

	xml_document xmlDoc;
	xml_parse_result result = xmlDoc.load_file(strXMLPath.c_str());

	if (result.status == status_ok)
	{
		m_sdpLogger->info("XML [{}] parsed successfully", strXMLPath);
		
		xml_node nodeQuestions = xmlDoc.child(NODE_QUESTIONS);
		for (xml_node_iterator it = nodeQuestions.begin(); it != nodeQuestions.end(); ++it)
		{
			Question Question;
			//Lets get question type (no need to use iterator here, but maybe in future I will use more attributes so...)
			for (xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
			{
				if (strcmp(ait->name(), ATTR_TYPE) == 0)
				{
					m_sdpLogger->debug("Attribute Name : \"{}\" | Value : \"{}\"", ait->name(), ait->value());
					switch (ait->as_int())
					{
					case 1:
						Question.Type = TEXT_QUESTION;
						break;
					case 2:
						Question.Type = IMAGE_QUESTION;
						break;
					}
				}
			}
			//Let's get question text
			Question.strQuestion = it->child_value(NODE_QUESTION_TEXT);
			m_sdpLogger->debug("Node: \"{}\" | Value : \"{}\"", NODE_QUESTION_TEXT, Question.strQuestion);
			//Let's get answers tex
			Question.strAnswerA = it->child_value(NODE_ANSWER_A);
			m_sdpLogger->debug("Node: \"{}\" | Value : \"{}\"", NODE_ANSWER_A, Question.strAnswerA);
			Question.strAnswerB = it->child_value(NODE_ANSWER_B);
			m_sdpLogger->debug("Node: \"{}\" | Value : \"{}\"", NODE_ANSWER_B, Question.strAnswerB);
			Question.strAnswerC = it->child_value(NODE_ANSWER_C);
			m_sdpLogger->debug("Node: \"{}\" | Value : \"{}\"", NODE_ANSWER_C, Question.strAnswerC);
			Question.strAnswerD = it->child_value(NODE_ANSWER_D);
			m_sdpLogger->debug("Node: \"{}\" | Value : \"{}\"", NODE_ANSWER_D, Question.strAnswerD);
			//Let's get corrent answer index
			char cCorrectAnswer = (char)it->child_value(NODE_CORRECT_ANSWER)[0];
			switch (cCorrectAnswer)
			{
			case 'A':
				Question.CorrectAnswerIndex = BuzzButton::BUZZ_BLUE;
				break;
			case 'B':
				Question.CorrectAnswerIndex = BuzzButton::BUZZ_ORANGE;
				break;
			case 'C':
				Question.CorrectAnswerIndex = BuzzButton::BUZZ_GREEN;
				break;
			case 'D':
				Question.CorrectAnswerIndex = BuzzButton::BUZZ_YELLOW;
				break;
			}
			m_sdpLogger->debug("Node: \"{}\" | Value : \"{}\"", NODE_CORRECT_ANSWER, Question.CorrectAnswerIndex);
			//Put question in vector to use it later			
			m_vecQuestions.push_back(Question);
		}
		bret = true;
	}
	else
	{
		m_sdpLogger->error("XML [{}] parsed with errors", strXMLPath);
		m_sdpLogger->error("Error description: {}", result.description());
	}
	return bret;
}

Question CQuestions::GetQuestionData(bool brand)
{
	int nSize = m_vecQuestions.size();
	Question QuestionOut;

	if (nSize > 0)
	{
		if (brand)
		{
			int nRandIndex = rand() % nSize;
			QuestionOut = m_vecQuestions[nRandIndex];
			m_vecQuestions.erase(m_vecQuestions.begin() + nRandIndex);
		}
		else
		{
			QuestionOut = m_vecQuestions[0];
			m_vecQuestions.erase(m_vecQuestions.begin());
		}
	}
	return QuestionOut;
}

int CQuestions::GetNumberQuestions()
{
	return m_vecQuestions.size();
}