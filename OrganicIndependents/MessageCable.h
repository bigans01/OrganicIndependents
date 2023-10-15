#pragma once

#ifndef MESSAGECABLE_H
#define MESSAGECABLE_H

#include "MessageContainer.h"

/*

Summary: this class contains incoming/outgoing queues of Message and MessageContainer instances;
it is designed to serve as a means for objects such as the OrganicServer and OrganicSystem to have a 
centralized area for exporting and importing data/requests.

*/
class MessageCable
{
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
		


		// incoming container functions
		void insertIncomingContainer(MessageContainer in_messageContainer)
		{
			std::lock_guard<std::mutex> lock(incomingContainerQueueGuard);
			incomingContainers.push(in_messageContainer);
		}

		void popIncomingContainerQueue()
		{
			std::lock_guard<std::mutex> lock(incomingContainerQueueGuard);
			incomingContainers.pop();
		}

		bool isIncomingContainerQueueEmpty()
		{
			std::lock_guard<std::mutex> lock(incomingContainerQueueGuard);
			return incomingContainers.empty();
		}

		MessageContainer* getIncomingMessageContainerRefFromFront()
		{
			std::lock_guard<std::mutex> lock(incomingContainerQueueGuard);
			return &incomingContainers.front();
		}



		// outgoing container functions
		void insertOutgoingContainer(MessageContainer in_messageContainer)
		{
			std::lock_guard<std::mutex> lock(outgoingContainerQueueGuard);
			outgoingContainers.push(in_messageContainer);
		}

		void popOutgoingContainerQueue()
		{
			std::lock_guard<std::mutex> lock(outgoingContainerQueueGuard);
			outgoingContainers.pop();
		}

		bool isOutgoingContainerQueueEmpty()
		{
			std::lock_guard<std::mutex> lock(outgoingContainerQueueGuard);
			return outgoingContainers.empty();
		}

		MessageContainer* getOutgoingMessageContainerRefFromFront()
		{
			std::lock_guard<std::mutex> lock(outgoingContainerQueueGuard);
			return &outgoingContainers.front();
		}

	private:
		std::queue<Message> incomingMessages;	// put responses (for instance, what a server responds to the client with)
		std::mutex incomingQueueGuard;

		std::queue<Message> outgoingMessages;	// put requests (for instance, for a client's request)
		std::mutex outgoingQueueGuard;

		std::queue<MessageContainer> incomingContainers;
		std::mutex incomingContainerQueueGuard;

		std::queue<MessageContainer> outgoingContainers;
		std::mutex outgoingContainerQueueGuard;



};

#endif
