#pragma once

#ifndef CYCLE_H
#define CYCLE_H

//#include <unordered_set>
#include <map>
#include <mutex>
#include <iostream>

template <typename CycleKey, typename CycleValue> class Cycle {
	public:
		bool empty()
		{
			bool isEmpty = false;
			if (cycleMap.empty() == false)
			{
				isEmpty = true;
			}
			return isEmpty;
		}

		typename std::map<CycleKey, CycleValue>::iterator begin() 
		{ 
			std::lock_guard<std::mutex> guard(cycleMutex);
			return cycleMap.begin(); 
		}

		typename std::map<CycleKey, CycleValue>::reverse_iterator rbegin() 
		{ 
			std::lock_guard<std::mutex> guard(cycleMutex);
			return cycleMap.rbegin();
		}
		
		typename std::map<CycleKey, CycleValue>::iterator end() 
		{ 
			std::lock_guard<std::mutex> guard(cycleMutex);
			return cycleMap.end(); 
		}

		typename std::map<CycleKey, CycleValue>::iterator find(CycleKey in_indexToFind) 
		{ 
			std::lock_guard<std::mutex> guard(cycleMutex);
			return cycleMap.find(in_indexToFind); 
		}

		typename std::map<CycleKey, CycleValue>::iterator getCurrentElementIter()
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			return currentCycleElement;
		}

		CycleValue eraseViaBegin(CycleKey in_elementToErase) {
			std::lock_guard<std::mutex> guard(cycleMutex);
			cycleMap.erase(in_elementToErase);
			resetBeginAndEndIters();
		}
		CycleValue size() {
			std::lock_guard<std::mutex> guard(cycleMutex);
			return cycleMap.size();
		}

		CycleValue& operator[](const CycleKey& in_key)
		{
			// For usage cases such as cycle[x] = value, the "value" would be equivalent to CycleValue&, and the second parameter used in std::make_pair.
			// See this for reference: https://en.cppreference.com/w/cpp/container/map/operator_at

			std::lock_guard<std::mutex> guard(cycleMutex);
			auto insertedValueIter = this->cycleMap.insert(std::make_pair(in_key, CycleValue()));	// attempt to insert the key/value pair if it doesn't exist, 
																									// and return the iterator to that
			resetBeginAndEndIters();	// reset the beginning and ending element iterators, before we return
			return (*insertedValueIter.first).second;		// return a reference to the data value that exists at the key value
		}

		void insert(CycleKey in_key, CycleValue in_insertCycleValue)	// insert a key at the specified "in_key" location, if it doesn't exist.
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			if (doesCycleValueExist(in_insertCycleValue) == false)
			{
				cycleMap[in_key] = in_insertCycleValue;
				resetBeginAndEndIters();
			}
		}

		typename std::map<CycleKey, CycleValue>::iterator getNextElement()
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			if (cycleMap.size() >= 2)
			{
				// CASE 1: currentCycleElement isn't equal to the end; just get the next element.
				if
				(
					(currentCycleElement != currentElementEndIter)
				)
				{
					//std::cout << "!!! First normal condition met." << std::endl;
					currentCycleElement++;
				}

				// CASE 2: currentCycleElement is the the ending element; the next element in the cycle is the beginning of the map.
				else if
				(
					currentCycleElement == currentElementEndIter
				)
				{
					//std::cout << "!!! Second normal condition met." << std::endl;
					currentCycleElement = currentElementStartIter;
				}
			}
			else
			{
				return currentCycleElement;	// if there's just one element, jsut return this
			}
			
			return currentCycleElement;
		}

		typename std::map<CycleKey, CycleValue>::iterator getPreviousElement()
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			//typename std::map<CycleKey, CycleValue>::reverse_iterator returnIter = cycleMap.find(0);
			if (cycleMap.size() >= 2)
			{
				// CASE 1: currentCycleElement isn't equal to the beginning; just get the previous element.
				if
				(
					(currentCycleElement != currentElementStartIter)
				)
				{
					// get the reverse iterator, increment by one, and use that iterator's key in a normal iterator find operation.
					auto reverseIter = findReverseIter(currentCycleElement->first);
					reverseIter++;
					CycleKey foundReverseIterKey = reverseIter->first;
					currentCycleElement = cycleMap.find(foundReverseIterKey);
				}

				// CASE 2: currentCycleElement is the the beginning element; the next element in the cycle is the end of the map.
				else if
				(
					(currentCycleElement == currentElementStartIter)
				)
				{
					currentCycleElement = currentElementEndIter;
				}
			}
			else
			{
				return currentCycleElement;	// if there's just one element, jsut return this
			}

			return currentCycleElement;
		}

		void setCurrentElementByKey(CycleKey in_keyToSet)
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			currentCycleElement = cycleMap.find(in_keyToSet);
		}

	private:
		std::map<CycleKey, CycleValue> cycleMap;
		typename std::map<CycleKey, CycleValue>::iterator currentElementStartIter;
		typename std::map<CycleKey, CycleValue>::iterator currentCycleElement;
		typename std::map<CycleKey, CycleValue>::iterator currentElementEndIter;
		std::mutex cycleMutex;

		void resetBeginAndEndIters()
		{
			currentElementStartIter = cycleMap.begin();
			if (cycleMap.size() == 1)
			{
				currentElementEndIter = currentElementStartIter;
			}
			else if (cycleMap.size() > 1)
			{
				currentElementEndIter = cycleMap.begin();
				for (int x = 0; x < (cycleMap.size() - 1); x++)
				{
					currentElementEndIter++;
				}
			}
		}

		bool doesCycleValueExist(CycleValue in_cycleValueToFind)
		{
			bool exists = false;
			auto cyclesBegin = cycleMap.begin();
			auto cyclesEnd = cycleMap.end();
			for (; cyclesBegin != cyclesEnd; cyclesBegin++)
			{
				if (cyclesBegin->second == in_cycleValueToFind)
				{
					exists = true;
					break;
				}
			}
			return exists;
		}

		
		typename std::map<CycleKey, CycleValue>::reverse_iterator findReverseIter(CycleKey in_keyToSearchFor)
		{
			typename std::map<CycleKey, CycleValue>::reverse_iterator returnIter = cycleMap.rbegin();
			for (; returnIter != cycleMap.rend(); returnIter++)
			{
				if (returnIter->first == in_keyToSearchFor)
				{
					return returnIter;
				}
			}
			return returnIter;
		}
		
};

#endif