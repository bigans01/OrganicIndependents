#include "stdafx.h"
#include "OrganicRawEnclave.h"

OrganicRawEnclave::OrganicRawEnclave()
{

}

OrganicRawEnclave::OrganicRawEnclave(OrganicRawEnclaveState in_organicRawEnclavestate)
{
	currentState = in_organicRawEnclavestate;
}
void OrganicRawEnclave::insertOrganicTriangleSecondary(int in_polyID, int in_clusterID, OrganicTriangleSecondary in_enclavePolyFractureResults)
{
	organicTriangleSecondarySGM.insertSecondary(in_polyID, in_clusterID, in_enclavePolyFractureResults);
}

void OrganicRawEnclave::insertEnclaveTriangleContainer(int in_polyID, int in_clusterID, EnclaveTriangleContainer in_enclaveTriangleContainer)
{
	etcSGM.insertEnclaveTriangle(in_polyID, in_clusterID, in_enclaveTriangleContainer);
}

void OrganicRawEnclave::insertBlockSkeleton(EnclaveKeyDef::EnclaveKey in_blockKey, EnclaveBlockSkeleton in_blockSkeleton)
{
	int blockCoordsToSingle = PolyUtils::convertBlockCoordsToSingle(in_blockKey.x, in_blockKey.y, in_blockKey.z);
	blockSkeletonMap[blockCoordsToSingle] = in_blockSkeleton;
}

void OrganicRawEnclave::createBlocksFromOrganicTriangleSecondaries(std::mutex* in_mutexRef)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);				// must be used because we will be modifying an OrganicRawEnclave that exists in an EnclaveFractureorganicTriangleSecondarySGM, which itself is in its own map, etc...
	resetBlockData();											// the number of triangles added to the block must be reset.
	organicTriangleSecondarySGM.generateBlockTrianglesFromSecondaries(&blockSkeletonMap, &blockMap, &total_triangles);
}

std::set<int> OrganicRawEnclave::getTouchedBlockList()
{
	std::set<int> touchedSet;	// the set to return
	touchedSet = organicTriangleSecondarySGM.generateTouchedBlockList();
	return touchedSet;
}

bool OrganicRawEnclave::checkIfFull()
{
	bool returnValue = false;
	//std::cout << "blockSkeletonMap size is: " << blockSkeletonMap.size();
	if (blockSkeletonMap.size() == 64)	// a full ORE has exactly 64 skeletons
	{
		returnValue = true;
		// if the ORE is full, we should empty the skeletonSGM, etcSGM, and organicTriangleSecondarySGM's managed contents;
		// However, we want the enclave block skeletons to remain.

		skeletonSGM.triangleSkeletonSupergroups.clear();
		etcSGM.enclaveTriangleSupergroups.clear();
		organicTriangleSecondarySGM.secondarySupergroups.clear();
		currentState = OrganicRawEnclaveState::FULL;
	}
	return returnValue;
}

bool OrganicRawEnclave::doesOREContainRenderableData()
{
	bool containsData = false;
	switch(currentState)
	{
		case OrganicRawEnclaveState::LOD_ENCLAVE:
		{
			//containsData = organicTriangleSecondarySGM.willSecondariesProduceFans();
			containsData = skeletonSGM.containsRenderableTriangles();
			break;
		};
		case OrganicRawEnclaveState::LOD_BLOCK:
		{
			break;
		};
		case OrganicRawEnclaveState::FULL:
		{
			containsData = false;
			break;
		}
	}
	return containsData;
}

OrganicRawEnclaveState OrganicRawEnclave::getState()
{
	return currentState;
}

int OrganicRawEnclave::getNumberOfBlockSkeletons()
{
	return blockSkeletonMap.size();
}

void OrganicRawEnclave::spawnRenderableBlocks(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	switch (currentState)
	{
		case OrganicRawEnclaveState::LOD_ENCLAVE:
		{
			// will spawn all renderable blocks via stored EnclaveTriangles.
			spawnEnclaveTriangleContainers(in_mutexRef, in_enclaveKey);
			createBlocksFromOrganicTriangleSecondaries(in_mutexRef);
			break;
		};
		case OrganicRawEnclaveState::LOD_BLOCK:
		{
			break;
		};
		case OrganicRawEnclaveState::FULL:
		{

			break;
		}
	}
}

