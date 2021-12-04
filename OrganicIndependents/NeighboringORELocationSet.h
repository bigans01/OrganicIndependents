#pragma once

#ifndef NEIGHBORINGORELOCATIONSET_H
#define NEIGHBORINGORELOCATIONSET_H

#include "NeighboringORELocation.h"
#include "EnclaveKeyDef.h"

class NeighboringORELocationSet
{
	public:
		NeighboringORELocationSet() {};
		NeighboringORELocationSet(EnclaveKeyDef::EnclaveKey in_setBlueprintKey,
								  EnclaveKeyDef::EnclaveKey in_setOREKey) :
			setBlueprintKey(in_setBlueprintKey),
			setOREKey(in_setOREKey)
		{
			detrmineNeighbors();
		}
		EnclaveKeyDef::EnclaveKey setBlueprintKey;
		EnclaveKeyDef::EnclaveKey setOREKey;

		NeighboringORELocation setArray[6];
	private:
		void detrmineNeighbors();

};

#endif
