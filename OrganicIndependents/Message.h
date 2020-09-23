#pragma once

#ifndef MESSAGE
#define MESSAGE

#include "MessageType.h"
#include <vector>

class Message
{
	public:
		Message(MessageType in_messageType) : messageType(in_messageType) {};
		MessageType messageType = MessageType::NOVAL;
		std::vector<int> intVector;
};

#endif
