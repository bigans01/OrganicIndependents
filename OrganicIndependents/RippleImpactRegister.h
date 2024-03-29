#pragma once

#ifndef RIPPLEIMPACTREGISTER_H
#define RIPPLEIMPACTREGISTER_H

#include "EnclaveKeyPair.h"
#include "NeighboringBlockLocation.h"
#include "NeighboringBlockRelativeOrientation.h"

/*

RippleImpactRegister:

Description:	stores the neighboring block locations that are produced by a block impact (i.e, a block being deleted or modified).
				Currently being used by the RJPhasedDeleteBlock phased job in OrganicCoreLib, but will probably be used by other classes/functions
				at a later date.

*/

class RippleImpactRegister
{
	public:
		void insertImpactedBlock(EnclaveKeyDef::EnclaveKey in_blueprintKey,
			EnclaveKeyDef::EnclaveKey in_oreKey,
			EnclaveKeyDef::EnclaveKey in_blockKey,
			BoundaryOrientation in_boundaryOrientation)
		{
			impacts[in_blueprintKey].impactedOREs[in_oreKey].insertBlockKey(in_blockKey, in_boundaryOrientation);
		}

		void removeImpactedBlock(EnclaveKeyDef::EnclaveKey in_blueprintKey,
			EnclaveKeyDef::EnclaveKey in_oreKey,
			EnclaveKeyDef::EnclaveKey in_blockKey)
		{
			impacts[in_blueprintKey].impactedOREs[in_oreKey].eraseBlockKey(in_blockKey);

			// check if the RippleImpactedORE has any more blocks; if it does not, we need to erase it.
			if (impacts[in_blueprintKey].impactedOREs[in_oreKey].impactedBlockOrientations.size() == 0)
			{
				impacts[in_blueprintKey].impactedOREs.erase(in_oreKey);
			}

			// after checking for any removable OREs, we should check to see for removable blueprints.
			// A blueprint should be removed if there are no remaining OREs in the map.
			if (impacts[in_blueprintKey].impactedOREs.size() == 0)
			{
				impacts.erase(in_blueprintKey);
			}
		}

		BoundaryOrientation lookupOrientation(EnclaveKeyDef::EnclaveKey in_blueprintKey,
			EnclaveKeyDef::EnclaveKey in_oreKey,
			EnclaveKeyDef::EnclaveKey in_blockKey)
		{
			return impacts[in_blueprintKey].impactedOREs[in_oreKey].fetchOrientationForKey(in_blockKey);
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchImpactedBlueprints()	// returns a vector of impacted blueprint keys.
		{
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> returnSet;
			for (auto& c : impacts)
			{
				returnSet.insert(c.first);
			}
			return returnSet;
		}

		std::vector<EnclaveKeyPair> fetchImpactedOREs()	// returns a vector of blueprint and ORE key pairs
		{
			std::vector<EnclaveKeyPair> returnVector;
			for (auto& currentBlueprint : impacts)
			{
				for (auto& currentORE : currentBlueprint.second.impactedOREs)
				{
					EnclaveKeyPair currentPair(currentBlueprint.first, currentORE.first);
					returnVector.push_back(currentPair);
				}
			}
			return returnVector;
		}

		std::vector<NeighboringBlockRelativeOrientation> fetchRelativeBlockOrientations()	// produces a vector of all orientations found within the impact register, 
		{
			std::vector<NeighboringBlockRelativeOrientation> acquiredOrientations;
			for (auto& currentBlueprint : impacts)
			{
				for (auto& currentORE : currentBlueprint.second.impactedOREs)
				{
					for (auto& currentBlockOrientation : currentORE.second.impactedBlockOrientations)
					{
						NeighboringBlockLocation currentLocation(currentBlueprint.first, currentORE.first, currentBlockOrientation.first);
						NeighboringBlockRelativeOrientation currentOrientation(currentLocation, currentBlockOrientation.second);
						acquiredOrientations.push_back(currentOrientation);
					}
				}
			}
			return acquiredOrientations;
		}

		// structs internal and local to this class only; must be declared in this order.
		struct RippleImpactedORE
		{
			std::unordered_map<EnclaveKeyDef::EnclaveKey, BoundaryOrientation, EnclaveKeyDef::KeyHasher> impactedBlockOrientations;
			void insertBlockKey(EnclaveKeyDef::EnclaveKey in_blockKey, BoundaryOrientation in_boundaryOrientation)
			{
				impactedBlockOrientations[in_blockKey] = in_boundaryOrientation;
			};

			void eraseBlockKey(EnclaveKeyDef::EnclaveKey in_blockKey)
			{
				impactedBlockOrientations.erase(in_blockKey);
			}

			BoundaryOrientation fetchOrientationForKey(EnclaveKeyDef::EnclaveKey in_blockKey)
			{
				return impactedBlockOrientations[in_blockKey];
			}
		};
			
		struct RippleImpactedBlueprint
		{
			std::unordered_map<EnclaveKeyDef::EnclaveKey, RippleImpactedORE, EnclaveKeyDef::KeyHasher> impactedOREs;
		};
		
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RippleImpactedBlueprint, EnclaveKeyDef::KeyHasher> impacts;
};

#endif
