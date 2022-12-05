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
				for (auto& wrappedBBFanBegin : tertiariesBegin.second.triangleMap)
				{
					EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(wrappedBBFanBegin.first);
					auto blockFinder = (*in_skeletonMapRef).find(wrappedBBFanBegin.first);
					if (blockFinder == (*in_skeletonMapRef).end())		// if it isn't in the skeleton map, we'll display it.
					{
						int keyToSingle = PolyUtils::convertBlockCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
						EnclaveBlock* blockRef = &(*in_enclaveBlockMapRef)[keyToSingle];
						blockRef->insertBBFanFromRawEnclave(wrappedBBFanBegin.second);
						*in_totalTrianglesRef += wrappedBBFanBegin.second.poly.numberOfTertiaries;	// increment the number of total_triangles, for when we eventually load into an Enclave itself.
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
				for (auto& wrappedBBFanBegin : tertiariesBegin.second.triangleMap)
				{
					EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(wrappedBBFanBegin.first);
					int keyToSingle = PolyUtils::convertBlockCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
					EnclaveBlock* blockRef = &(*in_enclaveBlockMapRef)[keyToSingle];
					blockRef->insertBBFanFromRawEnclave(wrappedBBFanBegin.second);

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