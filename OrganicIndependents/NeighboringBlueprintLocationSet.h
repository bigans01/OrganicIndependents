#pragma once

#ifndef NEIGHBORINGBLUEPRINTLOCATIONSET_H
#define NEIGHBORINGBLUEPRINTLOCATIONSET_H

#include "NeighboringBlueprintLocation.h"


class NeighboringBlueprintLocationSet
{
	public:
		NeighboringBlueprintLocationSet() {};
		NeighboringBlueprintLocationSet(EnclaveKeyDef::EnclaveKey in_setBlueprintKey) :
			setBlueprintKey(in_setBlueprintKey)
		{
			determineNeighbors();
		};

		EnclaveKeyDef::EnclaveKey setBlueprintKey;

		NeighboringBlueprintLocation setArray[6];
	private:
		void determineNeighbors();
};

#endif
