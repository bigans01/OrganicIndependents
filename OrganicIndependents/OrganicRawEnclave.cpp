#include "stdafx.h"
#include "OrganicRawEnclave.h"

OrganicRawEnclave::OrganicRawEnclave()
{

}

OrganicRawEnclave::OrganicRawEnclave(ORELodState in_ORELodState)
{
	currentLodState = in_ORELodState;
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

void OrganicRawEnclave::updateCurrentAppendedState()
{

	switch (currentAppendedState)
	{
		case (OREAppendedState::NONE):
		{
			currentAppendedState = OREAppendedState::SINGLE_APPEND;
			break;
		}
		case (OREAppendedState::SINGLE_APPEND):
		{
			currentAppendedState = OREAppendedState::MULTIPLE_APPEND;
			break;
		}
	}
	appendedUpdateCount++;
}

void OrganicRawEnclave::setOREasIndependent()
{
	currentDependencyState = OREDependencyState::INDEPENDENT;
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
		currentLodState = ORELodState::FULL;
	}
	return returnValue;
}

bool OrganicRawEnclave::doesOREContainRenderableData()
{
	bool containsData = false;
	switch(currentLodState)
	{
		case ORELodState::LOD_ENCLAVE_SMATTER:
		{
			//containsData = organicTriangleSecondarySGM.willSecondariesProduceFans();
			containsData = skeletonSGM.containsRenderableTriangles();
			break;
		};

		case ORELodState::LOD_ENCLAVE_RMATTER:
		{
			containsData = skeletonSGM.containsRenderableTriangles();
			break;
		};


		case ORELodState::LOD_BLOCK:
		{
			break;
		};

		case ORELodState::FULL:
		{
			containsData = false;
			break;
		}
	}
	return containsData;
}

ORELodState OrganicRawEnclave::getLodState()
{
	return currentLodState;
}

OREAppendedState OrganicRawEnclave::getAppendedState()
{
	return currentAppendedState;
}

OREDependencyState OrganicRawEnclave::getDependencyState()
{
	return currentDependencyState;
}

int OrganicRawEnclave::getNumberOfBlockSkeletons()
{
	return blockSkeletonMap.size();
}

void OrganicRawEnclave::updateOREForRMass()
{
	currentLodState = ORELodState::LOD_ENCLAVE_RMATTER;	// switch to RMatter mode
	currentDependencyState = OREDependencyState::INDEPENDENT;	// all RMatter is INDEPENDENT
	blockSkeletonMap.clear();
}

void OrganicRawEnclave::spawnRenderableBlocks(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	switch (currentLodState)
	{
		case ORELodState::LOD_ENCLAVE_SMATTER:
		{
			// will spawn all renderable blocks via stored EnclaveTriangles.
			spawnEnclaveTriangleContainers(in_mutexRef, in_enclaveKey);
			createBlocksFromOrganicTriangleSecondaries(in_mutexRef);
			break;
		};

		case ORELodState::LOD_ENCLAVE_RMATTER:
		{
			// will spawn all renderable blocks via stored EnclaveTriangles.
			spawnEnclaveTriangleContainers(in_mutexRef, in_enclaveKey);
			createBlocksFromOrganicTriangleSecondaries(in_mutexRef);
			break;
		};

		case ORELodState::LOD_BLOCK:
		{
			break;
		};
		case ORELodState::FULL:
		{

			break;
		}
	}
}

int OrganicRawEnclave::getNumberOfTrianglesByLOD()
{
	int triangleCount = 0;
	switch (currentLodState)
	{
		case ORELodState::LOD_ENCLAVE_SMATTER:
		{
			// if LOD_ENCLAVE, simply look at the skeletonSGM
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
						triangleCount++;
					}
				}
			}
			break;
		};

		case ORELodState::LOD_ENCLAVE_RMATTER:
		{
			// if LOD_ENCLAVE, simply look at the skeletonSGM
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
						triangleCount++;
					}
				}
			}
			break;
		};

		case ORELodState::LOD_BLOCK:
		{
			break;
		};
		case ORELodState::FULL:
		{
			break;
		}
	}
	return triangleCount;
}

int OrganicRawEnclave::getTotalTriangles()
{
	return total_triangles;
}

GroupSetPair OrganicRawEnclave::appendEnclaveTrianglesFromOtherORE(std::mutex* in_mutexRef, OrganicRawEnclave* in_otherEnclave)
{
	GroupSetPair returnPair;
	OperableIntSet preAddSet = getExistingEnclaveTriangleSkeletonContainerTracker();
	//std::cout << "---Set contents, before add: " << std::endl;
	//preAddSet.printSet();
	appendSpawnedEnclaveTriangleSkeletonContainers(in_mutexRef, in_otherEnclave->spawnEnclaveTriangleSkeletonContainers());
	updateCurrentAppendedState();
	OperableIntSet postAddSet = getExistingEnclaveTriangleSkeletonContainerTracker();
	//std::cout << "---Set contents, post add: " << std::endl;
	//postAddSet.printSet();
	postAddSet -= preAddSet;	// subtract what was previously there (preAddSet), from what is there afer the call to spawnEnclaveTriangleSkeletonContainers() above (which goes into postAddSet)
								// to get the new set of polys that were just added.	The old set (preAddSet) would be group 1 (what was previously there), and (postAddSet) would be group 2.
								// When the ORE reformer is called to set up, we can pass these in as the arguments to its constructor.
	//std::cout << "---Set contents, after subtraction: " << std::endl;
	//postAddSet.printSet();
	returnPair.group0Set = preAddSet;
	returnPair.group1Set = postAddSet;
	return returnPair;
}

