#include "stdafx.h"
#include "ExceptionRecorder.h"

void ExceptionRecorder::insertException(ExceptionRecord in_recordToInsert)
{
	recordedExceptionVector.push_back(in_recordToInsert);
}

bool ExceptionRecorder::doExceptionsExist()
{
	bool exceptionsExist = false;
	if (recordedExceptionVector.empty() == false)
	{
		exceptionsExist = true;
	}
	return exceptionsExist;
}

bool ExceptionRecorder::doesSpecificExceptionExist(ExceptionRecordType in_typeToFind)
{
	bool specificExceptionFound = false;
	for (auto& currentException : recordedExceptionVector)
	{
		if (currentException.getExceptionType() == in_typeToFind)
		{
			specificExceptionFound = true;
			break;
		}
	}

	return specificExceptionFound;
}