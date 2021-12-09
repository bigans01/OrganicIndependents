#pragma once

#ifndef NEIGHBORINGBLOCKLOCATIONSET_H
#define NEIGHBORINGBLOCKLOCATIONSET_H

#include "NeighboringBlockLocation.h"
#include "EnclaveKeyDef.h"
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
		NeighboringBlockLocation calculatePosX(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
												EnclaveKeyDef::EnclaveKey in_originOREKey,
												EnclaveKeyDef::EnclaveKey in_originBlockKey);
		NeighboringBlockLocation calculateNegX(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
												EnclaveKeyDef::EnclaveKey in_originOREKey,
												EnclaveKeyDef::EnclaveKey in_originBlockKey);
	private:
		void determineNeighbors();
											
											
											
};

#endif
