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
		void materialize(int in_originalMassID, ECBPolyPoint in_atomCorePoint, float in_expansionInterval);
		void detachFusableArea(FusableArea in_fusableArea);
		void insertFusableAreaAtKey(EnclaveKeyDef::EnclaveKey in_enclaveKey, ECBPolyPoint in_fusedPoint);
		void printRegistersInFusableAreas();

		// required virtual functions
		virtual void expand() = 0;		// expands a PAtom's fusable area layer by one -- it's virtual because 2D and 3D are done differently.

		int originalMassID = 0;				// the original mass ID of this instance of PAtomBase, 
	protected:
		friend class PMass;
		ECBPolyPoint atomCorePoint;			// the "core point" of the atom's nucleus
		float expansionInterval = 0.0f;		// the expansion interval that represents how much the atom expands whenever the expand() function is called.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, FusableArea, EnclaveKeyDef::KeyHasher> fusableAreaMap;
		PAtomState atomState = PAtomState::UNBONDED;	// an atom will always start out as UNBONDED until BONDS with another atom.
														// Which atoms are bonded-to isn't kept track here.
};

#endif
