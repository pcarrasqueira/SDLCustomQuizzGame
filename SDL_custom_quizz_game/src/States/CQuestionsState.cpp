#include "CQuestionsState.h"
#include "CScoresState.h"

CQuestionsState CQuestionsState::m_QuestionsState;

bool CQuestionsState::loadQuestion(CQuizzGameEngine* QuizzGameEngine, string strQuestionText)
{
	//Loading success flag
	bool success = true;

	//Open the font
	TTF_Font* lFont = TTF_OpenFont("data//font//Ubuntu-R.ttf", QUESTION_FONT_SIZE);
	if (lFont == NULL)
	{
		QuizzGameEngine->gMyLogger->error("Failed to load lazy font! SDL_ttf Error: {}", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if (!m_TextQuestion.loadFromRenderedText(strQuestionText, textColor, lFont, QuizzGameEngine->gRenderer, m_ImageQuestion.getWidth()))
		{
			QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
			success = false;
		}
	}

	return success;
}

bool CQuestionsState::loadAnswers(CQuizzGameEngine* QuizzGameEngine, string strAnswerA, string strAnswerB, string strAnswerC, string strAnswerD)
{
	//Loading success flag
	bool success = true;

	//Open the font
	TTF_Font*  lFont  = TTF_OpenFont("data//font//Ubuntu-R.ttf", ANSWER_FONT_SIZE);
	if (lFont == NULL)
	{
		QuizzGameEngine->gMyLogger->error("Failed to load lazy font! SDL_ttf Error: {}", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };

		if (!m_TextAnswerA.loadFromRenderedText(strAnswerA, textColor, lFont, QuizzGameEngine->gRenderer, m_ImageAnswerA.getWidth()))
		{
			QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
			success = false;
		}
		else if (!m_TextAnswerB.loadFromRenderedText(strAnswerB, textColor, lFont, QuizzGameEngine->gRenderer, m_ImageAnswerB.getWidth()))
		{
			QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
			success = false;
		}
		else if (!m_TextAnswerC.loadFromRenderedText(strAnswerC, textColor, lFont, QuizzGameEngine->gRenderer, m_ImageAnswerC.getWidth()))
		{
			QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
			success = false;
		}
		else if (!m_TextAnswerD.loadFromRenderedText(strAnswerD, textColor, lFont, QuizzGameEngine->gRenderer, m_ImageAnswerD.getWidth()))
		{
			QuizzGameEngine->gMyLogger->error("Failed to render text texture!");
			success = false;
		}
	}

	return success;
}

bool CQuestionsState::Init(CQuizzGameEngine* QuizzGameEngine)
{
	bool bret = true;

	m_eState = CQuizzGameStates::QUESTION_STATE;

	bIsToRender = false;
	bIsToGetNewQuestion = true;

	if (!LoadMedia(QuizzGameEngine))
	{
		bret = false;
	}
	//Initialize and load questions
	Questions.InitializeLogger(QuizzGameEngine->gMyLogger);

	if (!Questions.LoadQuestionsFromXML("data/xml/Questions/empty_questions.xml"))
	{
		QuizzGameEngine->gMyLogger->error("Failed to LoadQuestionsFromXML");
		bret = false;
	}


	//Render text textures
	int myWidth = QuizzGameEngine->gWindow.getWidth();
	int myHeight = 3 * QUESTION_FONT_SIZE;

	if (myHeight > QuizzGameEngine->gWindow.getHeight() / 4.0)
	{
		myHeight = QuizzGameEngine->gWindow.getHeight() / 4.0;
	}

	int ndelta = 15;
	int myWidthA = (QuizzGameEngine->gWindow.getWidth() / 2.0) - ndelta;
	int myHeightA = (QuizzGameEngine->gWindow.getHeight() / 8.0) / 2.0;

	//Question
	m_ImageQuestion.render(QuizzGameEngine->gRenderer, (QuizzGameEngine->gWindow.getWidth() - myWidth) / 2.0, (QuizzGameEngine->gWindow.getHeight()) / 4.0, myWidth, myHeight);

	//Answer A
	m_ImageAnswerA.render(QuizzGameEngine->gRenderer, ((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);

	//Answer C
	m_ImageAnswerC.render(QuizzGameEngine->gRenderer, ((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);

	//Answer B
	m_ImageAnswerB.render(QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth() / 2.0 + (((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);

	//Answer D
	m_ImageAnswerD.render(QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth() / 2.0 + (((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);

	return bret;
}

void CQuestionsState::Cleanup()
{
	//Free loaded images and text
	m_ImageQuestion.free();
	m_ImageAnswerA.free();
	m_ImageAnswerB.free();
	m_ImageAnswerC.free();
	m_ImageAnswerD.free();

	m_TextQuestion.free();
	m_TextAnswerA.free();
	m_TextAnswerB.free();
	m_TextAnswerC.free();
	m_TextAnswerD.free();
}

void CQuestionsState::Pause()
{

}

void CQuestionsState::Resume()
{

}

void CQuestionsState::HandleEvents(CQuizzGameEngine* QuizzGameEngine, SDL_Event& e)
{
	QuizzGameEngine->BuzzCommand.HandleEvents(QuizzGameEngine, e);
	QuizzGameEngine->KeyboardCommand.HandleEvents(QuizzGameEngine, e);

	switch (e.type) {
	case SDL_QUIT:
		QuizzGameEngine->Quit();
		break;
	}
}

void CQuestionsState::Update(CQuizzGameEngine* QuizzGameEngine)
{
	//load question and answers
	if (bIsToGetNewQuestion)
	{
		if (Questions.GetNumberQuestions() > 0)
		{
			ActualQuestion = Questions.GetQuestionData(false);
			if (!loadQuestion(QuizzGameEngine, ActualQuestion.strQuestion))
			{
				QuizzGameEngine->gMyLogger->error("Failed to loadQuestion");
			}
			else if (!loadAnswers(QuizzGameEngine, ActualQuestion.strAnswerA, ActualQuestion.strAnswerB, ActualQuestion.strAnswerC, ActualQuestion.strAnswerD))
			{
				QuizzGameEngine->gMyLogger->error("Failed to loadAnswers");
			}
			bIsToRender = true;
		}
		//Game over, lets show results
		else
		{
			QuizzGameEngine->ChangeState(CScoresState::Instance());
		}
	}

	//check if everyone answered, update points and load new question
	if (QuizzGameEngine->QuizzPlayers.HasEveryoneAnswered())
	{
		QuizzGameEngine->QuizzPlayers.UpdatePlayersScores(ActualQuestion.CorrectAnswerIndex);

		QuizzGameEngine->QuizzPlayers.ClearAnswers();

		bIsToGetNewQuestion = true;
		bIsToRender = false;
	}
}

void CQuestionsState::Render(CQuizzGameEngine* QuizzGameEngine)
{
	//Only draw when not minimized
	if (!QuizzGameEngine->gWindow.isMinimized())
	{
		if (bIsToRender)
		{
			bIsToGetNewQuestion = false;

			//Clear screen
			SDL_SetRenderDrawColor(QuizzGameEngine->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(QuizzGameEngine->gRenderer);

			//Render text textures
			int myWidth = QuizzGameEngine->gWindow.getWidth();
			int myHeight = 3 * QUESTION_FONT_SIZE;

			if (myHeight > QuizzGameEngine->gWindow.getHeight() / 4.0)
			{
				myHeight = QuizzGameEngine->gWindow.getHeight() / 4.0;
			}

			//Question
			m_ImageQuestion.render(QuizzGameEngine->gRenderer, (QuizzGameEngine->gWindow.getWidth() - myWidth) / 2.0, (QuizzGameEngine->gWindow.getHeight()) / 4.0, myWidth, myHeight);

			int nQBorderX = 20;
			int nQBorderY = (m_ImageQuestion.getHeight() / 2.0);

			m_TextQuestion.render(QuizzGameEngine->gRenderer, nQBorderX + ((QuizzGameEngine->gWindow.getWidth() - myWidth) / 2.0) + 50, nQBorderY - (QUESTION_FONT_SIZE ) + ((QuizzGameEngine->gWindow.getHeight()) / 4.0));

			int ndelta = 15;
			int myWidthA = (QuizzGameEngine->gWindow.getWidth() / 2.0) - ndelta;
			int myHeightA = (QuizzGameEngine->gWindow.getHeight() / 8.0) / 2.0;

			//Answer A
			m_ImageAnswerA.render(QuizzGameEngine->gRenderer, ((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);

			int nBorderX = 20;
			int nBorderY = m_ImageAnswerA.getHeight() / 2.0;

			m_TextAnswerA.render(QuizzGameEngine->gRenderer, nBorderX + ((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0, nBorderY - (ANSWER_FONT_SIZE / 2.0) + ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0));

			//Answer C
			m_ImageAnswerC.render(QuizzGameEngine->gRenderer, ((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0, ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);

			m_TextAnswerC.render(QuizzGameEngine->gRenderer, nBorderX + ((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0, nBorderY - (ANSWER_FONT_SIZE / 2.0) + ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0));

			//Answer B
			m_ImageAnswerB.render(QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth() / 2.0 + (((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0), myWidthA, myHeightA);

			m_TextAnswerB.render(QuizzGameEngine->gRenderer, nBorderX + QuizzGameEngine->gWindow.getWidth() / 2.0 + (((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0), nBorderY - (ANSWER_FONT_SIZE / 2.0) + ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 1.0 / 8.0));

			//Answer D
			m_ImageAnswerD.render(QuizzGameEngine->gRenderer, QuizzGameEngine->gWindow.getWidth() / 2.0 + (((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0), ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0), myWidthA, myHeightA);

			m_TextAnswerD.render(QuizzGameEngine->gRenderer, nBorderX + QuizzGameEngine->gWindow.getWidth() / 2.0 + (((QuizzGameEngine->gWindow.getWidth() / 2.0) - myWidthA) / 2.0), nBorderY - (ANSWER_FONT_SIZE / 2.0) + ((QuizzGameEngine->gWindow.getHeight() / 2.0) + myHeightA) *(1 + 4.0 / 8.0));
		}
	}
}

CQuizzGameStates CQuestionsState::GetStateEnum()
{
	return m_eState;
}


bool CQuestionsState::LoadMedia(CQuizzGameEngine* QuizzGameEngine)
{
	//Loading success flag
	bool success = true;

	//Load scene texture
	if (!m_ImageQuestion.loadFromFile("data\\graphics\\ui\\Question.png", QuizzGameEngine->gRenderer))
	{
		QuizzGameEngine->gMyLogger->error("Failed to load window texture!");
		success = false;
	}
	//Load scene texture
	if (!m_ImageAnswerA.loadFromFile("data\\graphics\\ui\\A.png", QuizzGameEngine->gRenderer))
	{
		QuizzGameEngine->gMyLogger->error("Failed to load window texture!");
		success = false;
	}
	//Load scene texture
	if (!m_ImageAnswerB.loadFromFile("data\\graphics\\ui\\B.png", QuizzGameEngine->gRenderer))
	{
		QuizzGameEngine->gMyLogger->error("Failed to load window texture!");
		success = false;
	}
	//Load scene texture
	if (!m_ImageAnswerC.loadFromFile("data\\graphics\\ui\\C.png", QuizzGameEngine->gRenderer))
	{
		QuizzGameEngine->gMyLogger->error("Failed to load window texture!");
		success = false;
	}
	//Load scene texture
	if (!m_ImageAnswerD.loadFromFile("data\\graphics\\ui\\D.png", QuizzGameEngine->gRenderer))
	{
		QuizzGameEngine->gMyLogger->error("Failed to load window texture!");
		success = false;
	}

	return success;
}
