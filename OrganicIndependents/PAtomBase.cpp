#include "stdafx.h"
#include "PAtomBase.h"

void PAtomBase::materialize(int in_originalMassID, ECBPolyPoint in_atomCorePoint, float in_expansionInterval)
{
	originalMassID = in_originalMassID;
	atomCorePoint = in_atomCorePoint;
	expansionInterval = in_expansionInterval;
}

void PAtomBase::detachFusableArea(FusableArea in_fusableArea)
{
	bool wasDetachableAreaFound = false;
	EnclaveKeyDef::EnclaveKey removableKey;
	auto existingFusablesBegin = fusableAreaMap.begin();
	auto existingFusablesEnd = fusableAreaMap.end();
	for (; existingFusablesBegin != existingFusablesEnd; existingFusablesBegin++)
	{
		if (in_fusableArea == existingFusablesBegin->second)	// match was found
		{
			wasDetachableAreaFound = true;
			removableKey = existingFusablesBegin->first;
		}
	}

	if (wasDetachableAreaFound == true)
	{
		// as soon as an area is removed, it means a bond between another atom must exist.
		fusableAreaMap.erase(removableKey);
		atomState = PAtomState::BONDED;
	}
}

void PAtomBase::insertFusableAreaAtKey(EnclaveKeyDef::EnclaveKey in_enclaveKey, ECBPolyPoint in_fusedPoint)
{
	FusableArea newArea(originalMassID, in_fusedPoint);
	fusableAreaMap[in_enclaveKey] = newArea;
}

void PAtomBase::printRegistersInFusableAreas()
{
	std::cout << "Printing out registers of all fusable areas..." << std::endl;
	auto fusablesBegin = fusableAreaMap.begin();
	auto fusablesEnd = fusableAreaMap.end();
	for (; fusablesBegin != fusablesEnd; fusablesBegin++)
	{
		fusablesBegin->second.printAtomicIDRegister();
	}
}