int OrganicRawEnclave::getTotalTriangles()
{
	return total_triangles;
}

void OrganicRawEnclave::printMapData()
{
	if (currentState == OrganicRawEnclaveState::LOD_ENCLAVE)
	{
		std::cout << "++State: LOD_ENCLAVE " << std::endl;
	}
	else if (currentState == OrganicRawEnclaveState::FULL)
	{
		std::cout << "++State: FULL " << std::endl;
	}
	std::cout << "Skeleton container map size: " << skeletonSGM.triangleSkeletonSupergroups.size() << std::endl;
	//ECBPolyPoint dumbPoint;
	//dumbPoint.isAllZero();
}

void OrganicRawEnclave::printEnclaveTriangleContainers()
{

	auto skeletonSGMBegin = skeletonSGM.triangleSkeletonSupergroups.begin();
	auto skeletonSGMEnd = skeletonSGM.triangleSkeletonSupergroups.end();
	for (; skeletonSGMBegin != skeletonSGMEnd; skeletonSGMBegin++)
	{
		std::cout << "----> Primal PolyID: " << skeletonSGMBegin->first << std::endl;
		auto skelContainerBegin = skeletonSGMBegin->second.skeletonMap.begin();
		auto skelContainerEnd = skeletonSGMBegin->second.skeletonMap.end();
		for (; skelContainerBegin != skelContainerEnd; skelContainerBegin++)
		{
			std::cout << ">>> SPoly id: " << skelContainerBegin->first << std::endl;
			auto skeletonsBegin = skelContainerBegin->second.skeletons.begin();
			auto skeletonsEnd = skelContainerBegin->second.skeletons.end();
			for (; skeletonsBegin != skeletonsEnd; skeletonsBegin++)
			{
				std::cout << "Points for STriangle with ID " << skeletonsBegin->first << std::endl;
				for (int x = 0; x < 3; x++)
				{
					std::cout << skeletonsBegin->second.points[x].x << ", " << skeletonsBegin->second.points[x].y << ", " << skeletonsBegin->second.points[x].z << std::endl;
				}
			}
		}
	}
}

void OrganicRawEnclave::resetBlockData()
{
	blockMap.clear();		// reset the blocks
	total_triangles = 0;	// reset triangle count
}

EnclaveBlock* OrganicRawEnclave::getBlockRefViaBlockKey(EnclaveKeyDef::EnclaveKey in_key)
{
	int keyToSingle = PolyUtils::convertBlockCoordsToSingle(in_key.x, in_key.y, in_key.z);
	return &blockMap[keyToSingle];
}

EnclaveTriangleSkeletonSupergroupManager OrganicRawEnclave::spawnEnclaveTriangleSkeletonContainers()
{
	EnclaveTriangleSkeletonSupergroupManager producedGroupManager = etcSGM.produceEnclaveTriangleSkeletons();
	return producedGroupManager;

}

void OrganicRawEnclave::appendSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, EnclaveTriangleSkeletonSupergroupManager in_enclaveTriangleSkeletonContainer)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	existingEnclaveTriangleSkeletonContainerTracker += skeletonSGM.appendSkeletonContainers(&in_enclaveTriangleSkeletonContainer);		// when appeneding a new container, keep track of the old group range, and the new group range (will be needed for ORE reforming)
}

