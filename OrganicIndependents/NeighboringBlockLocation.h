#pragma once

#ifndef NEIGHBORINGBLOCKLOCATION_H
#define NEIGHBORINGBLOCKLOCATION_H



class NeighboringBlockLocation
{
	public:
		NeighboringBlockLocation() {};
		NeighboringBlockLocation(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey, EnclaveKeyDef::EnclaveKey in_blockKey) :
			blueprintKey(in_blueprintKey),
			oreKey(in_oreKey),
			blockKey(in_blockKey)
		{};
		EnclaveKeyDef::EnclaveKey blueprintKey;
		EnclaveKeyDef::EnclaveKey oreKey;
		EnclaveKeyDef::EnclaveKey blockKey;

};

#endif
