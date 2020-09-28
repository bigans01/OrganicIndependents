#pragma once

#ifndef MESSAGE
#define MESSAGE

#include "MessageLocality.h"
#include "MessageType.h"
#include "EnclaveKeyDef.h"
#include <vector>

class Message
{
	public:
		Message() {};
		Message& operator=(const Message& message_b)
		{
			messageLocality = message_b.messageLocality;
			messageType = message_b.messageType;
			intVector = message_b.intVector;
			//intVectorIter = intVector.begin();
			messageID = message_b.messageID;
			return *this;
		}
		Message(int in_messageID, MessageLocality in_messageLocality, MessageType in_messageType) : messageID(in_messageID), messageLocality(in_messageLocality), messageType(in_messageType) {};
		MessageLocality messageLocality = MessageLocality::LOCAL;	// default is LOCAL
		MessageType messageType = MessageType::NOVAL;
		std::vector<int> intVector;
		std::vector<int>::iterator intVectorIter;
		int messageID = 0;				// set by the constructor.

		void insertEnclaveKey(EnclaveKeyDef::EnclaveKey in_key)
		{
			intVector.push_back(in_key.x);
			intVector.push_back(in_key.y);
			intVector.push_back(in_key.z);
		}

		void insertInt(int in_int)
		{
			intVector.push_back(in_int);
		}

		EnclaveKeyDef::EnclaveKey readEnclaveKey()
		{
			EnclaveKeyDef::EnclaveKey returnKey;
			returnKey.x = *intVectorIter++;
			returnKey.y = *intVectorIter++;
			returnKey.z = *intVectorIter++;
			return returnKey;
		}

		int readInt()
		{
			return *intVectorIter++;
		}

		void open()
		{
			intVectorIter = intVector.begin();
		}

};

#endif
