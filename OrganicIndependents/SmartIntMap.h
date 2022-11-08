#pragma once

#ifndef SMARTINTMAP_H
#define SMARTINTMAP_H

// Created on 4/15/2021
//
// Purpose: this class attempts to keep all of the int values in an std::map with an int key in a contiguous block; i.e, there are no gaps 
// in the sequence of keys. In other words, the key values would be 0,1,2,3,4 -- instead of 0,30,60,80. This is done by using the call "getNextAvailableKey"
// which returns the first "missing" key it finds in the map.


template <typename V> class SmartIntMap {
	public:
		typename std::map<int, V>::iterator begin() {  return smap.begin(); }
		typename std::map<int, V>::reverse_iterator rbegin() { return smap.rbegin(); }
		typename std::map<int, V>::iterator end() { return smap.end(); }
		typename std::map<int, V>::iterator find(int in_indexToFind) { return smap.find(in_indexToFind); }
		void erase(int in_indexToErase) { smap.erase(in_indexToErase); }
		int size() { return int(smap.size()); }
		int getNextAvailableKey();

		// really need to understand why this commented out block doesnt' work (research on tesmplate constructors? -- 4/15/2021)
		/*
		V operator[](const int in_key) 
		{
			//return (try_emplace(in_vVal).first->second);
			//return 3.0f;
			return (*((this->smap.insert(make_pair(in_key, V()))).first)).second;
		}
		*/

		V& operator[](const int& in_key)
		{
			return	(*((this->smap.insert(std::make_pair(in_key, V()))).first)).second;
		}

	private:
		std::map<int, V> smap;
};

template <typename V> int SmartIntMap<V>::getNextAvailableKey()
{
	int nextAvailableKeyToUse = 0;					// the return value.
	if (smap.size() == 0)
	{
		return 0;	// if the smap is empty, return 0.
	}
	else if (smap.size() != 0)
	{
		// check whether or a key at 0 exists.
		auto zeroFinder = smap.find(0);
		if (zeroFinder == smap.end())	// it wasn't found.
		{
			return 0;	// return index 0 as the one to use.
		}
		else
		{
			int currentTick = 0;
			int currentKeyValue = smap.begin()->first;
			auto smapBegin = smap.begin();
			auto smapEnd = smap.end();
			bool wasGapFound = false;
			for (; smapBegin != smapEnd; smapBegin++)
			{
				if (currentTick != 0)
				{
					if (smapBegin->first != (currentKeyValue + 1))	// if the current key we are on, isn't equal to +1 of the previous there's a "gap"
					{
						nextAvailableKeyToUse = currentKeyValue + 1;	// have the return value to use be this.
						wasGapFound = true;
						break;
					}
				}
				currentKeyValue = smapBegin->first;	// update the currentKeyValue
				currentTick++;
			}

			if (wasGapFound == false)	// there wasn't any gap found; use the index equivalent to smap.size().
			{
				nextAvailableKeyToUse = int(smap.size());
			}
		}
	}
	return nextAvailableKeyToUse;
}

#endif
