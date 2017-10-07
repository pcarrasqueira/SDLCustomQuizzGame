#include "CQuestions.h"
#include "pugixml.hpp"

using namespace pugi;

CQuestions::CQuestions()
{
}

CQuestions::~CQuestions()
{
}

void CQuestions::InitializeLogger(shared_ptr<spdlog::logger> &gLogger)
{
	sdpLogger = gLogger;
}

bool CQuestions::LoadQuestionsFromXML(string strXMLPath)
{
	xml_document xmlDoc;
	xml_parse_result result = xmlDoc.load_file(strXMLPath.c_str());

	if (result)
	{
		sdpLogger->info("XML [%s] parsed without errors", strXMLPath);
	}
	else
	{
		sdpLogger->error("XML [%s] parsed with errors", strXMLPath);
		sdpLogger->error("Error description: %s", result.description());
	}


	xml_node nodeQuestions = xmlDoc.child("Questions");
	for (xml_node_iterator it = nodeQuestions.begin(); it != nodeQuestions.end(); ++it)
	{
		for (xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
		{
			sdpLogger->info("Name :  %s , Value : %s",ait->name() ,ait->value());
		}
	}
	return true;
}