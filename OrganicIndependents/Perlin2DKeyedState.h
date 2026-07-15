#pragma once

#ifndef PERLIN2DKEYEDSTATE_H
#define PERLIN2DKEYEDSTATE_H

#include "EnclaveKeyDef.h"
#include "NGSClusterEntry.h"

/*
* 
* Description: Perlin2DKeyedState is meant to store the NGSCGroupingStatus value of a 2d-key mapped sector in a PerlinCluster;
* it's primary use comes into play in the PerlinClusterMeta::fetchKeyedStates() function, which returns a std::vector of these values, per sector.
* The vector can be interepreted in various ways by the caller.

*/

class Perlin2DKeyedState
{
	public:
		Perlin2DKeyedState() {};
		Perlin2DKeyedState(EnclaveKeyDef::Enclave2DKey in_twoDKey, NGSCGroupingStatus in_twoDKeyEntry) :
			twoDKey(in_twoDKey), twoDKeyEntry(in_twoDKeyEntry)
		{}

		EnclaveKeyDef::Enclave2DKey twoDKey;
		NGSCGroupingStatus twoDKeyEntry;

	private:

};

#endif
