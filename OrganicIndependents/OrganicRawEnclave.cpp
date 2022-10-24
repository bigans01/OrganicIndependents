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

void OrganicRawEnclave::insertOrganicTriangleSecondaryIntoRefedManager(OrganicTriangleSecondarySupergroupManager* in_refedManager,
	int in_polyID,
	int in_clusterID,
	OrganicTriangleSecondary in_enclavePolyFractureResults)
{
	in_refedManager->insertSecondary(in_polyID, in_clusterID, in_enclavePolyFractureResults);
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
	resetBlockDataAndTriangleCount();							// the number of triangles added to the block must be reset.
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
			auto blocksBegin = blockMap.begin();
			auto blocksEnd = blockMap.end();
			for (; blocksBegin != blocksEnd; blocksBegin++)
			{
				if (blocksBegin->second.getNumberOfTotalTriangles() != 0)
				{
					containsData = true;
					break;
				}
			}
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
	return int(blockSkeletonMap.size());
}

void OrganicRawEnclave::updateOREForRMass()
{
	currentLodState = ORELodState::LOD_ENCLAVE_RMATTER;	// switch to RMatter mode
	currentDependencyState = OREDependencyState::INDEPENDENT;	// all RMatter is INDEPENDENT
	blockSkeletonMap.clear();
	blockSkeletonMap = rMassSolidsMap;	// assumes that setBlockSkeletons has been called before this function.
}

void OrganicRawEnclave::setPendingRMatterSolids(std::mutex* in_mutexRef, Operable3DEnclaveKeySet in_skeletonBlockSet)
{
	// this function will eventually need to be edited to set the metadata for the EnclaveBlockSkeletons that will go into
	// rMassSolidMap. (Noted on 2/15/2022)
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	rMassSolidsMap.clear();	
	auto skeletonsBegin = in_skeletonBlockSet.begin();
	auto skeletonsEnd = in_skeletonBlockSet.end();
	for (; skeletonsBegin != skeletonsEnd; skeletonsBegin++)
	{
		EnclaveBlockSkeleton newSkeleton;
		int blockCoordsToSingle = PolyUtils::convertBlockCoordsToSingle((*skeletonsBegin).x, (*skeletonsBegin).y, (*skeletonsBegin).z);
		rMassSolidsMap[blockCoordsToSingle] = newSkeleton;
	}
}

void OrganicRawEnclave::morphLodToBlock(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	// first, generate the blocks.
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

		case ORELodState::FULL:
		{
			// when morphing from FULL to LOD_BLOCK, all blocks will initially be unexposed.
			produceAllUnexposedBlocks(in_mutexRef);
		};
	}

	currentLodState = ORELodState::LOD_BLOCK;					// switch to block mode
	currentDependencyState = OREDependencyState::INDEPENDENT;	// block mode is always independent
	currentAppendedState = OREAppendedState::NONE;				// appended state isn't applicable when in block mode

	// all maps that don't contain block data should be cleared; there is no need to have this data from this point forward,
	// as any data that exists should come from blocks and blocks alone, even if they have a state of EXPOSED, UNEXPOSED, etc; if this ORE is read from a file, the part of the file for that ORE 
	// needs to only have block data.
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	skeletonSGM.triangleSkeletonSupergroups.clear();
	etcSGM.enclaveTriangleSupergroups.clear();
	organicTriangleSecondarySGM.secondarySupergroups.clear();

}

void OrganicRawEnclave::produceAllUnexposedBlocks(std::mutex* in_mutexRef)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	// std::cout << "(OrganicRawEnclave): producing all unexposed blocks, due to morph from FULL to BLOCK. " << std::endl;
	// go through 0-3, for x/y/z
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			for (int z = 0; z < 4; z++)
			{
				// NOTE: the default material for a skeleton is 0; we will need to add functionality/options
				// to set this later; perhaps a variable defined in the ORE that will auto-set this
				EnclaveBlockSkeleton newSkeleton;
				int blockCoordsToSingle = PolyUtils::convertBlockCoordsToSingle(x, y, z);
				blockSkeletonMap[blockCoordsToSingle] = newSkeleton;
			}
		}
	}
	// std::cout << "(OrganicRawEnclave): Done producing unexposed blocks; size of blockSkeletonMap is now: " << blockSkeletonMap.size() << std::endl;
}

