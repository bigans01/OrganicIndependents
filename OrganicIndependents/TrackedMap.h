#pragma once

#ifndef TRACKEDMAP_H
#define TRACKEDMAP_H

/*

Description

Template class TrackedMap

A TrackedMap is a templated class, built off the std::map STL container. It is built in a similiar fashion to the Cycle
template class, with the primary difference being that the currentTrackedMapElement (which points to an element in the map) doesn't
go back to the beginning of the map when the value is currently pointing to the last element in the map, once getNextElement() 
is called; the same can be said for when currentTrackedMapElement is at the beginning of the map and getPreviousElement is called.

*/

template <typename TrackedMapKey, typename TrackedMapValue> class TrackedMap {
	public:

		// the following two constructors and = operator must be defined, 
		// because of the fact that the implicit constructors would try to copy the mutex, which is illegal.
		TrackedMap() {};
		TrackedMap(const TrackedMap& in_otherTrackedMap)
		{
			// Remember, when doing this, don't use the functions of in_otherTrackedMap (because they aren't const, etc)	

			// copy everything but the mutex
			// first, copy the map.
			trackedMap = in_otherTrackedMap.trackedMap;

			// reset the begin/end iters
			resetBeginAndEndIters();

			// once it's copied over, check if wasSetCurrentElementCalled was set;
			if (in_otherTrackedMap.wasSetCurrentElementCalled == true)
			{
				TrackedMapKey otherMapCurrentKey = in_otherTrackedMap.currentTrackedMapElement->first;
				setCurrentElementByKey(otherMapCurrentKey);
				wasSetCurrentElementCalled = true;
			}
			
			
		}
		TrackedMap& operator=(const TrackedMap& in_otherTrackedMap)
		{
			// Remember, when doing this, don't use the functions of in_otherTrackedMap (because they aren't const, etc)	

			// copy everything but the mutex
			// first, copy the map.
			trackedMap = in_otherTrackedMap.trackedMap;

			// reset the begin/end iters
			resetBeginAndEndIters();

			// once it's copied over, check if wasSetCurrentElementCalled was set;
			if (in_otherTrackedMap.wasSetCurrentElementCalled == true)
			{
				TrackedMapKey otherMapCurrentKey = in_otherTrackedMap.currentTrackedMapElement->first;
				setCurrentElementByKey(otherMapCurrentKey);
				wasSetCurrentElementCalled = true;
			}

			return *this;
		}

		bool empty()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			return trackedMap.empty();
		}

		typename std::map<TrackedMapKey, TrackedMapValue>::iterator begin()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			return trackedMap.begin();
		}

		typename std::map<TrackedMapKey, TrackedMapValue>::reverse_iterator rbegin()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			return trackedMap.rbegin();
		}

		typename std::map<TrackedMapKey, TrackedMapValue>::iterator end()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			return trackedMap.end();
		}

		typename std::map<TrackedMapKey, TrackedMapValue>::iterator find(TrackedMapKey in_indexToFind)
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			return trackedMap.find(in_indexToFind);
		}

		typename std::map<TrackedMapKey, TrackedMapValue>::iterator getCurrentElementIter()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			setCurrentElementIfNotSet();	// safety: set the value of currentTrackedMapElement to the beginning of the map, if it isn't set.
			return currentTrackedMapElement;
		}

		int size() 
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			return int(trackedMap.size());	// cast to int, to make compiler happy (avoiding a warning)
		}

		void clear()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			trackedMap.clear();			// first, empty out the trackedMap.
			resetBeginAndEndIters();	// reset the iters.
			currentTrackedMapElement = trackedMap.end();	// set the currentTrackedMapElement to be the end
			wasSetCurrentElementCalled = false;		// lastly, reset the value of wasSetCurrentElementCalled.
		}

		// erase: NEEDS WORK
		void erase(TrackedMapKey in_trackedMapKeyToErase)
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);

			// first, get size of current map; if the value of this is different after the erase,
			// we know the iterator needs to be reset.
			int preEraseAttemptMapSize = int(trackedMap.size());

			// check if the current element to erase is equal to the current iterator;
			// also, get what the next element would be post erase, if we did it.
			// if it is, we will need to reset the value of currentTrackedMapElement after the erase.

			bool isElementToEraseEqualTocurrentTrackedMapElement = false;
			auto elementToEraseFinder = trackedMap.find(in_trackedMapKeyToErase);
			auto predictedNext = predictNextElementAfterErase();	// no lock_guard should be attempted here, as that is already done at the beginning of this function.
																	// predictedNext will only really need to be used below if the pre erase size was > 2.

			if (currentTrackedMapElement == elementToEraseFinder)
			{
				isElementToEraseEqualTocurrentTrackedMapElement = true;
			}

			trackedMap.erase(in_trackedMapKeyToErase);
			resetBeginAndEndIters();

			// check to see if we need the reset the currentTrackedMapElement.
			// We only need to do this if it's value is equal to the key we're about to erase, as that would make currentTrackedMapElement 
			// invalid. Otherwise, if currentTrackedMapElement is NOT equal to the key we're about to erase -- we are fine, because it will
			// still be valid after this operation.

			// Two conditions must be met: 
			// A.) the size of the trackedMap can't be equal to currentMapSize value above
			// B.) the element that was erased was actually what the currentTrackedMapElement was before the erase
			if
			(
				(preEraseAttemptMapSize != int(trackedMap.size()))
				&&
				(isElementToEraseEqualTocurrentTrackedMapElement == true)
			)
			{
				// at this point, the value of currentTrackedMapElement is invalid, so it must be set again.
				// We do this based on what the value of preEraseAttemptMapSize is.

				// If preEraseAttemptMapSize was > 2, we now have at least 2 elements remaining. So the value
				// of currentTrackedMapElement would be equal to the predicted value returned by
				// the call to predictNextElementAfterErase().
				if (preEraseAttemptMapSize > 2)
				{
					currentTrackedMapElement = predictedNext;
				}

				// If there were exactly 2 elements before the erase,
				// There's only one remaining.
				else if (preEraseAttemptMapSize == 2)
				{
					currentTrackedMapElement = trackedMap.begin();
				}

				// If there was just 1 element in the map, there is now nothing; make the currentCycleElement the end of the map.
				else if (preEraseAttemptMapSize == 1)
				{
					currentTrackedMapElement = trackedMap.end();
				}
			}

			// if the size of the tracked map is now 0, it is back at it's original state, 
			// so the value of wasSetCurrentElementCalled needs to be set back to false.
			if (trackedMap.size() == 0)
			{
				wasSetCurrentElementCalled = false;
			}

		}

		TrackedMapValue& operator[](const TrackedMapKey& in_key)
		{
			// For usage cases such as TrackedMap[x] = value, the "value" would be equivalent to CycleValue&, and the second parameter used in std::make_pair.
			// See this for reference: https://en.cppreference.com/w/cpp/container/map/operator_at
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			auto insertedValueIter = this->trackedMap.insert(std::make_pair(in_key, TrackedMapValue()));	// attempt to insert the key/value pair if it doesn't exist, 
																											// and return the iterator to that
			resetBeginAndEndIters();	// reset the beginning and ending element iterators, before we return
			return (*insertedValueIter.first).second;		// return a reference to the data value that exists at the key value
		}

		void insert(TrackedMapKey in_key, TrackedMapValue in_trackedMapValue)
		{
			// It should be noted, that if the key of the iter returned (we'll call it returnedKey) by getCurrentElementIter()
			// matches the key we are inserting, that the returnedKey will NOT be invalidated -- and it's value will be updated.
			// This lesser known rule of std::map has been tested and verified.

			std::lock_guard<std::mutex> guard(trackedMapMutex);
			trackedMap[in_key] = in_trackedMapValue;
			resetBeginAndEndIters();
		}

		typename std::map<TrackedMapKey, TrackedMapValue>::iterator getNextElement()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			setCurrentElementIfNotSet(); // do this, in case that getCurrentElementIter() is not called before this.
			if (trackedMap.size() >= 2)
			{
				// If the case was met that currentTrackedMapElement == currentElementEndIter, we won't have to do anything,
				// as we'd be at the last of the elements in the map.

				// That is because if we are at the last element in the map, we obviously can't go to the next.
				// Otherwise, if currentTrackedMapElement isn't equal to the end element; just get the next element.

				// Also, if the map size is 1, we don't need to do any modification to currentTrackedMapElement, as that should already be set.

				// Additionally, if there weren't any elements in the map, we wouldn't have to do anything.
				if (currentTrackedMapElement != currentElementEndIter)
				{
					currentTrackedMapElement++;
				}
			}
			return currentTrackedMapElement;
		}

		typename std::map<TrackedMapKey, TrackedMapValue>::iterator getPreviousElement()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			setCurrentElementIfNotSet(); // do this, in case that getCurrentElementIter() is not called before this.
			if (trackedMap.size() >= 2)
			{
				// If the case was met that currentTrackedMapElement == currentElementStartIter, we won't have to do anything, 
				// as we would be at the beginning of the elements in the map.

				// That is because if we are at the first element in the map, we obviously can't go to the previous.

				// Also, if the map size is 1, we don't need to do any modification to currentTrackedMapElement, as that should already be set.

				// Additionally, if there weren't any elements in the map, we wouldn't have to do anything.

				if (currentTrackedMapElement != currentElementStartIter)
				{
					auto reverseIter = findReverseIter(currentTrackedMapElement->first);
					reverseIter++;
					TrackedMapKey foundReverseIterKey = reverseIter->first;
					currentTrackedMapElement = trackedMap.find(foundReverseIterKey);
				}
			}
			return currentTrackedMapElement;
		}

		void setCurrentElementByKey(TrackedMapKey in_keyToSet)
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			currentTrackedMapElement = trackedMap.find(in_keyToSet);
			wasSetCurrentElementCalled = true;
		}

	private:
		std::map<TrackedMapKey, TrackedMapValue> trackedMap;
		typename std::map<TrackedMapKey, TrackedMapValue>::iterator currentElementStartIter;
		typename std::map<TrackedMapKey, TrackedMapValue>::iterator currentTrackedMapElement;
		typename std::map<TrackedMapKey, TrackedMapValue>::iterator currentElementEndIter;
		std::mutex trackedMapMutex;
		bool wasSetCurrentElementCalled = false;

		void setCurrentElementIfNotSet()
		{
			if (wasSetCurrentElementCalled == false)
			{
				currentTrackedMapElement = trackedMap.begin();
				wasSetCurrentElementCalled = true;
			}
		}

		void resetBeginAndEndIters()
		{
			currentElementStartIter = trackedMap.begin();
			if (trackedMap.size() == 1)
			{
				currentElementEndIter = currentElementStartIter;
			}
			else if (trackedMap.size() > 1)
			{
				currentElementEndIter = trackedMap.begin();
				for (int x = 0; x < (trackedMap.size() - 1); x++)
				{
					currentElementEndIter++;
				}
			}
		}

		typename std::map<TrackedMapKey, TrackedMapValue>::reverse_iterator findReverseIter(TrackedMapKey in_keyToSearchFor)
		{
			typename std::map<TrackedMapKey, TrackedMapValue>::reverse_iterator returnIter = trackedMap.rbegin();
			for (; returnIter != trackedMap.rend(); returnIter++)
			{
				if (returnIter->first == in_keyToSearchFor)
				{
					return returnIter;
				}
			}
			return returnIter;
		}

		typename std::map<TrackedMapKey, TrackedMapValue>::iterator predictNextElementAfterErase()
		{
			typename std::map<TrackedMapKey, TrackedMapValue>::iterator predictedElement;
			// check that the currentTrackedMapElement is actually set.
			setCurrentElementIfNotSet();

			// There would be at least 2 elements remaining after the erase.
			if (trackedMap.size() > 2)
			{
				// CASE 1: currentTrackedMapElement isn't equal to the end (next to last) iterator; just get the next element.
				if (currentTrackedMapElement != currentElementEndIter)
				{
					predictedElement = currentTrackedMapElement;
					predictedElement++;
				}

				// CASE 2: currentTrackedMapelement is the ending element; we will have to get the previous element before this one.
				//		   A quick way to do this is to use a reverse iteratror begin (rbegin), then increment it by one, then get the first
				//		   value of that iterator. Then, use that first value in a find on the map (which returns a normal iterator).
				if (currentTrackedMapElement == currentElementEndIter)
				{
					// get the rbegin.
					auto reverseBegin = trackedMap.rbegin();
					reverseBegin++;
					int foundKey = reverseBegin->first;
					auto typicalIterator = trackedMap.find(foundKey);
				}
			}

			// NOTE: these next two else ifs may not be needed; more testing may need to be done.

			// There would be exactly 1 element remaining after the erase.
			else if (trackedMap.size() == 2)
			{
				predictedElement = trackedMap.begin();
			}

			// There would be 0 elements remaining after the erase.
			else if (trackedMap.size() == 1)
			{
				predictedElement = trackedMap.end();
			}

			return predictedElement;
		}
		
};

#endif