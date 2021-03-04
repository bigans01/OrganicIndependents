#pragma once

#ifndef PRINTABLEERROR_H
#define PRINTABLEERROR_H

#include "ErrorEnum.h"
#include <iostream>
#include <string>

class PrintableError
{
	public:
		PrintableError(ErrorEnum in_errorEnumValue, std::string in_errorDescription) :
			errorValue(in_errorEnumValue),
			errorDescription(in_errorDescription)
		{};
		PrintableError() {};
		void setValues(ErrorEnum in_errorEnumValue, std::string in_errorDescription)
		{
			errorValue = in_errorEnumValue;
			errorDescription = in_errorDescription;
		};

		// output operator
		friend std::ostream& operator<<(std::ostream& out, const PrintableError& set_b)
		{		
			const char* outputChar = 0;
			
			#define	PROCESS_ERROR_TEXT(p) case(p): outputChar = #p; break;
			//#define	PROCESS_ERROR_TEXT(p) outputChar = #p;
			switch (set_b.errorValue)
			{
				PROCESS_ERROR_TEXT(ErrorEnum::NONE);
				PROCESS_ERROR_TEXT(ErrorEnum::TEST);
			}
			#undef	PROCESS_ERROR_TEXT
			std::string stringedOutput(outputChar);
			stringedOutput.erase(0, 11);

			return out << stringedOutput << " > " << set_b.errorDescription << std::endl;
		}

		std::string errorDescription = "";
		ErrorEnum errorValue = ErrorEnum::NONE;
};

#endif