void OrganicRawEnclave::eraseBlock(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef); // lock while deleting the block.
	int blockCoordsToSingle = PolyUtils::convertBlockCoordsToSingle(in_blockKey.x, in_blockKey.y, in_blockKey.z);
	int erasedTriangles = blockMap[blockCoordsToSingle].getNumberOfTotalTriangles();	// get the number of triangles that existed in the block we're about to erase.
	blockMap.erase(blockCoordsToSingle);
	blockSkeletonMap.erase(blockCoordsToSingle);
	total_triangles -= erasedTriangles;
	eraseCounter++;
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
			// if we are in a LOD_BLOCK state, the blocks should already be setup, so there shouldn't be anything to do here. Shoo!
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
			// if LOD_BLOCK, iterate through the block map, get the total triangle count from each block.
			auto blocksBegin = blockMap.begin();
			auto blocksEnd = blockMap.end();
			for (; blocksBegin != blocksEnd; blocksBegin++)
			{
				triangleCount += blocksBegin->second.getNumberOfTotalTriangles();
			}
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

int OrganicRawEnclave::printBlockData(EnclaveKeyDef::EnclaveKey in_blockKey)
{
	int blockCoordsToSingle = PolyUtils::convertBlockCoordsToSingle(in_blockKey.x, in_blockKey.y, in_blockKey.z);
	auto blockFinder = blockMap.find(blockCoordsToSingle);
	if (blockFinder != blockMap.end())
	{
		std::cout << "(OrganicRawEnclave::printBlockData): found EXPOSED block. " << std::endl;
		blockFinder->second.listSecondaries();
	}
	else if (blockFinder == blockMap.end())
	{
		std::cout << "(OrganicRawEnclave::printBlockData): did NOT find EXPOSED block. " << std::endl;

		auto skeletonFinder = blockSkeletonMap.find(blockCoordsToSingle);
		if (skeletonFinder != blockSkeletonMap.end())
		{
			std::cout << "(OrganicRawEnclave::printBlockData): found block as UNEXPOSED (blockSkeletonMap) " << std::endl;
		}
		else
		{
			std::cout << "(OrganicRawEnclave::printBlockData): did NOT find block as UNEXPOSED (blockSkeletonMap) " << std::endl;
		}
	}
	return 0;
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

void OrganicRawEnclave::printEnclaveTriangleContainers(bool in_pauseBetweenTrianglesFlag)
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

			if (in_pauseBetweenTrianglesFlag == true)
			{
				std::cout << "Enter number to iterate to next triangle..." << std::endl;
				int someNumber = 3;
				std::cin >> someNumber;
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

void OrganicRawEnclave::printTrianglesPerBlock()
{
	auto blocksBegin = blockMap.begin();
	auto blocksEnd = blockMap.end();
	for (; blocksBegin != blocksEnd; blocksBegin++)
	{
		auto currentKey = PolyUtils::convertSingleToBlockKey(blocksBegin->first);
		std::cout << "Key "; currentKey.printKey(); std::cout << " has " << blocksBegin->second.getNumberOfBBFans() << " fans. " << std::endl;
	}
}

void OrganicRawEnclave::resetBlockDataAndTriangleCount()
{
	blockMap.clear();		// reset the blocks
	total_triangles = 0;	// reset triangle count
}

EnclaveBlock* OrganicRawEnclave::getBlockRefViaBlockKey(EnclaveKeyDef::EnclaveKey in_key)
{
	int keyToSingle = PolyUtils::convertBlockCoordsToSingle(in_key.x, in_key.y, in_key.z);
	return &blockMap[keyToSingle];
}

void OrganicRawEnclave::instantiateBlockAndRemoveSkeleton(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);	// because we are moving the state of a block from skeleton to "exposed", we need a lock.
	int keyToSingle = PolyUtils::convertBlockCoordsToSingle(in_key.x, in_key.y, in_key.z);
	blockSkeletonMap.erase(keyToSingle);
	EnclaveBlock newBlock;
	blockMap[keyToSingle] = newBlock;
}

void OrganicRawEnclave::instantiateBlockAndRemoveSkeletonIfNonExistent(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef); // we will be checking, and potentially modifying the ORE, so we need a lock.
	int keyToSingle = PolyUtils::convertBlockCoordsToSingle(in_key.x, in_key.y, in_key.z);
	auto existingBlockFinder = blockMap.find(keyToSingle);
	if (existingBlockFinder == blockMap.end())	// it doesn't exist
	{
		blockSkeletonMap.erase(keyToSingle);
		EnclaveBlock newBlock;
		blockMap[keyToSingle] = newBlock;
	}
}

