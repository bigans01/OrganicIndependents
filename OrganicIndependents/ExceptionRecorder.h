#pragma once

#ifndef EXCEPTIONRECORDER_H
#define EXCEPTIONRECORDER_H

#include "ExceptionRecord.h"

/*

Description:	can store one or more instances of an ExceptionRecord. Used by OrganicGLWinLib to record exceptions, and used by
				OrganicCoreLib to extract the exceptions and write them to log/other output.

*/

class ExceptionRecorder
{
	public:
		void insertException(ExceptionRecord in_recordToInsert);
		bool doExceptionsExist();
		std::vector<ExceptionRecord> recordedExceptionVector;
	private:
};

#endif
