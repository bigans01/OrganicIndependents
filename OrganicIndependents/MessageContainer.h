#pragma once

#ifndef MESSAGECONTAINER_H
#define MESSAGECONTAINER_H

#include "Message.h"
#include <vector>

class MessageContainer
{
	public:
		MessageContainer& operator+=(const MessageContainer& container_b)
		{
			auto otherContainerBegin = container_b.messages.begin();
			auto otherContainerEnd = container_b.messages.end();
			for (; otherContainerBegin != otherContainerEnd; otherContainerBegin++)
			{
				messages.push_back(*otherContainerBegin);
			}
			return *this;
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

	private:
		std::vector<Message> messages;
};

#endif
