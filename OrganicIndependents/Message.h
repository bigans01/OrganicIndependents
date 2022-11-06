#pragma once

#ifndef MESSAGE
#define MESSAGE

#include "MessageLocality.h"
#include "MessageType.h"

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
		Message(MessageType in_messageType) : messageType(in_messageType) {};

		MessageLocality messageLocality = MessageLocality::LOCAL;	// default is LOCAL
		MessageType messageType = MessageType::NOVAL;

		std::vector<int> intVector;
		std::vector<float> floatVector;
		std::vector<std::string> stringVector;
		
		std::vector<int>::iterator intVectorIter;
		std::vector<float>::iterator floatVectorIter;
		std::vector<std::string>::iterator stringVectorIter;

		int messageID = 0;				// set by the constructor.

		// ------------------------------------------------- appending functions
		void appendOtherMessage(Message* in_otherMessageRef)
		{
			// append any ints from other message.
			if (in_otherMessageRef->intVector.size() != 0)
			{
				for (auto& otherMessageIntVectorCurrentVal : in_otherMessageRef->intVector)
				{
					intVector.push_back(otherMessageIntVectorCurrentVal);
				}
			}

			// append any floats from other message.
			if (in_otherMessageRef->floatVector.size() != 0)
			{
				for (auto& otherMessageFloatVectorCurrentVal : in_otherMessageRef->floatVector)
				{
					floatVector.push_back(otherMessageFloatVectorCurrentVal);
				}
			}

			// append any strings from other message.
			if (in_otherMessageRef->stringVector.size() != 0)
			{
				for (auto& otherMessageStringVectorCurrentVal : in_otherMessageRef->stringVector)
				{
					stringVector.push_back(otherMessageStringVectorCurrentVal);
				}
			}
		}

		// ------------------------------------------------- insertion functions
		void insertEnclaveKey(EnclaveKeyDef::EnclaveKey in_key)
		{
			intVector.push_back(in_key.x);
			intVector.push_back(in_key.y);
			intVector.push_back(in_key.z);
		}

		void insertPoint(ECBPolyPoint in_point)
		{
			floatVector.push_back(in_point.x);
			floatVector.push_back(in_point.y);
			floatVector.push_back(in_point.z);
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

		// ------------------------------------------------- removal functions
		void removeIntsFromFrontAndResetIter(int in_numberOfIntsToRemove)
		{
			for (int x = 0; x < in_numberOfIntsToRemove; x++)
			{
				intVector.erase(intVector.begin());
			}
			intVectorIter = intVector.begin();
		}

		void removeFloatsFromFrontAndResetIter(int in_numberOfFloatsToRemove)
		{
			for (int x = 0; x < in_numberOfFloatsToRemove; x++)
			{
				floatVector.erase(floatVector.begin());
			}
			floatVectorIter = floatVector.begin();
		}

		void removeStringsFromFrontAndResetIter(int in_numberOfStringsToRemove)
		{
			for (int x = 0; x < in_numberOfStringsToRemove; x++)
			{
				stringVector.erase(stringVector.begin());
			}
			stringVectorIter = stringVector.begin();
		}


		// ------------------------------------------------- read functions
		EnclaveKeyDef::EnclaveKey readEnclaveKey()
		{
			EnclaveKeyDef::EnclaveKey returnKey;
			returnKey.x = *intVectorIter++;
			returnKey.y = *intVectorIter++;
			returnKey.z = *intVectorIter++;
			return returnKey;
		}

		ECBPolyPoint readPoint()
		{
			ECBPolyPoint returnPoint;
			returnPoint.x = *floatVectorIter++;
			returnPoint.y = *floatVectorIter++;
			returnPoint.z = *floatVectorIter++;
			return returnPoint;
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
