#include "stdafx.h"
#include "OrganicTriangleSecondarySupergroupManager.h"

void OrganicTriangleSecondarySupergroupManager::insertSecondary(int in_polyID, int in_supergroupID, OrganicTriangleSecondary in_organicTriangleSecondary)
{
	secondarySupergroups[in_polyID].insertOrganicTriangleSecondary(in_supergroupID, in_organicTriangleSecondary);
}

void OrganicTriangleSecondarySupergroupManager::generateBlockTrianglesFromSecondaries(std::map<int, EnclaveBlockSkeleton>* in_skeletonMapRef,
	std::map<int, EnclaveBlock>* in_enclaveBlockMapRef,
	int* in_totalTrianglesRef)
{
	for (auto& supergroupsBegin : secondarySupergroups)		// iterate through each super group.
	{
		for (auto& subgroupsBegin : supergroupsBegin.second.secondarySubgroups)				// for each supergroup, check each subgroup.
		{
			for (auto& tertiariesBegin : subgroupsBegin.second.enclaveTriangleTertiaryContainer)		// for each subgroup, iterate through each tertiary.
			{
				for (auto& currentGroup : tertiariesBegin.second.triangleMap)
				{
					EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(currentGroup.first);
					auto blockFinder = (*in_skeletonMapRef).find(currentGroup.first);
					if (blockFinder == (*in_skeletonMapRef).end())		// if it isn't in the skeleton map, we'll display it.
					{
						int keyToSingle = PolyUtils::convertBlockCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
						EnclaveBlock* blockRef = &(*in_enclaveBlockMapRef)[keyToSingle];

						// Set the current blockRef to be the BlockSubMode from the group.
						blockRef->setBlockMode(currentGroup.second.groupSubType);

						// Cycle through each fan in the group.
						for (auto& currentFanInGroup : currentGroup.second.fans)
						{

							//blockRef->insertBBFanFromRawEnclave(currentGroup.second);
							//*in_totalTrianglesRef += currentGroup.second.poly.numberOfTertiaries;	// increment the number of total_triangles, for when we eventually load into an Enclave itself.

							blockRef->insertBBFanFromRawEnclave(currentFanInGroup);
							*in_totalTrianglesRef += currentFanInGroup.poly.numberOfTertiaries;
						}
					}
				}
			}
		}
	}
}

void OrganicTriangleSecondarySupergroupManager::simulateExposedBlockGeneration(std::map<int, EnclaveBlock>* in_enclaveBlockMapRef)
{
	for (auto& supergroupsBegin : secondarySupergroups)		// iterate through each super group.
	{
		for (auto& subgroupsBegin : supergroupsBegin.second.secondarySubgroups)				// for each supergroup, check each subgroup.
		{
			for (auto& tertiariesBegin : subgroupsBegin.second.enclaveTriangleTertiaryContainer)		// for each subgroup, iterate through each tertiary.
			{
				for (auto& currentGroup : tertiariesBegin.second.triangleMap)
				{
					EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(currentGroup.first);
					int keyToSingle = PolyUtils::convertBlockCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
					EnclaveBlock* blockRef = &(*in_enclaveBlockMapRef)[keyToSingle];

					// Set the current blockRef to be the BlockSubMode from the group.
					blockRef->setBlockMode(currentGroup.second.groupSubType);

					// Cycle through each fan in the group.
					for (auto& currentFanInGroup : currentGroup.second.fans)
					{
						//blockRef->insertBBFanFromRawEnclave(currentGroup.second);

						blockRef->insertBBFanFromRawEnclave(currentFanInGroup);
					}
				}
			}
		}
	}
}

bool OrganicTriangleSecondarySupergroupManager::willSecondariesProduceFans()
{
	bool willSecondariesProduce = false;

	for (auto& supergroupsBegin : secondarySupergroups)		// iterate through each super group.
	{
		for (auto& subgroupsBegin : supergroupsBegin.second.secondarySubgroups)				// for each supergroup, check each subgroup.
		{
			for (auto& tertiariesBegin : subgroupsBegin.second.enclaveTriangleTertiaryContainer)		// for each subgroup, iterate through each tertiary.
			{
				if (tertiariesBegin.second.triangleMap.size() != 0)
				{
					willSecondariesProduce = true;
					break;
				}
			}
		}
	}
	return willSecondariesProduce;
}

std::set<int> OrganicTriangleSecondarySupergroupManager::generateTouchedBlockList()
{
	std::set<int> touchedSet;	// the set to return

	for (auto& supergroupsBegin : secondarySupergroups)		// iterate through each super group.
	{
		for (auto& subgroupsBegin : supergroupsBegin.second.secondarySubgroups)				// for each supergroup, check each subgroup.
		{
			for (auto& tertiariesBegin : subgroupsBegin.second.enclaveTriangleTertiaryContainer)		// for each subgroup, iterate through each tertiary.
			{
				for (auto& wrappedBBFanBegin : tertiariesBegin.second.triangleMap)
				{
					touchedSet.insert(wrappedBBFanBegin.first);	// insert the resulting value into the set	
				}
			}
		}
	}
	return touchedSet;
}