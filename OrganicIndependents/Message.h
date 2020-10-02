#pragma once

#ifndef MESSAGE
#define MESSAGE

#include "MessageLocality.h"
#include "MessageType.h"
#include "EnclaveKeyDef.h"
#include <vector>
#include <string>

class Message
{
	public:
		Message() {};
		Message& operator=(const Message& message_b)
		{
			messageLocality = message_b.messageLocality;
			messageType = message_b.messageType;
			intVector = message_b.intVector;
			floatVector = message_b.floatVector;
			stringVector = message_b.stringVector;
			messageID = message_b.messageID;
			return *this;
		}
		Message(int in_messageID, MessageLocality in_messageLocality, MessageType in_messageType) : messageID(in_messageID), messageLocality(in_messageLocality), messageType(in_messageType) {};

		MessageLocality messageLocality = MessageLocality::LOCAL;	// default is LOCAL
		MessageType messageType = MessageType::NOVAL;

		std::vector<int> intVector;
		std::vector<float> floatVector;
		std::vector<std::string> stringVector;
		
		std::vector<int>::iterator intVectorIter;
		std::vector<float>::iterator floatVectorIter;
		std::vector<std::string>::iterator stringVectorIter;

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

		void insertFloat(float in_float)
		{
			floatVector.push_back(in_float);
		}

		void insertString(std::string in_string)
		{
			stringVector.push_back(in_string);
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

		float readFloat()
		{
			return *floatVectorIter++;
		}

		std::string readString()
		{
			return *stringVectorIter++;
		}

		void open()
		{
			intVectorIter = intVector.begin();
			floatVectorIter = floatVector.begin();
			stringVectorIter = stringVector.begin();
		}

};

#endif