std::vector<EnclaveTriangle> OrganicRawEnclave::retriveAllEnclaveTrianglesForSupergroup(int in_superGroupID)
{
	// first, build a temporary instance of EnclaveTriangleContainerSupergroupManager
	EnclaveTriangleContainerSupergroupManager tempManager;
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
				tempManager.insertEnclaveTriangleDirectIntoSuperGroup(skeletonSGMBegin->first, currentSkeletonContainerBegin->first, currentSkeletonBegin->first, enclaveTriangle);
			}
		}
	}


	std::vector<EnclaveTriangle> triangleVector;
	auto idFinder = tempManager.enclaveTriangleSupergroups.find(in_superGroupID);
	if (idFinder != tempManager.enclaveTriangleSupergroups.end())
	{
		auto supergroupContainersBegin = idFinder->second.containerMap.begin();
		auto supergroupContainersEnd = idFinder->second.containerMap.end();
		for (; supergroupContainersBegin != supergroupContainersEnd; supergroupContainersBegin++)
		{
			auto currentContainerTrianglesBegin = supergroupContainersBegin->second.triangles.begin();
			auto currentContainerTrianglesEnd = supergroupContainersBegin->second.triangles.end();
			for (; currentContainerTrianglesBegin != currentContainerTrianglesEnd; currentContainerTrianglesBegin++)
			{
				triangleVector.push_back(currentContainerTrianglesBegin->second);
			}
		}
	}
	return triangleVector;
}

void OrganicRawEnclave::printMapData()
{
	if (currentLodState == ORELodState::LOD_ENCLAVE_SMATTER)
	{
		std::cout << "++State: LOD_ENCLAVE_SMATTER " << std::endl;
	}
	else if (currentLodState == ORELodState::LOD_ENCLAVE_RMATTER)
	{
		std::cout << "++State: LOD_ENCLAVE_RMATTER " << std::endl;
	}

	else if (currentLodState == ORELodState::FULL)
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

void OrganicRawEnclave::printTriangleMetadata()
{
	std::cout << "+++++Printing meta data about triangles in this ORE..." << std::endl;
	int numberOfTriangles = 0;

	auto skeletonSGMBegin = skeletonSGM.triangleSkeletonSupergroups.begin();
	auto skeletonSGMEnd = skeletonSGM.triangleSkeletonSupergroups.end();
	for (; skeletonSGMBegin != skeletonSGMEnd; skeletonSGMBegin++)
	{
		auto skelContainerBegin = skeletonSGMBegin->second.skeletonMap.begin();
		auto skelContainerEnd = skeletonSGMBegin->second.skeletonMap.end();
		for (; skelContainerBegin != skelContainerEnd; skelContainerBegin++)
		{
			auto skeletonsBegin = skelContainerBegin->second.skeletons.begin();
			auto skeletonsEnd = skelContainerBegin->second.skeletons.end();
			for (; skeletonsBegin != skeletonsEnd; skeletonsBegin++)
			{
				skeletonsBegin->second.printSkeletonTriangle();
				numberOfTriangles++;
			}
		}
	}

	std::cout << "+++++Finished printing metedata of EnclaveTriangleSkeletons in this ORE; count was: " << numberOfTriangles << std::endl;
}

void OrganicRawEnclave::printMetadata()
{
	std::string appendedState;
	switch (currentAppendedState)
	{
		case OREAppendedState::NONE: { appendedState = "NONE"; break; };
		case OREAppendedState::SINGLE_APPEND: { appendedState = "SINGLE_APPEND"; break; };
		case OREAppendedState::MULTIPLE_APPEND: { appendedState = "MULTIPLE_APPEND"; break; };
	}
	std::cout << "value of currentAppendedState: " << appendedState << std::endl;
	std::cout << "appended update count: " << appendedUpdateCount << std::endl;
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

std::map<int, EnclaveBlock>::iterator OrganicRawEnclave::getBlockMapBeginIter()
{
	return blockMap.begin();
}
std::map<int, EnclaveBlock>::iterator OrganicRawEnclave::getBlockMapEndIter()
{
	return blockMap.end();
}

std::map<int, EnclaveBlock>::iterator OrganicRawEnclave::getSpecificBlockIter(EnclaveKeyDef::EnclaveKey in_blockKey)
{
	int keyToSingle = PolyUtils::convertBlockCoordsToSingle(in_blockKey.x, in_blockKey.y, in_blockKey.z);
	return blockMap.find(keyToSingle);
}

EnclaveTriangleSkeletonSupergroupManager OrganicRawEnclave::spawnEnclaveTriangleSkeletonContainers()
{
	EnclaveTriangleSkeletonSupergroupManager producedGroupManager = etcSGM.produceEnclaveTriangleSkeletons();
	return producedGroupManager;

}

void OrganicRawEnclave::loadSkeletonContainersFromEnclaveContainers()
{
	skeletonSGM = etcSGM.produceEnclaveTriangleSkeletons();
}


void OrganicRawEnclave::appendSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, EnclaveTriangleSkeletonSupergroupManager in_enclaveTriangleSkeletonContainer)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	existingEnclaveTriangleSkeletonContainerTracker += skeletonSGM.appendSkeletonContainers(&in_enclaveTriangleSkeletonContainer);		// when appeneding a new container, keep track of the old group range, and the new group range (will be needed for ORE reforming)
}

void OrganicRawEnclave::reloadSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, EnclaveTriangleSkeletonSupergroupManager in_enclaveTriangleSkeletonContainer)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	skeletonSGM.resetSupergroups();		// clear out existing skeletons, reset the group counter
	existingEnclaveTriangleSkeletonContainerTracker.intSet.clear();		// because we will be reloading the tracker entirely, wipe it clean. Used by OREMatterCollider::extractResultsAndSendToORE in OrganicCoreLib, when an RMatter mass has been produced.
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