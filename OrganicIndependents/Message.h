#pragma once

#ifndef MESSAGE
#define MESSAGE

#include "MessageLocality.h"
#include "MessageType.h"
#include <type_traits>

/*

Description: The Message class is designed to be a jack-of-all-trades data container, that can be used as a means to transfer across multiple classes.
The MessageType of this class is an enum value that represents the intent of the Message; assuming that each MessageType is only used in a few functions,
finding where the Message is utilized should be trivial, as it is simply matter of running a ctrl+f or other similiar find logic on the libraries, to 
see where Message objects or instances of a certain MessageType are being utilized.

Additionally, a templated constructor was added/tested, on or around 11/23/2024. The templated constructor functions allow for inserting
of various data types covered by this class in one singular call, where each parameter int the template is inserted into the Message object
via recursion. 

*/

class Message
{
	public:
		// Below: base Message templated constructor
		template<typename FirstItem, typename ...RemainingItems> Message(MessageType in_messageType, FirstItem && firstI, RemainingItems && ...remainingI)
		{
			messageType = in_messageType;
			insertRecursively(std::forward<FirstItem>(firstI), std::forward<RemainingItems>(remainingI)...);
		}

		// Below: recursion template that does all the actual work when needing to insert data via a templated Message constructor.
		// There are multiple if else checks, per basic data type:
		//
		//	-one check where the value is hardcoded in-line, i.e the int 5
		//  -another check that examines if a variable of of that type is passed as a template argument (i.e, int myVal = 5, then pasisng myVal as a template parameter);
		//   this action causes the passed-in template parameter to evaluate as a referenced value (so it would equate to <data type>& below)
		//
		// Attempts to use std::remove_reference were made, but this resulted in an "illegal indirection" error, with code such as this:
		//
		//	else if constexpr (std::is_same<std::remove_reference<FirstItem>::type, ECBPolyPoint>) { ... }
		template<typename FirstItem, typename ...RemainingItems> void insertRecursively(FirstItem && firstI, RemainingItems && ...remainingI)
		{
			// int types
			if constexpr (std::is_same<FirstItem, int>::value) { intVector.push_back(std::forward<FirstItem>(firstI)); }
			else if constexpr (std::is_same<FirstItem, int&>::value) { intVector.push_back(std::forward<FirstItem>(firstI)); }

			// float types
			else if constexpr (std::is_same<FirstItem, float>::value) { floatVector.push_back(std::forward<FirstItem>(firstI)); }
			else if constexpr (std::is_same<FirstItem, float&>::value) { floatVector.push_back(std::forward<FirstItem>(firstI)); }

			// std::string types
			// Remember: the value passed in the templated constructor call MUST be of type std::string, not hardcoded (i.e, "hello") 
			else if constexpr (std::is_same<FirstItem, std::string>::value) { stringVector.push_back(std::forward<FirstItem>(firstI)); }
			else if constexpr (std::is_same<FirstItem, std::string&>::value) { stringVector.push_back(std::forward<FirstItem>(firstI)); }

			// EnclaveKeyDef::EnclaveKey types
			else if constexpr (std::is_same<FirstItem, EnclaveKeyDef::EnclaveKey>::value) { insertEnclaveKey(std::forward<FirstItem>(firstI)); }
			else if constexpr (std::is_same<FirstItem, EnclaveKeyDef::EnclaveKey&>::value) { insertEnclaveKey(std::forward<FirstItem>(firstI)); }

			// ECBPolyPoint types
			else if constexpr (std::is_same<FirstItem, ECBPolyPoint>::value) { insertPoint(std::forward<FirstItem>(firstI)); }
			else if constexpr (std::is_same<FirstItem, ECBPolyPoint&>::value) { insertPoint(std::forward<FirstItem>(firstI)); }

			// Const array type checks
			// 
			//else if constexpr (std::is_same<char[5], std::remove_cvref_t<FirstItem>>::value)	// --> works with "yes!" const char [5] in Message

			//else if constexpr (std::is_same_v<FirstItem, const char(&)[5]>)		// --> works with "yes!"
			//else if constexpr (std::is_same_v<std::remove_extent_t<FirstItem>, const char(&)[5]>)

			// For below, using an example of the string "yes!" passed as an arg:
			// 
			// Using std::remove_cvref_t<FirstItem>> causes the value to be char[5], from it's original form of const char(&)[5].
			// When using the char[5] check against std::is_array_v, it works.
			else if constexpr (std::is_array_v<std::remove_cvref_t<FirstItem>>)	// --> works, when attempting to detect an array; we can get size wof array with this!!!
			{
				// Get the size of array (must be a const value)
				const int arraySize = sizeof(std::remove_cvref_t<FirstItem>);

				// Check for char array (i.e, "thing" passed in Message directly, not std::string("thing"))
				if constexpr (std::is_same_v<std::remove_extent_t<FirstItem>, const char(&)[arraySize]>)
				{
					stringVector.push_back(std::forward<FirstItem>(firstI));
				}
			}
			else
			{
				std::cout << "UH OH..." << std::endl;
			}

			insertRecursively(std::forward<RemainingItems>(remainingI)...);
		}
		void insertRecursively() {};

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

		int getEstimatedByteSizeOfContents()
		{
			int totalSizeBytes = 0;
			totalSizeBytes += (intVector.size() * sizeof(int));
			totalSizeBytes += (floatVector.size() * sizeof(float));
			return totalSizeBytes;
		}

		// ------------------------------------------------- insertion functions (inserts at end of vector)
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

		// ------------------------------------------------- front insertion functions (inserts at front of vector)
		void insertEnclaveKeyFront(EnclaveKeyDef::EnclaveKey in_key)
		{
			// Remember, the int key value must be written in reverse order (z,y,x), because we're going the opposite way that the 
			// vector is used to.
			intVector.insert(intVector.begin(), in_key.z);
			intVector.insert(intVector.begin(), in_key.y);
			intVector.insert(intVector.begin(), in_key.x);
		}

		void insertPointFront(ECBPolyPoint in_point)
		{
			// Same rules from insertEnclaveKeyFront apply here (point is inserted in reverse order)
			floatVector.insert(floatVector.begin(), in_point.z);
			floatVector.insert(floatVector.begin(), in_point.y);
			floatVector.insert(floatVector.begin(), in_point.x);
		}

		void insertIntFront(int in_int)		// inserts an int into the front, instead of the back
		{
			intVector.insert(intVector.begin(), in_int);
		}

		void insertFloatFront(float in_float) // inserts a float into the front, instead of the back
		{
			floatVector.insert(floatVector.begin(), in_float);
		}

		void insertStringFront(std::string in_string)	// inserts a string into the front, instead of the back
		{
			stringVector.insert(stringVector.begin(), in_string);
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

		void removeEnclaveKeyFromFrontAndResetIter(int in_numberOfKeysToRemove)
		{
			// The number of ints to remove is always equal to the in_numberOfKeysToRemove *3, since an EnclaveKeyDef::EnclaveKey consists of 3 int values.
			for (int x = 0; x < (in_numberOfKeysToRemove * 3); x++)
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