void OrganicRawEnclave::spawnEnclaveTriangleContainers(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey)	// change the enclave key logic later...
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);

	//std::cout << ":::::::::: Calling spawn enclave triangle containers... " << std::endl;

	// part 1: inflate the triangle containers
	BorderDataMap borderDataMap; // for getting trace results in enclaves
	BlockBorderLineList blockBorderLineList;


	auto skeletonSGMBegin = skeletonSGM.triangleSkeletonSupergroups.begin();
	auto skeletonSGMEnd = skeletonSGM.triangleSkeletonSupergroups.end();
	for (skeletonSGMBegin; skeletonSGMBegin != skeletonSGMEnd; skeletonSGMBegin++)
	{
		auto currentSkeletonContainerBegin = skeletonSGMBegin->second.skeletonMap.begin();
		auto currentSkeletonContainerEnd = skeletonSGMBegin->second.skeletonMap.end();
		for (; currentSkeletonContainerBegin != currentSkeletonContainerEnd; currentSkeletonContainerBegin++)
		{
			auto currentSkeletonBegin = currentSkeletonContainerBegin->second.skeletons.begin();
			auto currentSkeletonEnd = currentSkeletonContainerBegin->second.skeletons.end();
			for (; currentSkeletonBegin != currentSkeletonEnd; currentSkeletonBegin++)
			{
				EnclaveTriangle enclaveTriangle = OrganicTransformUtils::inflateEnclaveTriangle(currentSkeletonBegin->second);
				etcSGM.insertEnclaveTriangleDirectIntoSuperGroup(skeletonSGMBegin->first, currentSkeletonContainerBegin->first, currentSkeletonBegin->first, enclaveTriangle);
			}
		}
	}

	// part 2: execute the runs for the triangles; delete/move EnclaveTriangles that are invalid, so that they don't run.
	// it should be noted, that if not erased, invalid EnclaveTriangles can still produce BBFan data that secondaries can read. It's just that all of them stop upon the first invalid detection.

	auto etcSGMBegin1 = etcSGM.enclaveTriangleSupergroups.begin();
	auto etcSGMEnd1 = etcSGM.enclaveTriangleSupergroups.end();
	for (etcSGMBegin1; etcSGMBegin1 != etcSGMEnd1; etcSGMBegin1++)
	{
		auto currentTriangleContainerBegin = etcSGMBegin1->second.containerMap.begin();
		auto currentTriangleContainerEnd = etcSGMBegin1->second.containerMap.end();
		for (; currentTriangleContainerBegin != currentTriangleContainerEnd; currentTriangleContainerBegin++)
		{
			auto currentTrianglesBegin = currentTriangleContainerBegin->second.triangles.begin();
			auto currentTrianglesEnd = currentTriangleContainerBegin->second.triangles.end();
			std::set<int> removalSet;	// in case we need to remove a triangle for being INVALID
			for (currentTrianglesBegin; currentTrianglesBegin != currentTrianglesEnd; currentTrianglesBegin++)
			{
				//if (currentTrianglesBegin->first == 0)
				//{
				currentTrianglesBegin->second.executeRun(&blockBorderLineList, &borderDataMap, in_enclaveKey);

				// is this triangle INVALID?
				if (currentTrianglesBegin->second.isTriangleValid == false)
				{


					removalSet.insert(currentTrianglesBegin->first);	// insert the int into the removal set
				}
				//}
			}


			if (!removalSet.empty())
			{
				//std::cout << "Removal set not empty; removing identified invalids...";
				auto removalSetBegin = removalSet.begin();
				auto removalSetEnd = removalSet.end();
				for (; removalSetBegin != removalSetEnd; removalSetBegin++)
				{
					//etcSGMBegin1->second.triangles.erase(*removalSetBegin);		// remove the INVALID triangle from the corresponding container.
				}
			}
		}
	}


	auto etcSGMBegin2 = etcSGM.enclaveTriangleSupergroups.begin();
	auto etcSGMEnd2 = etcSGM.enclaveTriangleSupergroups.end();
	for (etcSGMBegin2; etcSGMBegin2 != etcSGMEnd2; etcSGMBegin2++)
	{

		auto currentTriangleContainerBegin = etcSGMBegin2->second.containerMap.begin();
		auto currentTriangleContainerEnd = etcSGMBegin2->second.containerMap.end();
		for (; currentTriangleContainerBegin != currentTriangleContainerEnd; currentTriangleContainerBegin++)
		{

			OrganicTriangleSecondary container;
			container.loadDataFromEnclaveTriangleContainer(&currentTriangleContainerBegin->second);
			insertOrganicTriangleSecondary(etcSGMBegin2->first, currentTriangleContainerBegin->first, container);
		}

	}

}

OperableIntSet OrganicRawEnclave::getExistingEnclaveTriangleSkeletonContainerTracker()
{
	return existingEnclaveTriangleSkeletonContainerTracker;
}