#pragma once

#ifndef ATOMICBONDINGRESULT_H
#define ATOMICBONDINGRESULT_H

#include <mutex>
#include "PAtomBase.h"

class AtomicBondingResult
{
	public:
		AtomicBondingResult() {};
		AtomicBondingResult(std::shared_ptr<PAtomBase> in_leftAtomPtr) :
			leftAtomPtr(in_leftAtomPtr)
		{
			leftAtomID = leftAtomPtr->originalMassID;
			bondingResultExists = true;
		};

		bool bondingResultExists = false;
		int leftAtomID = 0;
		int rightAtomID = 0;
		std::shared_ptr<PAtomBase> leftAtomPtr;

};

#endif
