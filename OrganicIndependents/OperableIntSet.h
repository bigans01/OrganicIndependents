#pragma once

#ifndef OPERABLEINTSET_H
#define OPERABLEINTSET_H

#include <set>
#include <iostream>

class OperableIntSet
{
	public:
		OperableIntSet() {};
		OperableIntSet(std::set<int> in_set) : intSet(in_set) {};

		OperableIntSet& operator=(const std::set<int> in_set)		// copy from a set of ints
		{
			intSet = in_set;
			return *this;
		}

		OperableIntSet& operator+=(const int& in_intToAdd)			// add an int
		{
			intSet.insert(in_intToAdd);
			return *this;
		}

		OperableIntSet& operator-=(const int& in_intToSubtract)
		{
			intSet.erase(in_intToSubtract);
			return *this;
		}
		
		OperableIntSet& operator+=(const OperableIntSet& set_b)		// add from itself
		{
			auto otherSetBegin = set_b.intSet.begin();
			auto otherSetEnd = set_b.intSet.end();
			for (; otherSetBegin != otherSetEnd; otherSetBegin++)
			{
				intSet.insert(*otherSetBegin);	// insert the value into the set.
			}
			return *this;
		}

		OperableIntSet& operator-=(const OperableIntSet& set_b)		// subtract from itself
		{
			auto otherSetBegin = set_b.intSet.begin();
			auto otherSetEnd = set_b.intSet.end();
			for (; otherSetBegin != otherSetEnd; otherSetBegin++)
			{
				intSet.erase(*otherSetBegin);
			}
			return *this;
		}

		int size()
		{
			return int(intSet.size());
		}

		std::set<int>::iterator begin()
		{
			return intSet.begin();
		}

		std::set<int>::iterator end()
		{
			return intSet.end();
		}


		std::set<int> intSet;
		std::set<int> getSet() 
		{	
			return intSet;	
		}

		void printSet()
		{
			auto setBegin = intSet.begin();
			auto setEnd = intSet.end();
			for (; setBegin != setEnd; setBegin++)
			{
				std::cout << *setBegin << std::endl;
			}
		}
};

#endif
