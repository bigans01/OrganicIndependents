#pragma once

#ifndef NEIGHBORINGBLUEPRINTLOCATION_H
#define NEIGHBORINGBLUEPRINTLOCATION_H



class NeighboringBlueprintLocation
{
	public:
		NeighboringBlueprintLocation() {};
		NeighboringBlueprintLocation(EnclaveKeyDef::EnclaveKey in_blueprintKey) :
			blueprintKey(in_blueprintKey)
		{};
		EnclaveKeyDef::EnclaveKey blueprintKey;
};

#endif
