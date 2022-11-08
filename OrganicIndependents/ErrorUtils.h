#pragma once

#ifndef ERRORUTILS_H
#define ERRORUTILS_H

#include "PrintableError.h"
#include "ErrorEnum.h"

class ErrorUtils
{
	public:
		static PrintableError getPrintableError(ErrorEnum in_enumToFind)
		{
			PrintableError returnError;
			switch(in_enumToFind) 
			{
				case(ErrorEnum::NONE): { returnError.setValues(in_enumToFind, "No error."); break; };
				case(ErrorEnum::TEST): { returnError.setValues(in_enumToFind, "Test error."); break; };
				case(ErrorEnum::CUTLINEWELDER_BAD_TYPICAL_RUN): { returnError.setValues(in_enumToFind, "CutLineWelder::handleTypicalRun entered a while loop."); break; };
				case(ErrorEnum::CUTLINEWELDER_BAD_SLICE_RUN): { returnError.setValues(in_enumToFind, "CutLineWelder::handleSliceRun entered a while loop."); break; };
			}
			return returnError;
		};
		static PrintableError getPrintableErrorCustom(ErrorEnum in_enumToFind, std::string in_customErrorText)
		{
			PrintableError returnError;
			returnError.setValues(in_enumToFind, in_customErrorText);
			return returnError;
		}
};

#endif
