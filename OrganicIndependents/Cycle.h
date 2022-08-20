#pragma once

#ifndef CYCLE_H
#define CYCLE_H

#include <map>
#include <mutex>
#include <iostream>

/*

Description:

Template class Cycle

A Cycle is a templated class, built off the std::map STL container. Unlike a map, it has the ability to
store an iterator that represents the currently selected element in a list, through the member currentCycleElement.
This iterator can be manually set through the call to setCurrentElementByKey, or is automatically set to the beginning of the map
if setCurrentElementByKey hasn't been called yet, when calling getCurrentElementIter() for the first time.

If all elements are removed from the cycle, the value of getCurrentElementIter() will be an iterator that points to the end() 
of the underlying map, and the bool value of wasSetCurrentElementCalled will get set back to false. The begin, rbegin, end,
find, and erase functions all function the same as they would in a normal map; their purpose here is to serve as wrappers
for ease of use.

*/

template <typename CycleKey, typename CycleValue> class Cycle {
	public:
		bool empty()
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			return cycleMap.empty();
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
			setCurrentElementIfNotSet();	// safety: set the value of currentCycleElement to the beginning of the map, if it isn't set.
			return currentCycleElement;
		}

		int size() 
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			return cycleMap.size();
		}

		void clear()
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			cycleMap.clear();			// first, empty out the cycleMap.
			resetBeginAndEndIters();	// reset the iters.
			currentCycleElement = cycleMap.end();	// set the currentCycleElement to be the end
			wasSetCurrentElementCalled = false;		// lastly, reset the value of wasSetCurrentElementCalled.
		}

		void erase(CycleKey in_elementToErase) 
		{
			std::lock_guard<std::mutex> guard(cycleMutex);

			// first, get size of current map; if the value of this is different after the erase,
			// we know the iterator needs to be reset.
			int preEraseAttemptMapSize = cycleMap.size();

			// check if the current element to erase is equal to the current iterator;
			// also, get what the next element would be post erase, if we did it.
			// if it is, we will need to reset the value of currentCycleElement after the erase.
			
			bool isElementToEraseEqualToCurrentCycleElement = false;
			auto elementToEraseFinder = cycleMap.find(in_elementToErase);
			auto predictedNext = predictNextElementAfterErase();	// no lock_guard should be attempted here, as that is already done at the beginning of this function.
																	// predictedNext will only really need to be used below if the pre erase size was > 2.

			
			if (currentCycleElement == elementToEraseFinder)
			{
				isElementToEraseEqualToCurrentCycleElement = true;
			}
			

			// attempt the erase, and reset the begin/end iters.
			cycleMap.erase(in_elementToErase);
			resetBeginAndEndIters();

			
			// check to see if we need the reset the currentCycleElement.
			// We only need to do this if it's value is equal to the key we're about to erase, as that would make currentCycleElement 
			// invalid. Otherwise, if currentCycleElement is NOT equal to the key we're about to erase -- we are fine, because it will
			// still be valid after this operation.

			// Two conditions must be met: 
			// A.) the size of the cycleMap can't be equal to currentMapSize value above
			// B.) the element that was erased was actually what the currentCycleElement was before the erase
			if
			(
				(preEraseAttemptMapSize != cycleMap.size())
				&&
				(isElementToEraseEqualToCurrentCycleElement == true)
			)
			{
				// at this point, the value of currentCycleElement is invalid, so it must be set again.
				// We do this based on what the value of preEraseAttemptMapSize is.

				// If preEraseAttemptMapSize was > 2, we now have at least 2 elements remaining. So the value
				// of currentCycleElement would be equal to the predicted value returned by
				// the call to predictNextElementAfterErase().
				if (preEraseAttemptMapSize > 2)
				{
					currentCycleElement = predictedNext;
				}

				// If there were exactly 2 elements before the erase,
				// There's only one remaining.
				else if (preEraseAttemptMapSize == 2)
				{
					currentCycleElement = cycleMap.begin();
				}

				// If there was just 1 element in the map, there is now nothing; make the currentCycleElement the end of the map.
				else if (preEraseAttemptMapSize == 1)
				{
					currentCycleElement = cycleMap.end();
				}
			}

			// if the size of the cycle is now 0, it is back at it's original state, 
			// so the value of wasSetCurrentElementCalled needs to be set back to false.
			if (cycleMap.size() == 0)
			{
				wasSetCurrentElementCalled = false;
			}		
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
			// It should be noted, that if the key of the iter returned (we'll call it returnedKey) by getCurrentElementIter()
			// matches the key we are inserting, that the returnedKey will NOT be invalidated -- and it's value will be updated.
			// This lesser known rule of std::map has been tested and verified.

			std::lock_guard<std::mutex> guard(cycleMutex);
			cycleMap[in_key] = in_insertCycleValue;
			resetBeginAndEndIters();
		}


		typename std::map<CycleKey, CycleValue>::iterator getNextElement()
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			setCurrentElementIfNotSet(); // do this, in case that getCurrentElementIter() is not called before this.
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
			else if (cycleMap.size() == 1)
			{
				return currentCycleElement;	// if there's just one element, jsut return this
			}
			else if (cycleMap.size() == 0)
			{
				currentCycleElement = cycleMap.end();	// there are no elements remaining; return end()
			}
			
			return currentCycleElement;
		}

		typename std::map<CycleKey, CycleValue>::iterator getPreviousElement()
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			setCurrentElementIfNotSet(); // do this, in case that getCurrentElementIter() is not called before this.
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
			else if (cycleMap.size() == 1)
			{
				return currentCycleElement;	// if there's just one element, jsut return this
			}
			else if (cycleMap.size() == 0)
			{
				currentCycleElement = cycleMap.end();	// there are no elements remaining; return end()
			}

			return currentCycleElement;
		}

		void setCurrentElementByKey(CycleKey in_keyToSet)
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			currentCycleElement = cycleMap.find(in_keyToSet);
			wasSetCurrentElementCalled = true;
		}

	private:
		std::map<CycleKey, CycleValue> cycleMap;
		typename std::map<CycleKey, CycleValue>::iterator currentElementStartIter;
		typename std::map<CycleKey, CycleValue>::iterator currentCycleElement;
		typename std::map<CycleKey, CycleValue>::iterator currentElementEndIter;
		std::mutex cycleMutex;
		bool wasSetCurrentElementCalled = false;	// this gets set to true when setCurrentElementByKey is called,
													// or gets set automatically if setCurrentElementByKey hasn't been called before 
													// the first call to getCurrentElementIter(). This value gets set back to false
													// during the erase function call, if there are no elements left in the map after the erase.

		void setCurrentElementIfNotSet()
		{
			if (wasSetCurrentElementCalled == false)
			{
				currentCycleElement = cycleMap.begin();
				wasSetCurrentElementCalled = true;
			}
		}

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
		
		typename std::map<CycleKey, CycleValue>::iterator predictNextElementAfterErase()
		{
			typename std::map<CycleKey, CycleValue>::iterator predictedElement;
			// check that the currentCycleElement is actually set.
			setCurrentElementIfNotSet();

			// There would be at least 2 elements remaining after the erase.
			if (cycleMap.size() > 2)
			{
				// CASE 1: currentCycleElement isn't equal to the end; just get the next element.
				if (currentCycleElement != currentElementEndIter)
				{
					// the next element would be whatever is ahead of the currentCycleElement.
					predictedElement = currentCycleElement;
					predictedElement++;
				}

				// CASE 2: currentCycleElement is the the ending element; the next element in the cycle is the beginning of the map.
				else if (currentCycleElement == currentElementEndIter)
				{
					// if currentCycleElement is already at the end, the next element would be the beginning.
					predictedElement = currentElementStartIter;
				}
			}

			// NOTE: these next two else ifs may not be needed; more testing may need to be done.

			// There would be exactly 1 element remaining after the erase.
			else if (cycleMap.size() == 2)
			{
				predictedElement = cycleMap.begin();
			}

			// There would be 0 elements remaining after the erase.
			else if (cycleMap.size() == 1)
			{
				predictedElement = cycleMap.end();
			}

			return predictedElement;
		}
};

#endif