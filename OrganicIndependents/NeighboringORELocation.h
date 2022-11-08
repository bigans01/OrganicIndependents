#pragma once

#ifndef NEIGHBORINGORELOCATION_H
#define NEIGHBORINGORELOCATION_H



class NeighboringORELocation
{
	public:
		NeighboringORELocation() {};
		NeighboringORELocation(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey) :
			blueprintKey(in_blueprintKey),
			oreKey(in_oreKey)
		{};
		
		EnclaveKeyDef::EnclaveKey blueprintKey;
		EnclaveKeyDef::EnclaveKey oreKey;
};

#endif
