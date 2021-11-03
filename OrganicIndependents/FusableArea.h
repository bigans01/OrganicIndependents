#pragma once

#ifndef FUSABLEAREA_H
#define FUSABLEAREA_H

#include "ECBPolyPoint.h"
#include "OperableIntSet.h"
#include "FusableAreaState.h"

class FusableArea
{
	public:
		FusableArea(int in_originAtomID, ECBPolyPoint in_pointAgent) :
			pointAgent(in_pointAgent)
		{
			atomicIdRegister += in_originAtomID;	// when initially constructed, we must always add the ID of the originating atom.
		};

		FusableArea& operator+= (const FusableArea& in_fusableAreaB)
		{
			atomicIdRegister += in_fusableAreaB.atomicIdRegister;	// copy the atomic ID register values from the other FusableArea
			if (atomicIdRegister.size() > 1)
			{
				currentAreaState = FusableAreaState::MULTIPLE;
			}
			return *this;
		}

		bool operator==(const FusableArea& in_fusableAreaB)	// a FusableArea is considered "equal" to another FusableArea, if the pointAgents have the same value.
		{
			return pointAgent == in_fusableAreaB.pointAgent;
		}

	private:
		ECBPolyPoint pointAgent;
		FusableAreaState currentAreaState = FusableAreaState::SINGULAR;	// always singular by default
		OperableIntSet atomicIdRegister;	// stores a record of all Atoms that share this space
};

#endif
