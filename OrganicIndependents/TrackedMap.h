#pragma once

#ifndef TRACKEDMAP_H
#define TRACKEDMAP_H

#include <map>
#include <mutex>
#include <iostream>

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

		typename std::map<CycleKey, CycleValue>::iterator getCurrentElementIter()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			setCurrentElementIfNotSet();	// safety: set the value of currentCycleElement to the beginning of the map, if it isn't set.
			return currentTrackedMapElement;
		}

		int size() 
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			return trackedMap.size();
		}

		void clear()
		{
			std::lock_guard<std::mutex> guard(trackedMapMutex);
			trackedMap.clear();			// first, empty out the trackedMap.
			resetBeginAndEndIters();	// reset the iters.
			currentCycleElement = trackedMap.end();	// set the currentTrackedMapElement to be the end
			wasSetCurrentElementCalled = false;		// lastly, reset the value of wasSetCurrentElementCalled.
		}

		// erase: NEEDS WORK
		void erase(TrackedMapKey in_trackedMapKey)
		{
			// For usage cases such as cycle[x] = value, the "value" would be equivalent to CycleValue&, and the second parameter used in std::make_pair.
			// See this for reference: https://en.cppreference.com/w/cpp/container/map/operator_at
		}

		TrackedMapValue& operator[](const TrackedMapKey& in_key)
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			auto insertedValueIter = this->trackedMap.insert(std::make_pair(in_key, TrackedMapValue()));	// attempt to insert the key/value pair if it doesn't exist, 
																											// and return the iterator to that
			resetBeginAndEndIters();	// reset the beginning and ending element iterators, before we return
			return (*insertedValueIter.first).second;		// return a reference to the data value that exists at the key value
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
				currentCycleElement = cycleMap.begin();
				wasSetCurrentElementCalled = true;
			}
		}

		void resetBeginAndEndIters()
		{
			currentElementStartIter = trackedMap.begin();
			if (cycleMap.size() == 1)
			{
				currentElementEndIter = currentElementStartIter;
			}
			else if (cycleMap.size() > 1)
			{
				currentElementEndIter = trackedMap.begin();
				for (int x = 0; x < (cycleMap.size() - 1); x++)
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
		
};


#endif