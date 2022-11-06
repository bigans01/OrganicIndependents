#pragma once

#ifndef NEIGHBORINGBLOCKLOCATIONSET_H
#define NEIGHBORINGBLOCKLOCATIONSET_H

#include "NeighboringBlockLocation.h"
#include "EuclideanDirection3D.h"

class NeighboringBlockLocationSet
{
	public:
		NeighboringBlockLocationSet() {};
		NeighboringBlockLocationSet(EnclaveKeyDef::EnclaveKey in_setBlueprintKey,
									EnclaveKeyDef::EnclaveKey in_setOREKey,
									EnclaveKeyDef::EnclaveKey in_setBlockKey) :
			setBlueprintKey(in_setBlueprintKey),
			setOREKey(in_setOREKey),
			setBlockKey(in_setBlockKey)
		{
			determineNeighbors();
		};
		
		EnclaveKeyDef::EnclaveKey setBlueprintKey;
		EnclaveKeyDef::EnclaveKey setOREKey;
		EnclaveKeyDef::EnclaveKey setBlockKey;

		NeighboringBlockLocation setArray[6];	// an array containing all 6 possible values.

		
		// EAST
		NeighboringBlockLocation calculatePosX(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
												EnclaveKeyDef::EnclaveKey in_originOREKey,
												EnclaveKeyDef::EnclaveKey in_originBlockKey);

		// WEST
		NeighboringBlockLocation calculateNegX(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
												EnclaveKeyDef::EnclaveKey in_originOREKey,
												EnclaveKeyDef::EnclaveKey in_originBlockKey);

		// ABOVE
		NeighboringBlockLocation calculatePosY(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
												EnclaveKeyDef::EnclaveKey in_originOREKey,
												EnclaveKeyDef::EnclaveKey in_originBlockKey);

		// BELOW
		NeighboringBlockLocation calculateNegY(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
												EnclaveKeyDef::EnclaveKey in_originOREKey,
												EnclaveKeyDef::EnclaveKey in_originBlockKey);

		// NORTH
		NeighboringBlockLocation calculatePosZ(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
												EnclaveKeyDef::EnclaveKey in_originOREKey,
												EnclaveKeyDef::EnclaveKey in_originBlockKey);

		// SOUTH
		NeighboringBlockLocation calculateNegZ(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
			EnclaveKeyDef::EnclaveKey in_originOREKey,
			EnclaveKeyDef::EnclaveKey in_originBlockKey);

		NeighboringBlockLocation fetchSetByEuclideanDirection(EuclideanDirection3D in_direction);

		// Boundary orientation
		BoundaryOrientation getBoundaryValueForIndex(int in_index);

	private:
		void determineNeighbors();
											
											
											
};

#endif
