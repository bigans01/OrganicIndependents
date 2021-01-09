#include "stdafx.h"
#include "PolyLogger.h"

void PolyLogger::setDebugLevel(PolyDebugLevel in_debugLevel)
{
	logLevel = in_debugLevel;
}

bool PolyLogger::isLoggingSet()
{
	bool returnResult = false;
	if (logLevel != PolyDebugLevel::NONE)
	{
		returnResult = true;
	}
	return returnResult;
}

void PolyLogger::waitForDebugInput()
{
	if (logLevel == PolyDebugLevel::DEBUG)
	{
		int someVal = 3;
		std::cin >> someVal;
	}
}