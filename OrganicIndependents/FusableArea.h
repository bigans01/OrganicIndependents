#pragma once

#ifndef FUSABLEAREA_H
#define FUSABLEAREA_H

#include "ECBPolyPoint.h"
#include "OperableIntSet.h"
#include "FusableAreaState.h"

class FusableArea
{
	public:
		FusableArea() {};
		FusableArea(int in_originAtomID, ECBPolyPoint in_pointAgent) :
			pointAgent(in_pointAgent)
		{
			atomicIdRegister += in_originAtomID;	// when initially constructed, we must always add the ID of the originating atom.
		}

		FusableArea& operator+= (const FusableArea& in_fusableAreaB)
		{
			atomicIdRegister += in_fusableAreaB.atomicIdRegister;	// copy the atomic ID register values from the other FusableArea
			if (atomicIdRegister.size() > 1)
			{
				std::cout << "!> currentAreaState is now FUSED. " << std::endl;
				currentAreaState = FusableAreaState::FUSED;
			}
			return *this;
		}

		bool operator==(const FusableArea& in_fusableAreaB)	// a FusableArea is considered "equal" to another FusableArea, if the pointAgents have the same value.
		{
			return pointAgent == in_fusableAreaB.pointAgent;
		}

		void printAtomicIDRegister()
		{
			std::cout << "Registered PAtom IDs for this Fusable Area:" << std::endl;
			auto atomicIDsBegin = atomicIdRegister.begin();
			auto atomicIDsEnd = atomicIdRegister.end();
			for (; atomicIDsBegin != atomicIDsEnd; atomicIDsBegin++)
			{
				std::cout << *atomicIDsBegin << std::endl;
			}
		}

	private:
		ECBPolyPoint pointAgent;
		FusableAreaState currentAreaState = FusableAreaState::UNFUSED;	// always UNFUSED by default
		OperableIntSet atomicIdRegister;	// stores a record of all Atoms that share this space
};

#endif
