#pragma once

#ifndef MESSAGE
#define MESSAGE

#include "MessageType.h"
#include <vector>

class Message
{
	public:
		Message() {};
		Message(int in_messageID, MessageType in_messageType) : messageID(in_messageID), messageType(in_messageType) {};
		MessageType messageType = MessageType::NOVAL;
		std::vector<int> intVector;
		int messageID = 0;				// set by the constructor.
};

#endif
