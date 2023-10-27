#pragma once

#ifndef MESSAGECONTAINER_H
#define MESSAGECONTAINER_H

#include "MessageContainerType.h"

/*

Summary: a MessageContainer is simply a set of one or more Message instances.
We can also give a type to the MessageContainer to make it easier to determine how to work with it,
or where it should go.

*/

class MessageContainer
{
	public:
		// Below: this constructor is used when we want to instantiate a MessageContainer with a certain
		// MessageContainerType, in a clean easy to use way.
		MessageContainer(MessageContainerType in_containerType) : containerType(in_containerType) {};

		MessageContainer() {};	// default constructor that is now required, since we defined our own non-default one above.

		MessageContainer& operator+=(const MessageContainer& container_b)
		{
			// copy messages
			auto otherContainerBegin = container_b.messages.begin();
			auto otherContainerEnd = container_b.messages.end();
			for (; otherContainerBegin != otherContainerEnd; otherContainerBegin++)
			{
				messages.push_back(*otherContainerBegin);
			}

			// copy the type
			containerType = container_b.containerType;

			return *this;
		}

		int estimatedSizeOfAllMessages()
		{
			int estimatedTotal = 0;
			auto messagesBegin = messages.begin();
			auto messagesEnd = messages.end();
			for (; messagesBegin != messagesEnd; messagesBegin++)
			{
				estimatedTotal += messagesBegin->getEstimatedByteSizeOfContents();
			}
			return estimatedTotal;
		}


		std::vector<Message>::iterator begin()
		{
			return messages.begin();
		};

		std::vector<Message>::iterator end()
		{
			return messages.end();
		};

		void insertMessage(Message in_message)
		{
			messages.push_back(in_message);
		};

		bool empty()
		{
			return messages.empty();
		}

		int size()
		{
			return int(messages.size());
		}

		// append operations, that append to all Messages in a container
		void appendIntToAll(int in_int)
		{
			auto messagesBegin = messages.begin();
			auto messagesEnd = messages.end();
			for (; messagesBegin != messagesEnd; messagesBegin++)
			{
				messagesBegin->insertInt(in_int);
			}
		}

		MessageContainerType containerType = MessageContainerType::MC_UNDEFINED;

	private:
		std::vector<Message> messages;
};

#endif
