#include "stdafx.h"
#include "BlueprintScanningCuboid.h"

void BlueprintScanningCuboid::fillValuesAccordingToCenter()
{
	int centerKeyOffset = dimension / 2;
	EnclaveKeyDef::EnclaveKey lowerNWCornerKey = currentCenterKey;	// first, copy the center key.
	lowerNWCornerKey.x -= centerKeyOffset;							// adjust the lower NW corner
	lowerNWCornerKey.y -= centerKeyOffset;							// ""
	lowerNWCornerKey.z -= centerKeyOffset;							// ""
	for (int x = 0; x < dimension; x++)
	{
		for (int y = 0; y < dimension; y++)
		{
			for (int z = 0; z < dimension; z++)
			{
				int currentBufferElement = translateXYZToSingle(x, y, z);
				EnclaveKeyDef::EnclaveKey keyToInsert = lowerNWCornerKey;	// copy the current value of the lowerNWCornerKey, so that we may copy it and apply the x/y/z offsets.
				keyToInsert.x += x;
				keyToInsert.y += y;
				keyToInsert.z += z;
				keyArray[currentBufferElement] = keyToInsert;
			}
		}
	}
}

int BlueprintScanningCuboid::translateXYZToSingle(int x, int y, int z)
{
	int z_axis = z * (dimension * dimension);
	int y_axis = y * (dimension);
	return z_axis + y_axis + x;
}