void OrganicRawEnclave::insertVectoredBBFansIntoBlock(std::mutex* in_mutexRef, 
													  std::vector<OrganicWrappedBBFan> in_fanVector, 
													  EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef); // updating the blocks requires a proper lock.
	int keyToSingle = PolyUtils::convertBlockCoordsToSingle(in_key.x, in_key.y, in_key.z);
	for (auto& currentFan : in_fanVector)
	{
		blockMap[keyToSingle].insertBBFanFromRawEnclave(currentFan);
		total_triangles += currentFan.poly.numberOfTertiaries;
	}
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

void OrganicRawEnclave::clearBlockSkeletons(std::mutex* in_mutexRef)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	blockSkeletonMap.clear();
}

bool OrganicRawEnclave::doesBlockSkeletonExistNoMutex(EnclaveKeyDef::EnclaveKey in_blockKey)
{
	bool isASkeleton = false;
	int targetInt = PolyUtils::convertBlockCoordsToSingle(in_blockKey.x, in_blockKey.y, in_blockKey.z);
	auto skeletonFinder = blockSkeletonMap.find(targetInt);
	if (skeletonFinder != blockSkeletonMap.end())
	{
		isASkeleton = true;
	}
	return isASkeleton;
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

void OrganicRawEnclave::removeSkeletonSupergroup(std::mutex* in_mutexRef, int in_supergroupIDToRemove)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	skeletonSGM.eraseSupergroup(in_supergroupIDToRemove);
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

	bool trianglesRemoved = false;
	auto etcSGMBegin1 = etcSGM.enclaveTriangleSupergroups.begin();
	auto etcSGMEnd1 = etcSGM.enclaveTriangleSupergroups.end();
	int etcSGMSize = int(etcSGM.enclaveTriangleSupergroups.size());
	int currentSGMCounter = 0;
	for (etcSGMBegin1; etcSGMBegin1 != etcSGMEnd1; etcSGMBegin1++)
	{
		std::map<int, int> removalMap;
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
				currentTrianglesBegin->second.executeRun(&blockBorderLineList, &borderDataMap, in_enclaveKey, true);

				// is this triangle INVALID?
				if (currentTrianglesBegin->second.isTriangleValid == false)
				{
					//std::cout << "(OrganicRawEnclave): EnclaveTriangle detected as invalid, in call to spawnEnclaveTriangleContainers. Points were:" << std::endl;
					//currentTrianglesBegin->second.printPoints();
					//std::cout << "(Notice): size of etcSGM.enclaveTriangleSupergroups is: " << etcSGMSize << std::endl;
					trianglesRemoved = true;
					removalSet.insert(currentTrianglesBegin->first);	// insert the int into the removal set
					removalMap[currentTriangleContainerBegin->first] = currentTrianglesBegin->first;
					//int removalVal = 3;
					//std::cin >> removalVal;
					//break;
				}
				//}
			}
		}

		if (removalMap.size() != 0)	
		{
			//std::cout << "(OrganicRawEnclave): found some EnclaveTriangles to remove. " << std::endl;
			auto removalsBegin = removalMap.begin();
			auto removalsEnd = removalMap.end();
			for (; removalsBegin != removalsEnd; removalsBegin++)
			{
				etcSGMBegin1->second.containerMap[removalsBegin->first].triangles.erase(removalsBegin->second);
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

void OrganicRawEnclave::printBlockCategorizations()
{
	// print surrounded/cube (UNEXPOSED) blocks (skeletons)
	std::cout << "Printing UNEXPOSED blocks (skeletons): " << std::endl;
	auto blockSkeletonsBegin = blockSkeletonMap.begin();
	auto blockSkeletonsEnd = blockSkeletonMap.end();
	for (; blockSkeletonsBegin != blockSkeletonsEnd; blockSkeletonsBegin++)
	{
		EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(blockSkeletonsBegin->first);
		currentKey.printKey();
		std::cout << std::endl;
	}

	std::cout << "Printing EXPOSED blocks (non-skeletons): " << std::endl;
	auto fetchedExposedBlockMap = produceBlockCopies();

	// iterate through the exposed blocks we just created and print the block's EnclaveKey.
	auto blocksBegin = fetchedExposedBlockMap.begin();
	auto blocksEnd = fetchedExposedBlockMap.end();
	for (; blocksBegin != blocksEnd; blocksBegin++)
	{
		EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(blocksBegin->first);
		currentKey.printKey();
		std::cout << std::endl;
	}
}

std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> OrganicRawEnclave::fetchUnexposedBlockKeys()
{
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> returnSet;
	auto blockSkeletonsBegin = blockSkeletonMap.begin();
	auto blockSkeletonsEnd = blockSkeletonMap.end();
	for (; blockSkeletonsBegin != blockSkeletonsEnd; blockSkeletonsBegin++)
	{
		EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(blockSkeletonsBegin->first);
		returnSet.insert(currentKey);
	}
	return returnSet;
}

std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> OrganicRawEnclave::fetchExposedBlockKeys()
{
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> returnSet;

	// do the following if we are in a ORELodState of LOD_ENCLAVE_SMATTER or LOD_ENCLAVE_RMATTER.
	if
	(
		currentLodState == ORELodState::LOD_ENCLAVE_SMATTER
		||
		currentLodState == ORELodState::LOD_ENCLAVE_RMATTER
	)
	{
		// if there are blocks already populated, check those; we won't need -- and don't want to --
		// call produceBlockCopies() as that is an expensive operation.
		if (blockMap.size() != 0)
		{
			auto loadedBlocksBegin = blockMap.begin();
			auto loadedBlocksEnd = blockMap.end();
			for (; loadedBlocksBegin != loadedBlocksEnd; loadedBlocksBegin++)
			{
				EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(loadedBlocksBegin->first);
				returnSet.insert(currentKey);
			}
		}
		else
		{
			// we must generate block copies to produce this list; we will assume that the data needed to do this is already in the skeletonSGM.
			auto fetchedExposedBlockMap = produceBlockCopies();
			auto blocksBegin = fetchedExposedBlockMap.begin();
			auto blocksEnd = fetchedExposedBlockMap.end();
			for (; blocksBegin != blocksEnd; blocksBegin++)
			{
				EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(blocksBegin->first);
				returnSet.insert(currentKey);
			}
		}
	}

	// otherwise, scan the existing block map if we are in a ORELodState of LOD_BLOCK.
	else if
	(
		currentLodState == ORELodState::LOD_BLOCK
	)
	{
		// any block in the block map, that is "visible" (not implemented yet) would be considered EXPOSED.
		auto exposedBlocksBegin = blockMap.begin();
		auto exposedBlocksEnd = blockMap.end();
		for (; exposedBlocksBegin != exposedBlocksEnd; exposedBlocksBegin++)
		{
			EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(exposedBlocksBegin->first);
			returnSet.insert(currentKey);
		}
	}

	// NOTE: we don't do anything if the ORE's current state is ORELodState::FULL.

	return returnSet;
}

BlockCopyQuery OrganicRawEnclave::queryForBlockCopy(EnclaveKeyDef::EnclaveKey in_blockKey)
{
	// Currently, this function is only used by OrganicSystem::jobCheckIfPolysExistInBlockFace. 
	// It is not designed to be used on OREs that have a value of FULL for the currentLodState.
	// If a function is trying to call this one on a FULL ORE, there is improper logic being used somewhere.

	BlockCopyQuery returnQuery;

	// do the following if we are in a ORELodState of LOD_ENCLAVE_SMATTER or LOD_ENCLAVE_RMATTER.
	if
		(
			currentLodState == ORELodState::LOD_ENCLAVE_SMATTER
			||
			currentLodState == ORELodState::LOD_ENCLAVE_RMATTER
			)
	{
		// if there are blocks already populated, check those; we won't need -- and don't want to --
		// call produceBlockCopies() as that is an expensive operation.
		//std::cout << "(OrganicRawEnclave::queryForBlockCopy): entered RMATTER/SMATTER branch. " << std::endl;
		bool exposedDiscovered = false;
		if (blockMap.size() != 0)
		{
			auto loadedBlocksBegin = blockMap.begin();
			auto loadedBlocksEnd = blockMap.end();
			for (; loadedBlocksBegin != loadedBlocksEnd; loadedBlocksBegin++)
			{
				EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(loadedBlocksBegin->first);
				if (currentKey == in_blockKey)	// when these match, the block was found.
				{
					BlockCopyQuery foundCopy(blockMap[loadedBlocksBegin->first]);	// create a BlockCopyQuery instance with the wasFound set to true.
					returnQuery = foundCopy;
					exposedDiscovered = true;
				}
			}
		}
		else
		{
			// we must generate block copies to produce this list; we will assume that the data needed to do this is already in the skeletonSGM.
			auto fetchedExposedBlockMap = produceBlockCopies();
			auto blocksBegin = fetchedExposedBlockMap.begin();
			auto blocksEnd = fetchedExposedBlockMap.end();
			for (; blocksBegin != blocksEnd; blocksBegin++)
			{
				EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(blocksBegin->first);
				if (currentKey == in_blockKey)	// when these match, the block was found.
				{
					BlockCopyQuery foundCopy(fetchedExposedBlockMap[blocksBegin->first]);	// create a BlockCopyQuery instance with the wasFound set to true.
					returnQuery = foundCopy;
					exposedDiscovered = true;
				}
			}
		}

		// if we didn't find it in exposed, check the UNEXPOSED blocks.
		// If it was found, and because its unexposed, we'll just return en EnclaveBlock instance that has nothing in it
		// (by their nature, UNEXPOSED EnclaveBlocks should have no triangles to render)
		if (exposedDiscovered == false)
		{
			auto unexposedBlocksBegin = blockSkeletonMap.begin();
			auto unexposedBlocksEnd = blockSkeletonMap.end();
			for (; unexposedBlocksBegin != unexposedBlocksEnd; unexposedBlocksBegin++)
			{
				EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(unexposedBlocksBegin->first);
				if (currentKey == in_blockKey)	// when these match, the block was found.
				{
					EnclaveBlock emptyBlock;
					BlockCopyQuery foundCopy(emptyBlock);
					returnQuery = foundCopy;
				}
			}
		}

	}

	// otherwise, scan the existing EXPOSED and UNEXPOSED maps if we are in a ORELodState of LOD_BLOCK;
	// if we don't find anything in either of those, then the block doesn't exist.
	else if
	(
		currentLodState == ORELodState::LOD_BLOCK
	)
	{
		//std::cout << "(OrganicRawEnclave::queryForBlockCopy): entered BLOCK branch. " << std::endl;
		// any block in the block map, that is "visible" (not implemented yet) would be considered EXPOSED.
		auto exposedBlocksBegin = blockMap.begin();
		auto exposedBlocksEnd = blockMap.end();
		bool exposedDiscovered = false;
		for (; exposedBlocksBegin != exposedBlocksEnd; exposedBlocksBegin++)
		{
			EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(exposedBlocksBegin->first);
			if (currentKey == in_blockKey)	// when these match, the block was found.
			{
				BlockCopyQuery foundCopy(blockMap[exposedBlocksBegin->first]);	// create a BlockCopyQuery instance with the wasFound set to true.
				exposedDiscovered = true;
				returnQuery = foundCopy;
			}
		}

		// if we didn't find it in exposed, check the unexposed.
		// If it was found, and because its unexposed, we'll just return en EnclaveBlock instance that has nothing in it
		// (by their nature, UNEXPOSED EnclaveBlocks should have no triangles to render)
		if (exposedDiscovered == false)
		{
			auto unexposedBlocksBegin = blockSkeletonMap.begin();
			auto unexposedBlocksEnd = blockSkeletonMap.end();
			for (; unexposedBlocksBegin != unexposedBlocksEnd; unexposedBlocksBegin++)
			{
				EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(unexposedBlocksBegin->first);
				if (currentKey == in_blockKey)	// when these match, the block was found.
				{
					EnclaveBlock emptyBlock;
					BlockCopyQuery foundCopy(emptyBlock);
					returnQuery = foundCopy;
				}
			}
		}
	}

	// NOTE: we don't do anything if the ORE's current state is ORELodState::FULL.
	return returnQuery;
}

void OrganicRawEnclave::simulateBlockProduction()
{
	BorderDataMap borderDataMap; // for getting trace results in enclaves
	BlockBorderLineList blockBorderLineList;


	// part 1: inflate the triangle containers, into the temporary etcSGM.
	EnclaveTriangleContainerSupergroupManager temporaryETCSgm;
	OrganicTriangleSecondarySupergroupManager tempSecondarySGM;

	std::cout << "(OrganicRawEnclave): Running simulation step 1. " << std::endl;
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
				temporaryETCSgm.insertEnclaveTriangleDirectIntoSuperGroup(skeletonSGMBegin->first, currentSkeletonContainerBegin->first, currentSkeletonBegin->first, enclaveTriangle);
			}
		}
	}

	// part 2: execute the runs for the triangles; delete/move EnclaveTriangles that are invalid, so that they don't run.
	// it should be noted, that if not erased, invalid EnclaveTriangles can still produce BBFan data that secondaries can read. It's just that all of them stop upon the first invalid detection.

	std::cout << "(OrganicRawEnclave): Running simulation step 2. " << std::endl;
	auto etcSGMBegin1 = temporaryETCSgm.enclaveTriangleSupergroups.begin();
	auto etcSGMEnd1 = temporaryETCSgm.enclaveTriangleSupergroups.end();
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
				std::cout << "(OrganicRawEnclave): current simulation enclave triangle points: " << std::endl;
					currentTrianglesBegin->second.printPoints();
				std::cout << "(OrganicRawEnclave): current simulation enclave triangle empty normal: " << currentTrianglesBegin->second.emptyNormal.x << ", "
					<< currentTrianglesBegin->second.emptyNormal.y << ", "
					<< currentTrianglesBegin->second.emptyNormal.z << std::endl;
				int waitVal = 3;
				std::cin >> waitVal;

				//if (currentTrianglesBegin->first == 0)
				//{
				currentTrianglesBegin->second.executeRun(&blockBorderLineList, &borderDataMap, EnclaveKeyDef::EnclaveKey(0,0,0), false);
				

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
					currentTriangleContainerBegin->second.triangles.erase(*removalSetBegin);
				}
			}
		}
	}

	// part 3:
	std::cout << "(OrganicRawEnclave): Running simulation step 3. " << std::endl;
	auto etcSGMBegin2 = temporaryETCSgm.enclaveTriangleSupergroups.begin();
	auto etcSGMEnd2 = temporaryETCSgm.enclaveTriangleSupergroups.end();
	for (etcSGMBegin2; etcSGMBegin2 != etcSGMEnd2; etcSGMBegin2++)
	{

		auto currentTriangleContainerBegin = etcSGMBegin2->second.containerMap.begin();
		auto currentTriangleContainerEnd = etcSGMBegin2->second.containerMap.end();
		for (; currentTriangleContainerBegin != currentTriangleContainerEnd; currentTriangleContainerBegin++)
		{
			bool doBlocksExistAtY = currentTriangleContainerBegin->second.checkForYSliceBlocks(0);
			if (doBlocksExistAtY == true)
			{
				std::cout << "(OrganicRawEnclave): notice, found an EnclaveTriangleContainer that has blocks at 0! Printing triangles..." << std::endl;
				currentTriangleContainerBegin->second.printTrianglesInContainer();
				currentTriangleContainerBegin->second.printTouchedBlocksPerTriangle();
			}

			OrganicTriangleSecondary container;
			container.loadDataFromEnclaveTriangleContainer(&currentTriangleContainerBegin->second);
			//insertOrganicTriangleSecondary(etcSGMBegin2->first, currentTriangleContainerBegin->first, container);
			insertOrganicTriangleSecondaryIntoRefedManager(&tempSecondarySGM, etcSGMBegin2->first, currentTriangleContainerBegin->first, container);
			// need to insert from a temp OrganicTriangleSecondarySupergroupManager...
		}

	}
}

