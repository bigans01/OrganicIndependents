#pragma once

#ifndef STRINGTRACER_H
#define STRINGTRACER_H

#include <queue>
#include <string>
#include <iostream>

class StringTracer
{
	public:
		void insertTracingString(std::string in_string)
		{
			tracedStrings.push(in_string);
		}
		void printTrace()
		{
			if (!tracedStrings.empty())
			{
				while (!tracedStrings.empty())
				{
					std::cout << tracedStrings.front() << std::endl;
					tracedStrings.pop();
				}
			}
		}
	private:
		std::queue<std::string> tracedStrings;
};

#endif
