#pragma once

#ifndef POLYLOGGER_H
#define POLYLOGGER_H

#include "PolyDebugLevel.h"
#include <iostream>

class PolyLogger
{
private:
	PolyDebugLevel logLevel = PolyDebugLevel::NONE;
public:
	void setDebugLevel(PolyDebugLevel in_debugLevel);
	template<typename First, typename ...Rest> void log(First && first, Rest && ...rest)
	{
		//std::cout << "Hey we should be debugging here..." << std::endl;
		//std::cout << std::endl;
		if (logLevel == PolyDebugLevel::DEBUG)
		{
			//(std::cout << ... << args);
			std::cout << std::forward<First>(first);
			log(std::forward<Rest>(rest)...);
		}
		//std::cout << std::endl;
	}
	void log() {};
	bool isLoggingSet();
	void waitForDebugInput();
	PolyDebugLevel getLogLevel();
};

#endif