EnclaveBlockState OrganicRawEnclave::getBlockStatus(EnclaveKeyDef::EnclaveKey in_blockKey)
{
	EnclaveBlockState returnBlockState = EnclaveBlockState::NONEXISTENT;
	// we shouldn't need a mutex here, as we'd only be reading, and not modifying data.
	// if we are in a "matter" state, we need to generate the blocks 
	if
	(
		currentLodState == ORELodState::LOD_ENCLAVE_SMATTER
		||
		currentLodState == ORELodState::LOD_ENCLAVE_RMATTER
	)
	{
		//std::cout << "(OrganicRawEnclave): getBlockStatus -> entered RMATTER/SMATTER branch. " << std::endl;

		// if there are blocks already populated, check those; we won't need -- and don't want to --
		// call produceBlockCopies() as that is an expensive operation.
		int targetInt = PolyUtils::convertBlockCoordsToSingle(in_blockKey);
		if (blockMap.size() != 0)
		{
			//std::cout << "(OrganicRawEnclave): blocks were already loaded in this ORE (LOD_ENCLAVE_SMATTER or LOD_ENCLAVE_RMATTER)." << std::endl;
			auto existingBlockFinder = blockMap.find(targetInt);
			if (existingBlockFinder != blockMap.end())
			{
				returnBlockState = EnclaveBlockState::EXPOSED;
			}
		}

		// otherwise, we'll need to call produceBlockCopies, as it means we're looking at an ORE in a state of 
		// LOD_ENCLAVE_SMATTER / LOD_ENCLAVE_RMATTER, that hasn't called spawnRenderableBlocks() on it yet.
		else
		{

			auto exposedBlocks = produceBlockCopies();

			// check if it's an exposed block; if it is, return BlockManifestState::EXPOSED
			auto exposedBlocksFinder = exposedBlocks.find(targetInt);
			if (exposedBlocksFinder != exposedBlocks.end())
			{
				returnBlockState = EnclaveBlockState::EXPOSED;
				//std::cout << "(OrganicRawEnclave): found EXPOSED block for key: ";
				//in_blockKey.printKey();
				//std::cout << std::endl;
			}
		}
		// check if it's an unexposed block; if it is, return BlockManifestState::UNEXPOSED
		auto unexposedBlocks = fetchUnexposedBlockKeys();
		auto unexposedBlockFinder = unexposedBlocks.find(in_blockKey);
		if (unexposedBlockFinder != unexposedBlocks.end())
		{
			returnBlockState = EnclaveBlockState::UNEXPOSED;
			//std::cout << "(OrganicRawEnclave): found UNEXPOSED block for key: ";
			//in_blockKey.printKey();
			//std::cout << std::endl;
		}
	}

	// otherwise, if we are in ORELodState::BLOCK, just check whatever exists
	else if
	(
		currentLodState == ORELodState::LOD_BLOCK
	)
	{
		// ***** The below commented out code needs to be analyzed in next commit, to understand why it didn't work;
		// Main culprit of this is that skeletons/etcsgm gets wiped out in the call to morphLodToBlock (5/22/2022)
		/*
		// check if it's an exposed block; if it is, return BlockManifestState::EXPOSED
		auto exposedBlocksList = fetchExposedBlockKeys();
		auto exposedFinder = exposedBlocksList.find(in_blockKey);
		if (exposedFinder != exposedBlocksList.end())
		{
			returnBlockState = EnclaveBlockState::EXPOSED;
		}
		*/

		int targetInt = PolyUtils::convertBlockCoordsToSingle(in_blockKey);
		auto targetFinder = blockMap.find(targetInt);
		if (targetFinder != blockMap.end())
		{
			returnBlockState = EnclaveBlockState::EXPOSED;
		}

		// check if it's an unexposed block; if it is, return BlockManifestState::UNEXPOSED
		auto unexposedBlockList = fetchUnexposedBlockKeys();
		auto unexposedFinder = unexposedBlockList.find(in_blockKey);
		if (unexposedFinder != unexposedBlockList.end())
		{
			returnBlockState = EnclaveBlockState::UNEXPOSED;
		}
	}

	// otherwise, it's full, so it can either exist as UNEXPOSED or NONEXISTENT_BECAUSE_FULL
	else if
	(
		currentLodState == ORELodState::FULL
	)
	{
		//std::cout << "(OrganicRawEnclave): getBlockStatus -> entered FULL branch. " << std::endl;

		auto fullUnexposedBlockList = fetchUnexposedBlockKeys();
		auto fullUnexposedFinder = fullUnexposedBlockList.find(in_blockKey);
		if (fullUnexposedFinder != fullUnexposedBlockList.end())
		{
			returnBlockState = EnclaveBlockState::UNEXPOSED_BUT_FULL;
		}
		else
		{
			returnBlockState = EnclaveBlockState::NONEXISTENT_BECAUSE_FULL;
		}
	}
	return returnBlockState;
}

