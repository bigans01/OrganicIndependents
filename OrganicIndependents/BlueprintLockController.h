#pragma once

#ifndef BLUEPRINTLOCKCONTROLLER_H
#define BLUEPRINTLOCKCONTROLLER_H

#include <mutex>
#include <vector>
#include <map>
#include "EnclaveKeyDef.h"
#include "BlueprintLockRequest.h"
#include <iostream>

class BlueprintLockController
{
public:
	template<typename FirstKey, typename ...RemainingKeys> BlueprintLockRequest requestBlueprintLock(FirstKey&& firstKey, RemainingKeys && ...remainingKeys)
	{
		std::lock_guard<std::mutex> lock(blueprintLockScanGuard);
		BlueprintLockRequest producedRequest;

		// a zero return value of number of keys locked indicates that all the keys are available.
		if (numberOfKeysLocked(0, std::forward<FirstKey>(firstKey), std::forward<RemainingKeys>(remainingKeys)...) == 0)
		{
			//std::cout << "(BlueprintLockController): Blueprint lock combo available; attempting to lock. " << std::endl;
			//int successWorking = 3;
			//std::cin >> successWorking;
			nextRequestID++;
			insertKeyLocks(nextRequestID, std::forward<FirstKey>(firstKey), std::forward<RemainingKeys>(remainingKeys)...);
			producedRequest = BlueprintLockRequest(nextRequestID);
		}
		else
		{
			std::cout << "(BlueprintLockController): Lock not available. " << std::endl;
		}
		return producedRequest;
	}
	void releaseLock(int in_requestIDToRelease)
	{
		std::cout << "(BlueprintLockController): Attempting to erase request with ID: " << in_requestIDToRelease << std::endl;
		std::lock_guard<std::mutex> lock(blueprintLockScanGuard);
		lockedKeysMap.erase(in_requestIDToRelease);
	};
private:
	std::mutex blueprintLockScanGuard;
	std::map<int, std::vector<EnclaveKeyDef::EnclaveKey>> lockedKeysMap;
	int nextRequestID = 0;	// if a request is valid, this is the value that will be assigned to that request ID.

	template<typename FirstKey, typename ...RemainingKeys> int numberOfKeysLocked(int in_recursiveReturnValue, FirstKey&& firstKey, RemainingKeys && ...remainingKeys)
	{
		int numberOfLockedKeys = in_recursiveReturnValue;
		EnclaveKeyDef::EnclaveKey currentKey = std::forward<FirstKey>(firstKey);

		//std::cout << "Checking if ";
		//currentKey.printKey();
		//std::cout << " is locked. " << std::endl;

		if (isKeyLocked(currentKey))
		{
			//std::cout << "(BlueprintLockController): key ";
			//currentKey.printKey();
			//std::cout << " is LOCKED." << std::endl;
			numberOfLockedKeys++;
		}
		return numberOfKeysLocked(numberOfLockedKeys, std::forward<RemainingKeys>(remainingKeys)...);
	}
	int numberOfKeysLocked(int in_recursiveReturnValue) { return in_recursiveReturnValue; }; // required for above templated recursive function

	template<typename FirstKey, typename ...RemainingKeys> void insertKeyLocks(int in_requestID, FirstKey&& firstKey, RemainingKeys && ...remainingKeys)
	{
		lockedKeysMap[in_requestID].push_back(std::forward<FirstKey>(firstKey));
		insertKeyLocks(in_requestID, std::forward<RemainingKeys>(remainingKeys)...);
	}
	void insertKeyLocks(int in_requestID) {};

	bool isKeyLocked(EnclaveKeyDef::EnclaveKey in_keyToSearch)
	{
		bool isLocked = false;
		auto lockedBegin = lockedKeysMap.begin();
		auto lockedEnd = lockedKeysMap.end();
		for (; lockedBegin != lockedEnd; lockedBegin++)
		{
			auto currentVectorBegin = lockedBegin->second.begin();
			auto currentVectorEnd = lockedBegin->second.end();
			for (; currentVectorBegin != currentVectorEnd; currentVectorBegin++)
			{
				if ((*currentVectorBegin) == in_keyToSearch)
				{
					isLocked = true;	// we found it, so let's break out of the loop.
					break;
				}
			}
		}
		return isLocked;
	};
};

#endif
