#pragma once

#ifndef DEBUGOPTIONSET_H
#define DEBUGOPTIONSET_H

#include <set>
#include "DebugOption.h"
#include "PolyDebugLevel.h"

class DebugOptionSet
{
	public:
		DebugOptionSet() {};
		DebugOptionSet(std::set<DebugOption> in_set) : debugOptionSet(in_set) {};

		DebugOptionSet& operator=(const std::set<DebugOption> in_set)
		{
			debugOptionSet = in_set;
			return *this;
		}

		DebugOptionSet& operator+=(const DebugOption& in_optionToAdd)
		{
			debugOptionSet.insert(in_optionToAdd);
			return *this;
		}

		DebugOptionSet& operator+=(const DebugOptionSet& set_b)
		{
			auto otherSetBegin = set_b.debugOptionSet.begin();
			auto otherSetEnd = set_b.debugOptionSet.end();
			for (; otherSetBegin != otherSetEnd; otherSetBegin++)
			{
				debugOptionSet.insert(*otherSetBegin);
			}
			return *this;
		}

		DebugOptionSet& operator-=(const DebugOptionSet& set_b)
		{
			auto otherSetBegin = set_b.debugOptionSet.begin();
			auto otherSetEnd = set_b.debugOptionSet.end();
			for (; otherSetBegin != otherSetEnd; otherSetBegin++)
			{
				debugOptionSet.erase(*otherSetBegin);
			}
			return *this;
		}

		std::set<DebugOption>::iterator begin() { return debugOptionSet.begin(); };
		std::set<DebugOption>::iterator end() { return debugOptionSet.end(); };
		std::set<DebugOption>::iterator find(DebugOption in_optionToFind) { return debugOptionSet.find(in_optionToFind); };
		std::set<DebugOption> getSet()
		{
			return debugOptionSet;
		}
		bool containsOption(DebugOption in_optionToFind)
		{
			bool wasFound = false;
			auto finder = debugOptionSet.find(in_optionToFind);
			if (finder != debugOptionSet.end())
			{
				wasFound = true;
			}
			return wasFound;
		}

		PolyDebugLevel returnDebugLevelIfFound(DebugOption in_optionToFind)
		{
			PolyDebugLevel returnValue = PolyDebugLevel::NONE;
			auto finder = debugOptionSet.find(in_optionToFind);
			if (finder != debugOptionSet.end())
			{
				returnValue = PolyDebugLevel::DEBUG;
			}
			return returnValue;
		}
	private:
		std::set<DebugOption> debugOptionSet;


};

#endif