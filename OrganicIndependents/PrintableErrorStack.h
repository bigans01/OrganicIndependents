#pragma once

#ifndef PRINTABLEERRORSTACK_H
#define PRINTABLEERRORSTACK_H

#include <stack>
#include <iostream>
#include "PrintableError.h"

class PrintableErrorStack
{
	private:
		void insertError(PrintableError in_errorToInsert)
		{
			errorStack.push(in_errorToInsert);
		}
		void printErrorStack()
		{
			while (!errorStack.empty())
			{
				PrintableError currentError = errorStack.top();
				std::cout << currentError;
				errorStack.pop();
			}
		};
		friend class ErrorSensor;
		std::stack<PrintableError> errorStack;

};

#endif
