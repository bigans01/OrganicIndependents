#include "stdafx.h"
#include "ExceptionRecord.h"

Message ExceptionRecord::getExceptionMessageCopy()
{
	return exceptionMessage;
}

std::vector<std::string> ExceptionRecord::transformRecordToStrings()
{
	// the vector where the converted output would go to.
	std::vector<std::string> returnStrings;

	// this will consist of several nested switch/case statements.
	switch (exceptionType)
	{
		// Used in function CleaveSequenceFactory::handleScenarioTypical. (OrganicGLWinLib)
		case ExceptionRecordType::EXCEPTION_INVALID_TYPICAL_SEQUENCE:
		{
			switch (exceptionMessage.messageType)
			{
				// A Message of this type will be generated in CleaveSequenceFactory::writeOutCategorizedLines, in OrganicGLWinLib.
				case MessageType::CLEAVESEQUENCE_CATEGORIZED_LINES: { writeOutCategorizedLines(&returnStrings); break; }
			};
			break;
		};

		// Used in function WeldedTriangleGroupBuilder::handleFinalObservation() (OrganicGLWinLib)
		case ExceptionRecordType::EXCEPTION_WELDED_TRIANGLE_SHIFT_LINES_EXCEEDED: { writeOutWeldedTriangleShiftLinesExceeded(&returnStrings); break; }
		case ExceptionRecordType::EXCEPTION_TERMINATION_ATTEMPTS_EXCEEDED: { writeOutExcessiveTerminatingLines(&returnStrings); break; }
	}

	// the return vector that contains our output.
	return returnStrings;
}

void ExceptionRecord::writeOutCategorizedLines(std::vector<std::string>* in_outVectorRef)
{
	// Remember, open the message first.
	exceptionMessage.open();

	// read the initial string.
	auto initialDebugString = exceptionMessage.readString();
	in_outVectorRef->push_back(initialDebugString);

	// read the totalManagerLines value.
	int totalManagerLines = exceptionMessage.readInt();
	int totalSequenceLines = exceptionMessage.readInt();

	// read the line manager header string, then read strings a number of times equal to totalManagerLines.
	std::string lineManagerHeader = exceptionMessage.readString();
	in_outVectorRef->push_back(lineManagerHeader);
	for (int x = 0; x < totalManagerLines; x++)
	{
		in_outVectorRef->push_back(exceptionMessage.readString());
	}

	// now, read the sequence lines header, then read strings a number of times equal to totalSequenceLines.
	std::string cleaveSequenceHeader = exceptionMessage.readString();
	in_outVectorRef->push_back(cleaveSequenceHeader);
	for (int x = 0; x < totalSequenceLines; x++)
	{
		in_outVectorRef->push_back(exceptionMessage.readString());
	}	
}

void ExceptionRecord::writeOutWeldedTriangleShiftLinesExceeded(std::vector<std::string>* in_outVectorRef)
{
	// Remember, open the message first.
	exceptionMessage.open();

	// there should only be two context strings; simply read and push back.
	in_outVectorRef->push_back(exceptionMessage.readString());
	in_outVectorRef->push_back(exceptionMessage.readString());
}

void ExceptionRecord::writeOutExcessiveTerminatingLines(std::vector<std::string>* in_outVectorRef)
{
	// Remember, open the message first.
	exceptionMessage.open();

	// First two strings are context strings.
	in_outVectorRef->push_back(exceptionMessage.readString());
	in_outVectorRef->push_back(exceptionMessage.readString());

	// Read the context string for the original lines.
	in_outVectorRef->push_back(exceptionMessage.readString());

	// Read the next int, to get the number of original lines to read.
	int numberOfOriginalLinesToRead = exceptionMessage.readInt();
	for (int x = 0; x < numberOfOriginalLinesToRead; x++)
	{
		in_outVectorRef->push_back(exceptionMessage.readString());
	}

	// Read the context string final lines:
	in_outVectorRef->push_back(exceptionMessage.readString());

	// Read the next int, to get the number of remaining lines to read.
	int numberOfRemainingLinesToRead = exceptionMessage.readInt();
	for (int x = 0; x < numberOfRemainingLinesToRead; x++)
	{
		in_outVectorRef->push_back(exceptionMessage.readString());
	}
}