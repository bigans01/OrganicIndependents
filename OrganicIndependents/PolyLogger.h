#pragma once

#ifndef POLYLOGGER_H
#define POLYLOGGER_H

#include "PolyDebugLevel.h"

class PolyLogger
{
	public:
		PolyLogger() {};
		PolyLogger(PolyDebugLevel in_logLevel) :
			logLevel(in_logLevel)
		{};

		void setDebugLevel(PolyDebugLevel in_debugLevel);
		template<typename First, typename ...Rest> void log(First && first, Rest && ...rest)
		{
			if (logLevel == PolyDebugLevel::DEBUG)
			{
				std::cout << std::forward<First>(first);
				log(std::forward<Rest>(rest)...);
			}
		}
		void log() {};
		bool isLoggingSet();
		void waitForDebugInput();
		PolyDebugLevel getLogLevel();
	private:
		PolyDebugLevel logLevel = PolyDebugLevel::NONE;
};

#endif
