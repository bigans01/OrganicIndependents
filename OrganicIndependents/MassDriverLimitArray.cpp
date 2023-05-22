#include "stdafx.h"
#include "MassDriverLimitArray.h"

void MassDriverLimitArray::updateLimit(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	int x_coord = (in_enclaveKey.x * 4) + in_blockKey.x;
	int y_coord = (in_enclaveKey.y * 4) + in_blockKey.y;
	int z_coord = (in_enclaveKey.z * 4) + in_blockKey.z;

	// fetch the value from the X/Z coord
	char currentY = limitArray[x_coord][z_coord].maxY;

	// fetch the new value to insert
	char newY = char(y_coord);

	/*
	if
	(
		(in_enclaveKey.x == 0)
		&&
		(in_enclaveKey.y == 7)
		&&
		(in_enclaveKey.z == 0)
	)
	{
		//std::cout << "Enclave Key (" << in_enclaveKey.x << "," << in_enclaveKey.y << ", " << in_enclaveKey.z << ") | Block key: (" << in_blockKey.x << ", " << in_blockKey.y << "," << in_blockKey.z << "); Y-coord is: " << y_coord << std::endl;
	}
	*/

	if (newY > currentY)
	{
		limitArray[x_coord][z_coord].maxY = newY;
	}
}

void MassDriverLimitArray::updateLimitDebug(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	int x_coord = (in_enclaveKey.x * 4) + in_blockKey.x;
	int y_coord = (in_enclaveKey.y * 4) + in_blockKey.y;
	int z_coord = (in_enclaveKey.z * 4) + in_blockKey.z;

	// fetch the value from the X/Z coord
	char currentY = limitArray[x_coord][z_coord].maxY;

	// fetch the new value to insert
	char newY = char(y_coord);

	/*
	if
	(
		(in_enclaveKey.x == 0)
		&&
		(in_enclaveKey.y == 7)
		&&
		(in_enclaveKey.z == 0)
	)
	{
		//std::cout << "Enclave Key (" << in_enclaveKey.x << "," << in_enclaveKey.y << ", " << in_enclaveKey.z << ") | Block key: (" << in_blockKey.x << ", " << in_blockKey.y << "," << in_blockKey.z << "); Y-coord is: " << y_coord << std::endl;
	}
	*/

	if (newY > currentY)
	{
		limitArray[x_coord][z_coord].maxY = newY;
	}

	std::cout << "(MassDriverLimitArray::updateLimitDebug) [" << x_coord << "][" << z_coord << "] -> currentY: " << int(currentY) << "| newY: " << int(newY) << " | new value in array: " << int(limitArray[x_coord][z_coord].maxY) << std::endl;
}