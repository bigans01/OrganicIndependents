#pragma once

#ifndef CYCLE_H
#define CYCLE_H

//#include <unordered_set>
#include <map>
#include <iostream>

template <typename V> class Cycle {
	public:
		typename std::map<int, V>::iterator begin() { return cycleMap.begin(); }
		typename std::map<int, V>::iterator end() { return cycleMap.end(); }
		typename std::map<int, V>::iterator find(V in_indexToFind) { return cycleMap.find(in_indexToFind); }

		void erase(V in_elementToErase) {
			cycleMap.erase(in_elementToErase);
		}
		void size() {
			return cycleMap.size();
		}

		V& operator[](const int& in_key)
		{
			return	(*((this->cycleMap.insert(std::make_pair(in_key, V()))).first)).second;
		}

		void insert(int in_key, V in_insertValue)
		{
			cycleMap[in_key] = in_insertValue;
		}

	private:
		std::map<int, V> cycleMap;
};

#endif