#pragma once

#ifndef MESSAGECABLE_H
#define MESSAGECABLE_H

#include <queue>
#include <mutex>
#include "Message.h"

class MessageCable
{
	private:
		std::queue<Message> incomingMessages;	// put responses (for instance, what a server responds to the client with)
		std::mutex incomingQueueGuard;

		std::queue<Message> outgoingMessages;	// put requests (for instance, for a client's request)
		std::mutex outgoingQueueGuard;
	public:
		// incoming message functions
		void insertIncomingMessage(Message in_message)
		{
			std::lock_guard<std::mutex> lock(incomingQueueGuard);
			incomingMessages.push(in_message);
		}

		void popIncomingQueue()
		{
			std::lock_guard<std::mutex> lock(incomingQueueGuard);
			incomingMessages.pop();
		}

		bool isIncomingQueueEmpty()
		{
			std::lock_guard<std::mutex> lock(incomingQueueGuard);
			return incomingMessages.empty();
		}

		Message* getIncomingMessageRefFromFront()
		{
			std::lock_guard<std::mutex> lock(incomingQueueGuard);
			return &incomingMessages.front();
		}



		// outgoing message functions
		void insertOutgoingMessage(Message in_message)
		{
			std::lock_guard<std::mutex> lock(outgoingQueueGuard);
			outgoingMessages.push(in_message);
		}

		void popOutgoingQueue()
		{
			std::lock_guard<std::mutex> lock(outgoingQueueGuard);
			outgoingMessages.pop();
		}

		bool isOutgoingQueueEmpty()
		{
			std::lock_guard<std::mutex> lock(outgoingQueueGuard);
			return outgoingMessages.empty();
		}

		Message* getOutgoingMessageRefFromFront()
		{
			std::lock_guard<std::mutex> lock(outgoingQueueGuard);
			return &outgoingMessages.front();
		}


};

#endif
