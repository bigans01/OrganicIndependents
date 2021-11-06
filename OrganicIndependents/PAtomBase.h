#pragma once

#ifndef PATOMBASE_H
#define PATOMBASE_H

#include <unordered_map>
#include "EnclaveKeyDef.h"
#include "FusableArea.h"
#include "ECBPolyPoint.h"
#include "PAtomState.h"

class PAtomBase
{
	public:
		void materialize(int in_originalMassID, ECBPolyPoint in_atomCorePoint, float in_expansionInterval)
		{
			originalMassID = in_originalMassID;
			atomCorePoint = in_atomCorePoint;
			expansionInterval = in_expansionInterval;
		}

		void detachFusableArea(FusableArea in_fusableArea)
		{
			bool wasDetachableAreaFound = false;
			EnclaveKeyDef::EnclaveKey removableKey;
			auto existingFusablesBegin = fusableAreaMap.begin();
			auto existingFusablesEnd = fusableAreaMap.end();
			for (; existingFusablesBegin != existingFusablesEnd; existingFusablesBegin++)
			{
				if (in_fusableArea == existingFusablesBegin->second)	// match was found
				{
					wasDetachableAreaFound = true;
					removableKey = existingFusablesBegin->first;
				}
			}

			if (wasDetachableAreaFound == true)
			{
				// as soon as an area is removed, it means a bond between another atom must exist.
				fusableAreaMap.erase(removableKey);
				atomState = PAtomState::BONDED;
			}
		}

		void insertFusableAreaAtKey(EnclaveKeyDef::EnclaveKey in_enclaveKey, ECBPolyPoint in_fusedPoint)
		{
			FusableArea newArea(originalMassID, in_fusedPoint);
			fusableAreaMap[in_enclaveKey] = newArea;
		}

		virtual void expand() = 0;
	protected:
		friend class PMass;
		int originalMassID = 0;				// the original mass ID of this instance of PAtomBase, 
		ECBPolyPoint atomCorePoint;			// the "core point" of the atom's nucleus
		float expansionInterval = 0.0f;		// the expansion interval that represents how much the atom expands whenever the expand() function is called.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FusableArea, EnclaveKeyDef::KeyHasher> fusableAreaMap;
		PAtomState atomState = PAtomState::UNBONDED;	// an atom will always start out as UNBONDED until BONDS with another atom.
														// Which atoms are bonded-to isn't kept track here.
};

#endif