std::map<int, EnclaveBlock> OrganicRawEnclave::produceBlockCopies()
{
	OrganicTriangleSecondarySupergroupManager tempOtsSGM;
	EnclaveTriangleContainerSupergroupManager tempEtcSGM;
	BorderDataMap borderDataMap; // for getting trace results in enclaves
	BlockBorderLineList blockBorderLineList;

	// part 1: inflate the triangle containers, into the tempEtcSGM.
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
				tempEtcSGM.insertEnclaveTriangleDirectIntoSuperGroup(skeletonSGMBegin->first, currentSkeletonContainerBegin->first, currentSkeletonBegin->first, enclaveTriangle);
			}
		}
	}

	// part 2: execute the runs for the triangles; delete/move EnclaveTriangles that are invalid, so that they don't run.
	// it should be noted, that if not erased, invalid EnclaveTriangles can still produce BBFan data that secondaries can read. It's just that all of them stop upon the first invalid detection.
	auto etcSGMBegin1 = tempEtcSGM.enclaveTriangleSupergroups.begin();
	auto etcSGMEnd1 = tempEtcSGM.enclaveTriangleSupergroups.end();
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
				currentTrianglesBegin->second.executeRun(&blockBorderLineList, &borderDataMap, EnclaveKeyDef::EnclaveKey(0, 0, 0), false);

				// is this triangle INVALID?
				if (currentTrianglesBegin->second.isTriangleValid == false)
				{
					//std::cout << "(OrganicRawEnclave): WARNING: EnclaveTriangle detected as invalid, in call to produceBlockCopies. " << std::endl;
					removalSet.insert(currentTrianglesBegin->first);	// insert the int into the removal set
					//int removalVal = 3;
					//std::cin >> removalVal;
				}
			}

			// Need to figure out why this below area is needed; possibly deprecated? (3/22/2022)
			if (!removalSet.empty())
			{
				//std::cout << "Removal set not empty; removing identified invalids...";
				auto removalSetBegin = removalSet.begin();
				auto removalSetEnd = removalSet.end();
				for (; removalSetBegin != removalSetEnd; removalSetBegin++)
				{
					//etcSGMBegin1->second.triangles.erase(*removalSetBegin);		// remove the INVALID triangle from the corresponding container.
					currentTriangleContainerBegin->second.triangles.erase(*removalSetBegin);
				}
			}
		}
	}

	// part 3: load simulated results into into the EnclaveTriangleContainerSupergroupManager.
	auto etcSGMBegin2 = tempEtcSGM.enclaveTriangleSupergroups.begin();
	auto etcSGMEnd2 = tempEtcSGM.enclaveTriangleSupergroups.end();
	for (etcSGMBegin2; etcSGMBegin2 != etcSGMEnd2; etcSGMBegin2++)
	{
		auto currentTriangleContainerBegin = etcSGMBegin2->second.containerMap.begin();
		auto currentTriangleContainerEnd = etcSGMBegin2->second.containerMap.end();
		for (; currentTriangleContainerBegin != currentTriangleContainerEnd; currentTriangleContainerBegin++)
		{
			OrganicTriangleSecondary container;
			container.loadDataFromEnclaveTriangleContainer(&currentTriangleContainerBegin->second);
			insertOrganicTriangleSecondaryIntoRefedManager(&tempOtsSGM, etcSGMBegin2->first, currentTriangleContainerBegin->first, container);
		}

	}

	// part 4: generate the block triangles, and put them into the tempBlockMap before the function returns; these are the "exposed" blocks.
	std::map<int, EnclaveBlock> exposedBlockMap;
	tempOtsSGM.generateBlockTrianglesFromSecondaries(&blockSkeletonMap, &exposedBlockMap, &total_triangles);
	return exposedBlockMap;
}

OperableIntSet OrganicRawEnclave::getExistingEnclaveTriangleSkeletonContainerTracker()
{
	return existingEnclaveTriangleSkeletonContainerTracker;
}