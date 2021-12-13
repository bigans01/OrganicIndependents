#include "stdafx.h"
#include "NeighboringBlockLocationSet.h"

void NeighboringBlockLocationSet::determineNeighbors()
{
	setArray[0] = calculatePosX(setBlueprintKey, setOREKey, setBlockKey);	// EAST, pos x
	setArray[1] = calculateNegX(setBlueprintKey, setOREKey, setBlockKey);	// WEST, neg x
	setArray[2] = calculatePosY(setBlueprintKey, setOREKey, setBlockKey);	// ABOVE, pos y
	setArray[3] = calculateNegY(setBlueprintKey, setOREKey, setBlockKey);	// BELOW, neg y
	setArray[4] = calculatePosZ(setBlueprintKey, setOREKey, setBlockKey);	// NORTH, neg z
	setArray[5] = calculateNegZ(setBlueprintKey, setOREKey, setBlockKey);	// SOUTH, pos z
}

NeighboringBlockLocation NeighboringBlockLocationSet::fetchSetByEuclideanDirection(EuclideanDirection3D in_direction)
{
	switch (in_direction)
	{
		case EuclideanDirection3D::POS_X: { return setArray[0]; break; }
		case EuclideanDirection3D::NEG_X: { return setArray[1]; break; }
		case EuclideanDirection3D::POS_Y: { return setArray[2]; break; }
		case EuclideanDirection3D::NEG_Y: { return setArray[3]; break; }
		case EuclideanDirection3D::NEG_Z: { return setArray[4]; break; }
		case EuclideanDirection3D::POS_Z: { return setArray[5]; break; }
	}	    
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

NeighboringBlockLocation NeighboringBlockLocationSet::calculatePosY(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
	EnclaveKeyDef::EnclaveKey in_originOREKey,
	EnclaveKeyDef::EnclaveKey in_originBlockKey)
{
	if (in_originOREKey.y == 7)	// we are at the final ORE, going positive y.
	{
		if (in_originBlockKey.y == 3)	// we are at the final ORE, AND the final block going positive y
		{
			in_originOREKey.y = 0;			// because we will move into a new blueprint on positive y, the ore key gets reset to 0.
			in_originBlockKey.y = 0;		// " gets reset to 0.
			in_originBlueprintKey.y += 1;	// " add 1 to blueprint y.
		}
		else   // we are at the final ORE, but NOT the final block
		{
			in_originBlockKey.y += 1;
		}
	}
	else   // we are NOT at the final ORE going positive y, so just do normal operations
	{
		if (in_originBlockKey.y == 3)
		{
			in_originOREKey.y += 1;
			in_originBlockKey.y = 0;
		}
		else
		{
			in_originBlockKey.y += 1;
		}
	}
	return NeighboringBlockLocation(in_originBlueprintKey, in_originOREKey, in_originBlockKey);
}

NeighboringBlockLocation NeighboringBlockLocationSet::calculateNegY(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
	EnclaveKeyDef::EnclaveKey in_originOREKey,
	EnclaveKeyDef::EnclaveKey in_originBlockKey)
{
	if (in_originOREKey.y == 0)	// we are at the first ORE, going negative y.
	{
		if (in_originBlockKey.y == 0)	// we are at the first ORE, and the first block going negative y
		{
			in_originOREKey.y = 7;			// because we will move into a new blueprint on negative y, the ORE key gets reset to 7.
			in_originBlockKey.y = 3;		// " gets reset to 3
			in_originBlueprintKey.y -= 1;	// " subtract 1 from blueprint y.
		}
		else	// we are at the first ORE, but NOT the first block
		{
			in_originBlockKey.y -= 1;
		}
	}
	else
	{
		if (in_originBlockKey.y == 0)
		{
			in_originOREKey.y -= 1;
			in_originBlockKey.y = 3;
		}
		else
		{
			in_originBlockKey.y -= 1;
		}
	}
	return NeighboringBlockLocation(in_originBlueprintKey, in_originOREKey, in_originBlockKey);
}

NeighboringBlockLocation NeighboringBlockLocationSet::calculatePosZ(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
	EnclaveKeyDef::EnclaveKey in_originOREKey,
	EnclaveKeyDef::EnclaveKey in_originBlockKey)
{
	if (in_originOREKey.z == 7)	// we are at the final ORE, going positive z.
	{
		if (in_originBlockKey.z == 3)	// we are at the final ORE, AND the final block going positive z
		{
			in_originOREKey.z = 0;			// because we will move into a new blueprint on positive z, the ore key gets reset to 0.
			in_originBlockKey.z = 0;		// " gets reset to 0.
			in_originBlueprintKey.z += 1;	// " add 1 to blueprint z.
		}
		else   // we are at the final ORE, but NOT the final block
		{
			in_originBlockKey.z += 1;
		}
	}
	else   // we are NOT at the final ORE going positive z, so just do normal operations
	{
		if (in_originBlockKey.z == 3)
		{
			in_originOREKey.z += 1;
			in_originBlockKey.z = 0;
		}
		else
		{
			in_originBlockKey.z += 1;
		}
	}
	return NeighboringBlockLocation(in_originBlueprintKey, in_originOREKey, in_originBlockKey);
}

NeighboringBlockLocation NeighboringBlockLocationSet::calculateNegZ(EnclaveKeyDef::EnclaveKey in_originBlueprintKey,
	EnclaveKeyDef::EnclaveKey in_originOREKey,
	EnclaveKeyDef::EnclaveKey in_originBlockKey)
{
	if (in_originOREKey.z == 0)	// we are at the first ORE, going negative z.
	{
		if (in_originBlockKey.z == 0)	// we are at the first ORE, and the first block going negative z
		{
			in_originOREKey.z = 7;			// because we will move into a new blueprint on negative z, the ORE key gets reset to 7.
			in_originBlockKey.z = 3;		// " gets reset to 3
			in_originBlueprintKey.z -= 1;	// " subtract 1 from blueprint z.
		}
		else	// we are at the first ORE, but NOT the first block
		{
			in_originBlockKey.z -= 1;
		}
	}
	else
	{
		if (in_originBlockKey.z == 0)
		{
			in_originOREKey.z -= 1;
			in_originBlockKey.z = 3;
		}
		else
		{
			in_originBlockKey.z -= 1;
		}
	}
	return NeighboringBlockLocation(in_originBlueprintKey, in_originOREKey, in_originBlockKey);
}