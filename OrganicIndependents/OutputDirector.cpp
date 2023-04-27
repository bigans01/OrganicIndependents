#include "stdafx.h"
#include "OutputDirector.h"

void OutputDirector::setOutputMode(OutputDirectorMode in_outputMode)
{
	outputMode = in_outputMode;
}

void OutputDirector::analyzedAndInsertString(std::string in_stringToAnalyze)
{
	bool containsNewLine = false;
	std::string lastTwo = in_stringToAnalyze.substr(in_stringToAnalyze.size() - 1);
	if (in_stringToAnalyze.size() >= 1)
	{
		if (lastTwo == "\n")
		{
			containsNewLine = true;
		}

		// If we found a new line at the end of the string, we will append everything before it to
		if (containsNewLine)
		{
			// popback only once to remove the newline
			in_stringToAnalyze.pop_back();

			// if the remaining length of in_stringToAnalyze is not 0, append.
			if (in_stringToAnalyze.size() != 0)
			{
				currentStringToWrite += in_stringToAnalyze;
			}
			recordedOutput.push_back(currentStringToWrite);
			currentStringToWrite = "";	// reset the value back.
		}

		// If there was no new line char, just append.
		else
		{
			currentStringToWrite += in_stringToAnalyze;
		}
	}
}

std::vector<std::string> OutputDirector::getRecordedOutput()
{
	// check if there's any remaining output to write, left in currentStringToWrite.
	if (currentStringToWrite != "")
	{
		recordedOutput.push_back(currentStringToWrite);
		currentStringToWrite = "";
	}

	return recordedOutput;
}
