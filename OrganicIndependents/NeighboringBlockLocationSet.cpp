#include "stdafx.h"
#include "NeighboringBlockLocationSet.h"

void NeighboringBlockLocationSet::determineNeighbors()
{

}

NeighboringBlockLocation NeighboringBlockLocationSet::calculatePosX(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
	EnclaveKeyDef::EnclaveKey in_originOREKey,
	EnclaveKeyDef::EnclaveKey in_originBlockKey)
{
	if (in_originOREKey.x == 7)	// we are at the final ORE, going positive X.
	{
		if (in_originBlockKey.x == 3)	// we are at the final ORE, AND the final block going positive X
		{
			in_originOREKey.x = 0;			// because we will move into a new blueprint on positive x, the ore key gets reset to 0.
			in_originBlockKey.x = 0;		// " gets reset to 0.
			in_originBlueprintKey.x += 1;	// " add 1 to blueprint x.
		}
		else   // we are at the final ORE, but NOT the final block
		{
			in_originBlockKey.x += 1;
		}
	}
	else   // we are NOT at the final ORE going positive X, so just do normal operations
	{
		if (in_originBlockKey.x == 3)
		{
			in_originOREKey.x += 1;
			in_originBlockKey.x = 0;
		}
		else
		{
			in_originBlockKey.x += 1;
		}
	}
	return NeighboringBlockLocation(in_originBlueprintKey, in_originOREKey, in_originBlockKey);
}

NeighboringBlockLocation NeighboringBlockLocationSet::calculateNegX(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
	EnclaveKeyDef::EnclaveKey in_originOREKey,
	EnclaveKeyDef::EnclaveKey in_originBlockKey)
{
	if (in_originOREKey.x == 0)	// we are at the first ORE, going negative x.
	{
		if (in_originBlockKey.x == 0)	// we are at the first ORE, and the first block going negative X
		{
			in_originOREKey.x = 7;			// because we will move into a new blueprint on negative X, the ORE key gets reset to 7.
			in_originBlockKey.x = 3;		// " gets reset to 3
			in_originBlueprintKey.x -= 1;	// " subtract 1 from blueprint x.
		}
		else	// we are at the first ORE, but NOT the first block
		{
			in_originBlockKey.x -= 1;
		}
	}
	else
	{
		if (in_originBlockKey.x == 0)
		{
			in_originOREKey.x -= 1;
			in_originBlockKey.x = 3;
		}
		else
		{
			in_originBlockKey.x -= 1;
		}
	}
	return NeighboringBlockLocation(in_originBlueprintKey, in_originOREKey, in_originBlockKey);
}