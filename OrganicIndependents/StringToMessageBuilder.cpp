#include "stdafx.h"
#include "StringToMessageBuilder.h"

MessageBuildResult StringToMessageBuilder::buildMessageFromString(std::string in_string)
{
	MessageBuildResult buildAttempt;
	// loop until we find the first space character.
	bool wasSpaceFound = false;
	int stringSize = in_string.size();		
	int firstSpaceIndex = 0;
	int commandEndIndex = 0;				// the index where the command to check ends; would be equal to firstSpaceIndex -1.
	int firstCommandLength = 0;
	for (int x = 0; x < stringSize; x++)
	{
		if (isspace(in_string.at(x)))
		{
			//std::cout << "Found space at index " << x << std::endl;
			firstSpaceIndex = x;
			commandEndIndex = firstSpaceIndex - 1;
			wasSpaceFound = true;
			break;
		}
		firstCommandLength++;
	}


	//std::cout << "First space index: " << firstSpaceIndex << std::endl;
	//std::cout << "Command end index: " << commandEndIndex << std::endl;


	std::string commandToVerify;
	// if a space character was found, analyze.
	if (wasSpaceFound == true)
	{
		//std::cout << "A space was found..." << std::endl;
		commandToVerify = in_string.substr(0, firstCommandLength);
		
	}
	else if (wasSpaceFound == false)
	{
		commandToVerify = in_string;
	}

	//std::cout << commandToVerify << std::endl;
	if (commandToVerify == "toggleBlockTargetHighlight")
	{
		std::cout << "(StringToMessageBuilder): found message for toggleBlockTargetHighlight " << std::endl;
		buildAttempt.isMessageValid = true;
		buildAttempt.resultingMessage.messageType = MessageType::REQUEST_FROM_CLIENT_TOGGLE_BLOCK_HIGHLIGHTING;
	}

	return buildAttempt;
}