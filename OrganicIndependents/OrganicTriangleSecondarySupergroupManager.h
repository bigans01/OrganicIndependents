#pragma once

#ifndef ORGANICTRIANGLESECONDARYSUPERGROUPMANAGER_H
#define ORGANICTRIANGLESECONDARYSUPERGROUPMANAGER_H

#include <map>
#include "OrganicTriangleSecondarySupergroup.h"
#include "EnclaveBlockSkeleton.h"
#include "EnclaveBlock.h"
#include "PolyUtils.h"
#include "EnclaveKeyDef.h"
#include <set>

class OrganicTriangleSecondarySupergroupManager
{
public:
	std::map<int, OrganicTriangleSecondarySupergroup> secondarySupergroups;
	void insertSecondary(int in_polyID, int in_supergroupID, OrganicTriangleSecondary in_organicTriangleSecondary)
	{
		secondarySupergroups[in_polyID].insertOrganicTriangleSecondary(in_supergroupID, in_organicTriangleSecondary);
	}

	void generateBlockTrianglesFromSecondaries(std::map<int, EnclaveBlockSkeleton>* in_skeletonMapRef,
		std::map<int, EnclaveBlock>* in_enclaveBlockMapRef,
		int* in_totalTrianglesRef)
	{
		auto supergroupsBegin = secondarySupergroups.begin();
		auto supergroupsEnd = secondarySupergroups.end();
		for (; supergroupsBegin != supergroupsEnd; supergroupsBegin++)		// iterate through each super group.
		{
			auto subgroupsBegin = supergroupsBegin->second.secondarySubgroups.begin();
			auto subgroupsEnd = supergroupsBegin->second.secondarySubgroups.end();
			for (; subgroupsBegin != subgroupsEnd; subgroupsBegin++)				// for each supergroup, check each subgroup.
			{
				auto tertiariesBegin = subgroupsBegin->second.enclaveTriangleTertiaryContainer.begin();
				auto tertiariesEnd = subgroupsBegin->second.enclaveTriangleTertiaryContainer.end();
				for (; tertiariesBegin != tertiariesEnd; tertiariesBegin++)		// for each subgroup, iterate through each tertiary.
				{
					auto wrappedBBFanBegin = tertiariesBegin->second.triangleMap.begin();
					auto wrappedBBFanEnd = tertiariesBegin->second.triangleMap.end();
					for (; wrappedBBFanBegin != wrappedBBFanEnd; wrappedBBFanBegin++)
					{
						EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(wrappedBBFanBegin->first);
						auto blockFinder = (*in_skeletonMapRef).find(wrappedBBFanBegin->first);
						if (blockFinder == (*in_skeletonMapRef).end())		// if it isn't in the skeleton map, we'll display it.
						{
							//EnclaveBlock* blockRef = 
							int keyToSingle = PolyUtils::convertBlockCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
							EnclaveBlock* blockRef = &(*in_enclaveBlockMapRef)[keyToSingle];
							blockRef->insertBBFanFromRawEnclave(wrappedBBFanBegin->second);
							*in_totalTrianglesRef += wrappedBBFanBegin->second.poly.numberOfTertiaries;	// increment the number of total_triangles, for when we eventually load into an Enclave itself.
						}
					}
				}
			}
		}
	}

	bool willSecondariesProduceFans()
	{
		bool willSecondariesProduce = false;

		auto supergroupsBegin = secondarySupergroups.begin();
		auto supergroupsEnd = secondarySupergroups.end();
		for (; supergroupsBegin != supergroupsEnd; supergroupsBegin++)		// iterate through each super group.
		{
			auto subgroupsBegin = supergroupsBegin->second.secondarySubgroups.begin();
			auto subgroupsEnd = supergroupsBegin->second.secondarySubgroups.end();
			for (; subgroupsBegin != subgroupsEnd; subgroupsBegin++)				// for each supergroup, check each subgroup.
			{
				auto tertiariesBegin = subgroupsBegin->second.enclaveTriangleTertiaryContainer.begin();
				auto tertiariesEnd = subgroupsBegin->second.enclaveTriangleTertiaryContainer.end();
				for (; tertiariesBegin != tertiariesEnd; tertiariesBegin++)		// for each subgroup, iterate through each tertiary.
				{
					if (tertiariesBegin->second.triangleMap.size() != 0)
					{
						willSecondariesProduce = true;
						break;
					}
				}
			}
		}
		return willSecondariesProduce;
	}

	std::set<int> generateTouchedBlockList()
	{
		std::set<int> touchedSet;	// the set to return
		auto supergroupsBegin = secondarySupergroups.begin();
		auto supergroupsEnd = secondarySupergroups.end();
		for (; supergroupsBegin != supergroupsEnd; supergroupsBegin++)		// iterate through each super group.
		{
			auto subgroupsBegin = supergroupsBegin->second.secondarySubgroups.begin();
			auto subgroupsEnd = supergroupsBegin->second.secondarySubgroups.end();
			for (; subgroupsBegin != subgroupsEnd; subgroupsBegin++)				// for each supergroup, check each subgroup.
			{
				auto tertiariesBegin = subgroupsBegin->second.enclaveTriangleTertiaryContainer.begin();
				auto tertiariesEnd = subgroupsBegin->second.enclaveTriangleTertiaryContainer.end();
				for (; tertiariesBegin != tertiariesEnd; tertiariesBegin++)		// for each subgroup, iterate through each tertiary.
				{
					auto wrappedBBFanBegin = tertiariesBegin->second.triangleMap.begin();
					auto wrappedBBFanEnd = tertiariesBegin->second.triangleMap.end();
					for (; wrappedBBFanBegin != wrappedBBFanEnd; wrappedBBFanBegin++)
					{
						touchedSet.insert(wrappedBBFanBegin->first);	// insert the resulting value into the set
					}
				}
			}
		}
		return touchedSet;
	}
};

#endif
