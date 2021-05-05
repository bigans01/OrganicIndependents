#include "stdafx.h"
#include "ECBBorder.h"

EnclaveKeyDef::EnclaveKey ECBBorder::getKey()
{
	EnclaveKeyDef::EnclaveKey keyToReturn;
	keyToReturn.x = int(move_x);
	keyToReturn.y = int(move_y);
	keyToReturn.z = int(move_z);
	return keyToReturn;
}