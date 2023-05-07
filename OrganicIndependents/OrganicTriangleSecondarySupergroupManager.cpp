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

						// If the block is in a BLOCK_NORMAL_FILLED state, we should no longer append triangles to it, as these would be unnecessary,
						// since BLOCK_NORMAL_FILLED means all 6 sides of the block have been filled (usually to account for incalculable blocks)
						auto filledBlockCheck = blockRef->getBlockMode();
						if (filledBlockCheck != BlockSubType::BLOCK_NORMAL_FILLED)
						{
							// As long as the subtype of the block isn't BlockSubType::BLOCK_NORMAL_FILLED, 
							// we should be ok to add data. However, it's entirely possible in the next call to 
							// setBlockMode below, that this will become BlockSubType::BLOCK_NORMAL_FILLED;
							// At the same time, if this condition is met, it also means that the block will be entirely 
							// filled on all 6 sides in block of code.

							// Set the current blockRef to be the BlockSubMode from the group.
							blockRef->setBlockMode(currentGroup.second.groupSubType);

							// Remember, if the currentGroup was filled because incalculable (aka BlockSubType::BLOCK_NORMAL_FILLED), 
							// the block must first be reset before anything else.
							if (currentGroup.second.getFilledBecauseIncalculableValue() == true)
							{
								*in_totalTrianglesRef -= blockRef->resetBlock();	// subtract the number of triangles that were in the block, and reset it.
							}

							// Insert the fan group, but remember to fetch the number of triangles.
							*in_totalTrianglesRef += blockRef->insertFanGroup(currentGroup.second);

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

					// Remember, if the currentGroup was filled because incalculable, the block must first be reset before anything else.
					if (currentGroup.second.getFilledBecauseIncalculableValue() == true)
					{
						blockRef->resetBlock();	// reset the block, before inserting.
					}

					// Cycle through each fan in the group.
					// Insert the fan group. We don't care about the number of triangles here, as that data isn't necessary/used.
					blockRef->insertFanGroup(currentGroup.second);
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