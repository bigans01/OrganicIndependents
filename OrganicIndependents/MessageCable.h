#pragma once

#ifndef MESSAGECABLE_H
#define MESSAGECABLE_H

#include <queue>
#include "Message.h"

class MessageCable
{
	public:	
		std::queue<Message> outgoingMessages;	// put requests (for instance, for a client's request)
		std::queue<Message> incomingMessages;	// put responses (for instance, what a server responds to the client with)
};

#endif
