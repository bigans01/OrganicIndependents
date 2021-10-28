#pragma once

#ifndef CYCLE_H
#define CYCLE_H

//#include <unordered_set>
#include <map>
#include <mutex>
#include <iostream>

template <typename CycleKey, typename CycleValue> class Cycle {
	public:
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

		CycleValue erase(CycleKey in_elementToErase) {
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
			std::lock_guard<std::mutex> guard(cycleMutex);
			return	(*((this->cycleMap.insert(std::make_pair(in_key, CycleValue()))).first)).second;
		}

		void insert(CycleKey in_key, CycleValue in_insertCycleValuealue)
		{
			std::lock_guard<std::mutex> guard(cycleMutex);
			cycleMap[in_key] = in_insertCycleValuealue;
			resetBeginAndEndIters();
		}

	private:
		std::map<CycleKey, CycleValue> cycleMap;
		typename std::map<CycleKey, CycleValue>::iterator currentBeginIter;
		typename std::map<CycleKey, CycleValue>::iterator currentEndIter;
		std::mutex cycleMutex;

		void resetBeginAndEndIters()
		{
			currentBeginIter = cycleMap.begin();
			currentEndIter = cycleMap.end();
		}
};

#endif