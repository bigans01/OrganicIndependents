#pragma once

#ifndef ERRORSENSOR_H
#define ERRORSENSOR_H

#include "PrintableErrorStack.h"
#include "ErrorEnum.h"
#include "ErrorUtils.h"
#include <string>

class ErrorSensor
{
	public:
		bool containsErrors = false;
		void insertError(ErrorEnum in_errorEnum)
		{
			sensorErrorStack.insertError(ErrorUtils::getPrintableError(in_errorEnum));
		}
		void insertErrorCustom(ErrorEnum in_errorEnum, std::string in_errorEnumCustomString)
		{
			sensorErrorStack.insertError(ErrorUtils::getPrintableErrorCustom(in_errorEnum, in_errorEnumCustomString));
		}
		void printErrors()
		{
			sensorErrorStack.printErrorStack();
		}
		bool wereErrorsFound()
		{
			bool errorsFound = false;
			if (!sensorErrorStack.errorStack.empty())
			{
				errorsFound = true;
			}
			return errorsFound;
		}
	private:
		PrintableErrorStack sensorErrorStack;


};

#endif
