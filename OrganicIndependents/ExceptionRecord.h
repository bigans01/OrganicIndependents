#pragma once

#ifndef EXCEPTIONRECORD_H
#define EXCEPTIONRECORD_H

#include "Message.h"
#include "ExceptionRecordType.h"
#include <vector>
#include <iostream>

/*

Description:  An ExceptionRecord is designed to store any exception that occurs during runtime, so that it may later be analyzed and/or
			  later recorded into a log. It does not handle the exception; rather, it simply serves as a means to record one.
			  The transformRecordToStrings() function will make different function calls based on the value of
			  the ExceptionRecordType and/or the Message type of the exceptionMessage member. The ExceptionRecord is not responsible for making 
			  sure that the Messages are formatted correctly; proper Message structure needs to be done by the developer

			  The return value of transformRecordToStrings() goes through several switch statements to determine the strings to produce. 
			  These strings can then be used in logging.
*/

class ExceptionRecord
{
	public:
		ExceptionRecord() {};

		// constructor for specifying an ExceptionRecordType ONLY.
		ExceptionRecord(ExceptionRecordType in_exceptionRecordType) :
			exceptionType(in_exceptionRecordType)
		{};

		// constructor for specifying an ExceptionRecordType AND a Message.
		ExceptionRecord(ExceptionRecordType in_exceptionRecordType, Message in_exceptionMessage) :
			exceptionType(in_exceptionRecordType),
			exceptionMessage(in_exceptionMessage),
			doesExceptionContainMessage(true)
		{};
		Message getExceptionMessageCopy();						// returns a copy of exceptionMessage.
		std::vector<std::string> transformRecordToStrings();	// transforms the records to strings that can be used in output (i.e, writing to a file)
	private:
		ExceptionRecordType exceptionType = ExceptionRecordType::NOVAL;	// set by either constructor
		bool doesExceptionContainMessage = false;	// gets set to true when the 2nd constructor above gets called.
		Message exceptionMessage;					// optional Message

		// Functions for producing string output; these should all insert into the passed in std::vector reference.
		// There should be 1 function for each specific case.
		void writeOutCategorizedLines(std::vector<std::string>* in_outVectorRef);					// for ExceptionRecordType::EXCEPTION_INVALID_TYPICAL_SEQUENCE
		void writeOutWeldedTriangleShiftLinesExceeded(std::vector<std::string>* in_outVectorRef);	// for ExceptionRecordType::EXCEPTION_WELDED_TRIANGLE_SHIFT_LINES_EXCEEDED

};

#endif
