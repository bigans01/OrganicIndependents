#include "stdafx.h"
#include "OrganicRawEnclave.h"

MessageContainer OrganicRawEnclave::convertOREToBDMFormat(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey)
{
	// Remember: the returning MessageContainer must be MessageContainerType::MC_BDM, so that
	// if we are sending it to an OrganicSystem instance, the MessageCable knows what to do with it.
	// See the ServerMessageInterpreter class in OrganicServerLib, and CoreMessageInterpreter in OrganicCoreLib,
	// for examples of this usage.
	MessageContainer oreDataContainer(MessageContainerType::MC_BDM);

	// Step XX: Build ORE header Message (BDM_ORE_HEADER); 
	// remember to append the in_oreKey and in_blueprintKey values value to the front of it, in that order.
	Message constructedOREHeader = buildOREHeaderBDMMessage(in_blueprintKey, in_oreKey);
	oreDataContainer.insertMessage(constructedOREHeader);

	// Step XX: build the oreRTHandler's reconstruction Message (BDM_ORE_SKELETONSGM); 
	// remember to append the in_oreKey and in_blueprintKey values value to the front of it, in that order.

	// OT-12
	Message constructedSGMMessage = oreRTHandler.convertHandlerToBDM(in_blueprintKey, in_oreKey);
	oreDataContainer.insertMessage(constructedSGMMessage);

	// Step XX: Create a BDM_SKELETONBLOCK_TAGGED, for each skeleton in blockSkeletonMap
	for (auto& currentSkeleton : blockSkeletonMap)
	{
		Message currentSkeletonData(MessageType::BDM_SKELETONBLOCK_TAGGED);
		
		// First, enter the the EnclaveKey coordinates (blueprint key, ore key, block key). Then, inseert the int-casted materialID.
		currentSkeletonData.insertEnclaveKey(in_blueprintKey);
		currentSkeletonData.insertEnclaveKey(in_oreKey);
		currentSkeletonData.insertEnclaveKey(PolyUtils::convertSingleToBlockKey(currentSkeleton.first));
		currentSkeletonData.insertInt(int(currentSkeleton.second.materialID));

		oreDataContainer.insertMessage(currentSkeletonData);
	}

	// Step XX: Create a BDM_BLOCK_TAGGED Message, for each block in the blockMap.
	//
	// The block key, ORE key, and blueprint key must be appended to the Message before it goes into the container, in that order.
	for (auto& currentBlock : blockMap)
	{
		// fetch the raw block data (would be BDM_BLOCK_UNTAGGED format)
		Message currentBlockData = currentBlock.second.writeEnclaveBlockToBDMMessage();

		// append block coordinates to front (remember, reverse order, so: block, ORE, blueprint)
		EnclaveKeyDef::EnclaveKey currentBlockKey = PolyUtils::convertSingleToBlockKey(currentBlock.first);
		currentBlockData.insertEnclaveKeyFront(currentBlockKey);
		currentBlockData.insertEnclaveKeyFront(in_oreKey);
		currentBlockData.insertEnclaveKeyFront(in_blueprintKey);

		// lastly, change the Message type, and then insert it into the MessageContainer.
		currentBlockData.messageType = MessageType::BDM_BLOCK_TAGGED;
		oreDataContainer.insertMessage(currentBlockData);
	}

	return oreDataContainer;
}

void OrganicRawEnclave::reconstituteAsLodBlock(Message in_bdmMetadataMessage, 
											   std::unordered_map<EnclaveKeyDef::EnclaveKey, Message, EnclaveKeyDef::KeyHasher>* in_blockMessageMapRef,
											   std::unordered_map<EnclaveKeyDef::EnclaveKey, Message, EnclaveKeyDef::KeyHasher>* in_skeletonMessageMapRef)
{
	//std::cout << "(OrganicRawEnclave::reconstituteAsLodBlock) -> reconstituting ORE as LOD_BLOCK. " << std::endl;

	reconstituteOREStatesFromMessage(in_bdmMetadataMessage);

	reconstituteBlocksFromBDMMap(in_blockMessageMapRef);
	reconstituteSkeletonsFromBDMMap(in_skeletonMessageMapRef);

	//printOREStates();

	// reset total triangles, but not the block map
	total_triangles = 0;

	// cycle through each block, and update the value of total_triangles
	for (auto& currentBlock : blockMap)
	{
		total_triangles += currentBlock.second.getNumberOfTotalTriangles();
	}
	//std::cout << "(OrganicRawEnclave::reconstituteAsLodBlock) -> reconstitution as LOD_BLOCK complete; number of total triangles is: " << total_triangles << std::endl;
}

void OrganicRawEnclave::reconstituteAsSMatter(Message in_bdmMetadataMessage,
											  std::unordered_map<EnclaveKeyDef::EnclaveKey, Message, EnclaveKeyDef::KeyHasher>* in_skeletonMessageMapRef,
											  Message in_skeletonSGMBuildingMessage)
{
	//currentLodState = ORELodState::LOD_BLOCK;					// switch to block mode
	//currentAppendedState = OREAppendedState::NONE;				// appended state isn't applicable when in block mode
	//currentDependencyState = OREDependencyState::INDEPENDENT;	// block mode is always independent

	reconstituteOREStatesFromMessage(in_bdmMetadataMessage);
	reconstituteSkeletonsFromBDMMap(in_skeletonMessageMapRef);

	// OT-13
	RenderableTriangleHandler rebuiltHandler(in_skeletonSGMBuildingMessage);
	oreRTHandler = rebuiltHandler;
}

void OrganicRawEnclave::reconstituteAsFull(Message in_bdmMetadataMessage,
										   std::unordered_map<EnclaveKeyDef::EnclaveKey, Message, EnclaveKeyDef::KeyHasher>* in_skeletonMessageMapRef)
{
	//std::cout << "(OrganicRawEnclave::reconstituteAsFull) -> reconstituting ORE as FULL. " << std::endl;
	total_triangles = 0;

	reconstituteOREStatesFromMessage(in_bdmMetadataMessage);
	reconstituteSkeletonsFromBDMMap(in_skeletonMessageMapRef);

	//std::cout << "(OrganicRawEnclave::reconstituteAsFull) -> reconstitution to FULL complete; number of skeletons is: " << blockSkeletonMap.size() << std::endl;
}

void OrganicRawEnclave::reconstituteOREStatesFromMessage(Message in_oreHeaderMessage)
{
	in_oreHeaderMessage.open();
	currentLodState = ORELodState(in_oreHeaderMessage.readInt());
	currentAppendedState = OREAppendedState(in_oreHeaderMessage.readInt());
	currentDependencyState = OREDependencyState(in_oreHeaderMessage.readInt());
}

void OrganicRawEnclave::clearOutDataContainers()
{
	// OT-6
	oreRTHandler.clear();
}

void OrganicRawEnclave::reconstituteBlocksFromBDMMap(std::unordered_map<EnclaveKeyDef::EnclaveKey, Message, EnclaveKeyDef::KeyHasher>* in_blockMessageMapRef)
{
	int exposedBlockCounter = 0;
	std::mutex dummyTestMutex;
	for (auto currentBlockMessageIter : *in_blockMessageMapRef)
	{
		EnclaveKeyDef::EnclaveKey currentBlockKey = currentBlockMessageIter.first;

		//std::cout << "(OrganicRawEnclave::reconstituteBlocksFroBDMMap) -> constructed data for block at key: ";
		//currentBlockKey.printKey();
		//std::cout << std::endl;

		Message copiedMessage = currentBlockMessageIter.second;
		copiedMessage.open();

		EnclaveBlock rebuiltBlock(&copiedMessage);

		insertBlock(&dummyTestMutex, PolyUtils::convertBlockCoordsToSingle(currentBlockKey), rebuiltBlock);
		exposedBlockCounter++;
	}
	//std::cout << "OrganicRawEnclave::reconstituteBlocksFroBDMMap) Total exposed blocks reconstituted: " << exposedBlockCounter << std::endl;
}

void OrganicRawEnclave::reconstituteSkeletonsFromBDMMap(std::unordered_map<EnclaveKeyDef::EnclaveKey, Message, EnclaveKeyDef::KeyHasher>* in_skeletonMessageMapRef)
{
	int skeletonCounter = 0;
	for (auto currentSkeletonIter : *in_skeletonMessageMapRef)
	{
		EnclaveKeyDef::EnclaveKey currentBlockKey = currentSkeletonIter.first;

		Message copiedMessage = currentSkeletonIter.second;
		copiedMessage.open();

		int materialValue = copiedMessage.readInt();
		EnclaveBlockSkeleton newSkeleton;
		newSkeleton.materialID = TriangleMaterial(materialValue);

		insertBlockSkeleton(currentBlockKey, newSkeleton);

		skeletonCounter++;
	}
	//std::cout << "(OrganicRawEnclave::reconstituteSkeletonsFroBDMMap) Total skeletons reconstituted: " << skeletonCounter << std::endl;
}

void OrganicRawEnclave::printOREStates()
{
	std::string lodStatePrefix = "currentLodState: ";
	switch (currentLodState)
	{
		case ORELodState::FULL: { lodStatePrefix += "FULL"; break; }
		case ORELodState::LOD_BLOCK: { lodStatePrefix += "LOD_BLOCK"; break; }
		case ORELodState::LOD_ENCLAVE_SMATTER: { lodStatePrefix += "LOD_ENCLAVE_SMATTER"; break; }
		case ORELodState::LOD_ENCLAVE_RMATTER: { lodStatePrefix += "LOD_ENCLAVE_RMATTER"; break; }
	}
	std::cout << "Value of currentLodState: " << lodStatePrefix << std::endl;

	std::string dependencyStatePrefix = "currentDependencyState: ";
	switch (currentDependencyState)
	{
		case OREDependencyState::NOVAL: { dependencyStatePrefix += "NOVAL"; break; }
		case OREDependencyState::DEPENDENT_ON_PARENTS: { dependencyStatePrefix += "DEPENDENT_ON_PARENTS"; break; }
		case OREDependencyState::INDEPENDENT: { dependencyStatePrefix += "INDEPENDENT"; break; }
	}
	std::cout << "Value of currentDependencyState: " << dependencyStatePrefix << std::endl;

	std::string appendedStatePrefix = "currentAppendedState: ";
	switch (currentAppendedState)
	{
		case  OREAppendedState::NONE: { appendedStatePrefix += "NONE"; break; }
		case  OREAppendedState::SINGLE_APPEND: { appendedStatePrefix += "SINGLE_APPEND"; break; }
		case  OREAppendedState::MULTIPLE_APPEND: { appendedStatePrefix += "MULTIPLE_APPEND"; break; }
	}
	std::cout << "Value of appendedState: " << appendedStatePrefix << std::endl;
}

Message OrganicRawEnclave::buildOREHeaderBDMMessage(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey)
{
	Message oreHeaderMessage(MessageType::BDM_ORE_HEADER);

	// Insert the blueprint key, followed by the ORE key, first.
	oreHeaderMessage.insertEnclaveKey(in_blueprintKey);
	oreHeaderMessage.insertEnclaveKey(in_oreKey);

	// First part of Message: store the currentLodState, currentAppendedState, and currentDependencyState, in that order.
	oreHeaderMessage.insertInt(int(currentLodState));	// when reading a Message of the type BDM_ORE_HEADER, the value used for currentLodState
														// will need to be used to determine how the ORE should be populated, assuming that it has all the data it needs.

	oreHeaderMessage.insertInt(int(currentAppendedState));
	oreHeaderMessage.insertInt(int(currentDependencyState));

	// Next, get the total number of real blocks (from the blockMap) and skeleton blocks (from blockSkeletonMap), in that order.
	oreHeaderMessage.insertInt(int(blockMap.size()));
	oreHeaderMessage.insertInt(int(blockSkeletonMap.size()));

	return oreHeaderMessage;
}

OrganicRawEnclave::OrganicRawEnclave()
{

}

OrganicRawEnclave::OrganicRawEnclave(ORELodState in_ORELodState)
{
	currentLodState = in_ORELodState;

	// In most, if not all cases, this constructor function will pass 
	// in a value of ORELodState::FULL (see the usage of this constructor in
	// the function, OrganicMassDriverElevator::applyMass in OrganicCoreLib)
	//
	// The idea being that when the state is FULL, all blocks need to be skeletons.
	if (currentLodState == ORELodState::FULL)
	{
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
	}
}


void OrganicRawEnclave::insertOrganicTriangleSecondaryIntoRefedManager(OrganicTriangleSecondarySupergroupManager* in_refedManager,
	int in_polyID,
	int in_clusterID,
	OrganicTriangleSecondary in_enclavePolyFractureResults)
{
	in_refedManager->insertSecondary(in_polyID, in_clusterID, in_enclavePolyFractureResults);
}

void OrganicRawEnclave::insertTiledTriangleIntoRTHandler(int in_polyID, 
													   int in_clusterID, 
													   EnclaveTriangleContainer in_enclaveTriangleContainer,
													   OrganicTriangleSecondary in_secondaryComponent)
{
	// OT-5 (remove everything but the call to oreRTHandler.insertTiledTriangles, when ready.
	oreRTHandler.insertTiledTriangles(in_polyID, in_clusterID, in_enclaveTriangleContainer);
}

void OrganicRawEnclave::insertBlockSkeleton(EnclaveKeyDef::EnclaveKey in_blockKey, EnclaveBlockSkeleton in_blockSkeleton)
{
	int blockCoordsToSingle = PolyUtils::convertBlockCoordsToSingle(in_blockKey.x, in_blockKey.y, in_blockKey.z);
	blockSkeletonMap[blockCoordsToSingle] = in_blockSkeleton;
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
}

void OrganicRawEnclave::setOREasIndependent()
{
	currentDependencyState = OREDependencyState::INDEPENDENT;
}

std::set<int> OrganicRawEnclave::getTouchedBlockList()
{
	std::set<int> touchedSet;	// the set to return

	// OT-15
	touchedSet = oreRTHandler.generateTouchedBlockList();
	return touchedSet;
}

std::set<int> OrganicRawEnclave::getUnexposedBlockList()
{
	std::set<int> unexposedSet;
	for (auto& currentSkeletonBlock : blockSkeletonMap)
	{
		unexposedSet.insert(currentSkeletonBlock.first);
	}
	return unexposedSet;
}

std::map<int, EnclaveBlockSkeleton> OrganicRawEnclave::getUnexposedBlocksCopy()
{
	return blockSkeletonMap;
}

bool OrganicRawEnclave::checkIfFull()
{
	bool returnValue = false;
	//std::cout << "blockSkeletonMap size is: " << blockSkeletonMap.size();
	if (blockSkeletonMap.size() == 64)	// a full ORE has exactly 64 skeletons
	{
		returnValue = true;
		// if the ORE is full, we should empty/reset the following:
		// --skeletonSGM, 
		// --etcSGM
		// --organicTriangleSecondarySGM's managed contents;
		// --the block map, as it shouldn't be populated if the ORE is full.
		// --the total_triangles need to be reset back to 0.
		//
		// ...however, we want the enclave block skeletons to remain.
		blockMap.clear();
		clearOutDataContainers();
		total_triangles = 0;
		currentLodState = ORELodState::FULL;
	}
	return returnValue;
}

bool OrganicRawEnclave::doesOREContainRenderableData()
{
	bool containsData = false;
	switch(currentLodState)
	{
		// For both cases below: OT-7.
		case ORELodState::LOD_ENCLAVE_SMATTER:
		{
			containsData = oreRTHandler.containsRenderableTriangles();
			break;
		};

		case ORELodState::LOD_ENCLAVE_RMATTER:
		{
			containsData = oreRTHandler.containsRenderableTriangles();
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
	//currentLodState = ORELodState::LOD_ENCLAVE_RMATTER;	// switch to RMatter mode
	//morphLodToBlock();




	currentDependencyState = OREDependencyState::INDEPENDENT;	// all RMatter is INDEPENDENT
	blockSkeletonMap.clear();
	blockSkeletonMap = rMassSolidsMap;	// assumes that setBlockSkeletons has been called before this function.
}

void OrganicRawEnclave::setPendingRMatterSolids(std::mutex* in_mutexRef, 
												Operable3DEnclaveKeySet in_skeletonBlockSet,
												Operable3DEnclaveKeySet in_filledClaimedNonSolids)
{
	// this function will eventually need to be edited to set the metadata for the EnclaveBlockSkeletons that will go into
	// rMassSolidMap. (Noted on 2/15/2022)
	std::lock_guard<std::mutex> lock(*in_mutexRef);

	rMassSolidsMap.clear();	

	// first, generate exposed block copies. Compare that set against the in_filledClaimedNonSolids set.
	// If a key in the claimed filled non solid is NOT found in the exposed block copies, it is a false removal, 
	// because it means that it should of been exposed, but was not (i.e, it produced no triangles). As a result, it needs to be solid.
	// To do that, simply throw it into the rMassSolidsMap.
	//
	// This check has to be done, because it was discovered around 12/3/2022 that there are 
	// rare cases where a LANDLOCKED RMatter block is ignored in the call to RMorphableMeshGroup::scanForSolidBlocks,
	// when it has at least one TRACE_BIT is detected in it's area, but then the block that is claimed to be no longer LANDLOCKED
	// never becomes an EXPOSED block -- thus resulting in a "hole."
	//
	// Thus, if the claimed non-LANDLOCKED block contains an INNER_MASS, but did not end up becoming EXPOSED,
	// it needs to go back to being LANDLOCKED.

	auto exposedBlocks = produceBlockCopies();
	for (auto& currentClaimedNonFilled : in_filledClaimedNonSolids.getSet())
	{
		int blockCoordsToSingle = PolyUtils::convertBlockCoordsToSingle((currentClaimedNonFilled).x, (currentClaimedNonFilled).y, (currentClaimedNonFilled).z);
		auto matchFinder = exposedBlocks.find(blockCoordsToSingle);

		// if there was no match, it means that realistically, the block is still solid.	
		if (matchFinder == exposedBlocks.end())
		{
			EnclaveBlockSkeleton newSkeleton;
			rMassSolidsMap[blockCoordsToSingle] = newSkeleton;
		}
		
	}
	

	/*
	auto skeletonsBegin = in_skeletonBlockSet.begin();
	auto skeletonsEnd = in_skeletonBlockSet.end();
	for (; skeletonsBegin != skeletonsEnd; skeletonsBegin++)
	{
		EnclaveBlockSkeleton newSkeleton;
		int blockCoordsToSingle = PolyUtils::convertBlockCoordsToSingle((*skeletonsBegin).x, (*skeletonsBegin).y, (*skeletonsBegin).z);
		rMassSolidsMap[blockCoordsToSingle] = newSkeleton;
	}
	*/

	// I'm not sure how this for loop below automatically set up it's looping from the Operable3DEnclaveKeySet,
	// but hey -- it works and is probably OK. (I left the old code comemnted out in a block above)
	for (auto& currentSkeletonCoord : in_skeletonBlockSet)
	{
		EnclaveBlockSkeleton newSkeleton;
		int blockCoordsToSingle = PolyUtils::convertBlockCoordsToSingle((currentSkeletonCoord).x, (currentSkeletonCoord).y, (currentSkeletonCoord).z);
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
			spawnContainersAndCreateBlocks(in_mutexRef, in_enclaveKey);
			break;
		};

		case ORELodState::LOD_ENCLAVE_RMATTER:
		{
			// will spawn all renderable blocks via stored EnclaveTriangles.
			spawnContainersAndCreateBlocks(in_mutexRef, in_enclaveKey);
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
	clearOutDataContainers();	
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
}

void OrganicRawEnclave::insertBlock(std::mutex* in_mutexRef, int in_blockIndex, EnclaveBlock in_blockToInsert)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef); // lock while performing this operation, obviously.

	// we will need to decrement the total triangles by the amount of the old block, if it exists.
	auto doesBlockAlreadyExist = blockMap.find(in_blockIndex);
	if (doesBlockAlreadyExist != blockMap.end())
	{
		blockSkeletonMap.erase(in_blockIndex);
		int erasedTriangles = blockMap[in_blockIndex].getNumberOfTotalTriangles();
		total_triangles -= erasedTriangles;
	}

	// now, insert the block into the map, but only after we've gotten the total number of triangles from it.
	total_triangles += in_blockToInsert.getNumberOfTotalTriangles();
	blockMap[in_blockIndex] = in_blockToInsert;
}

Operable3DEnclaveKeySet OrganicRawEnclave::spawnRenderableBlocks(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	// The return value of this function will be a set of EnclaveKey instances
	// that represent blocks that couldn't be produced.
	Operable3DEnclaveKeySet uncalculatedBlocks;
	switch (currentLodState)
	{
		case ORELodState::LOD_ENCLAVE_SMATTER:
		{
			// will spawn all renderable blocks via stored EnclaveTriangles.
			uncalculatedBlocks += spawnContainersAndCreateBlocks(in_mutexRef, in_enclaveKey);
			break;
		};

		case ORELodState::LOD_ENCLAVE_RMATTER:
		{
			// will spawn all renderable blocks via stored EnclaveTriangles.
			uncalculatedBlocks += spawnContainersAndCreateBlocks(in_mutexRef, in_enclaveKey);
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
	return uncalculatedBlocks;
}

int OrganicRawEnclave::getNumberOfTrianglesByLOD()
{
	int triangleCount = 0;

	// For both cases: OT-8 applies 
	switch (currentLodState)
	{
		case ORELodState::LOD_ENCLAVE_SMATTER:
		{
			triangleCount = oreRTHandler.getTriangleCountFromContainers();
			break;
		};

		case ORELodState::LOD_ENCLAVE_RMATTER:
		{
			// if LOD_ENCLAVE, simply look at the skeletonSGM
			triangleCount = oreRTHandler.getTriangleCountFromContainers();
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

int OrganicRawEnclave::getTotalUnexposedBlocks()
{
	return int(blockSkeletonMap.size());
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
	appendSpawnedEnclaveTriangleSkeletonContainers(in_mutexRef, 
												   &in_otherEnclave->oreRTHandler);

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

std::vector<EnclaveTriangle> OrganicRawEnclave::retrieveHandlerTiledTriangles(int in_superGroupID)
{
	//OT-4 -- tested, verified.
	return oreRTHandler.retrieveTiledTrianglesForSupergroup(in_superGroupID);
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

	//std::cout << "Skeleton container map size: " << skeletonSGM.triangleSkeletonSupergroups.size() << std::endl;
	std::cout << "Number of blocks: " << blockMap.size() << std::endl;
	std::cout << "Number of skeletons: " << blockSkeletonMap.size() << std::endl;
	//ECBPolyPoint dumbPoint;
	//dumbPoint.isAllZero();
}

void OrganicRawEnclave::printContainerStats()
{
	//std::cout << "Number of entries in skeletonSGM: " << skeletonSGM.getTotalEntries() << std::endl;
	//std::cout << "Number of entires in etcSGM: " << etcSGM.enclaveTriangleSupergroups.size() << std::endl;
	//std::cout << "Number of entries in organicTriangleSecondarySGM: " << organicTriangleSecondarySGM.secondarySupergroups.size() << std::endl;
	std::cout << "Total blocks: " << blockMap.size() << std::endl;
	std::cout << "Total triangles: " << total_triangles << std::endl;
	std::cout << "Total skeletons: " << blockSkeletonMap.size() << std::endl;
	std::cout << "Total oreRTHandler entries: " << oreRTHandler.getTriangleCountFromContainers() << std::endl;
}

void OrganicRawEnclave::printRTHandlerContainers(bool in_pauseBetweenTrianglesFlag)
{
	// OT-14
	oreRTHandler.printData();
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

void OrganicRawEnclave::appendSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, 
																		RenderableTriangleHandler* in_handlerRef)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);

	// OT-9: when ready, be sure to append the results of the below function to the appropriate Operable3DEnclaveKeySet.
	existingEnclaveTriangleSkeletonContainerTracker += oreRTHandler.appendSkeletonContainers(in_handlerRef);

	updateCurrentAppendedState();
}

void OrganicRawEnclave::reloadSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, 
																		RenderableTriangleHandler* in_handlerRef)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);

	oreRTHandler.clear();

	existingEnclaveTriangleSkeletonContainerTracker.intSet.clear();		// because we will be reloading the tracker entirely, wipe it clean. Used by OREMatterCollider::extractResultsAndSendToORE in OrganicCoreLib, when an RMatter mass has been produced.

	// OT-10: when ready, be sure to append the results of the below function to the appropriate Operable3DEnclaveKeySet.
	existingEnclaveTriangleSkeletonContainerTracker += oreRTHandler.appendSkeletonContainers(in_handlerRef);
}

void OrganicRawEnclave::removeSkeletonSupergroup(std::mutex* in_mutexRef, int in_supergroupIDToRemove)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);

	// OT-11: erase corresponding supergroup as needed 
	oreRTHandler.eraseSupergroup(in_supergroupIDToRemove);
}

Operable3DEnclaveKeySet OrganicRawEnclave::spawnContainersAndCreateBlocks(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);

	// PART 1: Before creating the containers, remove the block data.
	resetBlockDataAndTriangleCount();


	Operable3DEnclaveKeySet incalculableBlocks;	// stores blocks that a call to EnclaveTriangle::executeRun() failed to produce.

	//std::cout << ":::::::::: Calling spawn enclave triangle containers... " << std::endl;
	// OT-3: generation of blocks (tested, verified)
	incalculableBlocks += oreRTHandler.produceBlocksAndInvalids(&blockSkeletonMap, &blockMap, &total_triangles);

	// the return value of this function will be any malformed/bad blocks.
	return incalculableBlocks;
}

Message OrganicRawEnclave::fetchBDMMessageForRTHandler(EnclaveKeyDef::EnclaveKey in_blueprintKeyForBDM, EnclaveKeyDef::EnclaveKey in_oreKeyForBDM)
{
	//return skeletonSGM.convertSkeletonSGMToBDM(in_blueprintKeyForBDM, in_oreKeyForBDM);
	return oreRTHandler.convertHandlerToBDM(in_blueprintKeyForBDM, in_oreKeyForBDM);
}

void OrganicRawEnclave::printBlockCategorizations()
{
	// DEBUG: print container data for oreRTHandler.
	printContainerStats();
	std::cout << "Total oreRTHandler triangle count: " << oreRTHandler.getTriangleCountFromContainers() << std::endl;

	// print surrounded/cube (UNEXPOSED) blocks (skeletons)
	std::cout << "Printing UNEXPOSED blocks (skeletons), total count is:  " << blockSkeletonMap.size() << std::endl;
	auto blockSkeletonsBegin = blockSkeletonMap.begin();
	auto blockSkeletonsEnd = blockSkeletonMap.end();
	for (; blockSkeletonsBegin != blockSkeletonsEnd; blockSkeletonsBegin++)
	{
		EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(blockSkeletonsBegin->first);
		currentKey.printKey();
		std::cout << std::endl;
	}

	std::cout << "Printing EXPOSED blocks (non-skeletons), total count is: : " << blockMap.size() << std::endl;

	// If the ORE is in a LOD_BLOcK state, we will need to read from the exposed blocks,
	// as EnclaveTriangle instances or their related inflate/deflate/skeleton components
	// shouldn't be populated. 
	if (currentLodState == ORELodState::LOD_BLOCK)
	{
		for (auto& currentExposedBlock : blockMap)
		{
			EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(currentExposedBlock.first);
			currentKey.printKey();
			std::cout << std::endl;
		}
	}

	// Otherwise, if we are not in an LOD_BLOCK state, we'll need to produce the copies from the skeletons.
	else
	{
		auto fetchedExposedBlockMap = produceBlockCopies();

		// iterate through the exposed blocks we just created and print the block's EnclaveKey.
		for (auto& currentBlock : fetchedExposedBlockMap)
		{
			EnclaveKeyDef::EnclaveKey currentKey = PolyUtils::convertSingleToBlockKey(currentBlock.first);
			currentKey.printKey();
			std::cout << std::endl;
		}
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

std::string OrganicRawEnclave::getOREHash()
{
	std::string baseOREHash = "0";

	for (auto& currentBlockToHash : blockMap)
	{
		std::string hashToAdd = currentBlockToHash.second.getBlockHash();
		baseOREHash = HashUtils::sha256(hashToAdd + baseOREHash);
	}

	return baseOREHash;
}

void OrganicRawEnclave::simulateBlockProduction()
{

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

		// An important note: the reason for using the OR statement when checking the getBlockMode()
		// below, is due to the fact that both BlockSubType::BLOCK_NORMAL and BlockSubType::BLOCK_NORMAL_FILLED
		// are an indicator that the block is "EXPOSED"; in the future, sometime after 4/20/2023, this logic
		// might be changed, as blocks having a mode of BlockSubType::BLOCK_INDEPENDENT need to have a 
		// different return code -- or one that is already used -- to indicate that they shouldn't be used, since
		// BLOCK_INDEPENDENT indicates that it can't reliably be used in something like an sPolyShellProducer.
		// An example of this need would be in the function RJPhasedDeleteBlock::runPhasedPrechecks() in OrganicCoreLib, 
		// where it calls OrganicJobProxy::callGetBlockStateFromEFRM to check the block state. We don't want to 
		// return "EXPOSED" or "UNEXPOSED" as this would be misleading. We may need a new value for the 
		// RJPhasedDeleteBlock::runPhasedPrechecks() function to interpret.

		// if there are blocks already populated, check those; we won't need -- and don't want to --
		// call produceBlockCopies() as that is an expensive operation.
		int targetInt = PolyUtils::convertBlockCoordsToSingle(in_blockKey);
		if (blockMap.size() != 0)
		{
			//std::cout << "(OrganicRawEnclave): blocks were already loaded in this ORE (LOD_ENCLAVE_SMATTER or LOD_ENCLAVE_RMATTER)." << std::endl;
			auto existingBlockFinder = blockMap.find(targetInt);
			if (existingBlockFinder != blockMap.end())
			{
				//Under normal operations (BlockSubType::BLOCK_NORMAL or BlockSubType::BLOCK_NORMAL_FILLED), it's simply exposed...
				// But if it's BlockSubType::BLOCK_INDEPENDENT, we will need logic to tell this block to be "ignored"
				if 
				(
					(existingBlockFinder->second.getBlockMode() == BlockSubType::BLOCK_NORMAL)
					||
					(existingBlockFinder->second.getBlockMode() == BlockSubType::BLOCK_NORMAL_FILLED)
				)
				{
					returnBlockState = EnclaveBlockState::EXPOSED;
				}
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
				// Under normal operations (BlockSubType::BLOCK_NORMAL or BlockSubType::BLOCK_NORMAL_FILLED), it's simply exposed...
				// But if it's BlockSubType::BLOCK_INDEPENDENT, we will need logic to tell this block to be "ignored"
				if 
				(
					(exposedBlocksFinder->second.getBlockMode() == BlockSubType::BLOCK_NORMAL)
					||
					(exposedBlocksFinder->second.getBlockMode() == BlockSubType::BLOCK_NORMAL_FILLED)
				)
				{
					returnBlockState = EnclaveBlockState::EXPOSED;
				}
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
			// Under normal operations, it's simply exposed...
			if 
			(
				(targetFinder->second.getBlockMode() == BlockSubType::BLOCK_NORMAL)
				||
				(targetFinder->second.getBlockMode() == BlockSubType::BLOCK_NORMAL_FILLED)
			)
			{
				returnBlockState = EnclaveBlockState::EXPOSED;
			}
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
	// OT-1: tested, verified
	std::map<int, EnclaveBlock> exposedBlockMap;
	exposedBlockMap = oreRTHandler.produceBlockCopies();


	return exposedBlockMap;
}

std::vector<ORETerrainTriangle> OrganicRawEnclave::produceAndReturnTerrainTriangles()
{
	// OT-2: tested, verified. the oreRTHandler needs to have functionality
	// that copies the call to skeletonSGM below.
	return oreRTHandler.produceTerrainTriangles();
}

OperableIntSet OrganicRawEnclave::getExistingEnclaveTriangleSkeletonContainerTracker()
{
	return existingEnclaveTriangleSkeletonContainerTracker;
}

void OrganicRawEnclave::testORTHblocks()
{
	std::cout << "Testing block copying functionality..." << std::endl;

	std::cout << "Printing container stats..." << std::endl;
	printMapData();
	std::cout << "++++++++" << std::endl;
	printContainerStats();

	std::map<int, EnclaveBlock> oldBlocksMethodMap = produceBlockCopies();
	//organicTriangleSecondarySGM.simulateExposedBlockGeneration(&oldBlocksMethodMap);

	std::cout << "Size of old blocks method (from produceBlockCopies): " << oldBlocksMethodMap.size() << std::endl;

	std::cout << ">>>> getting stats of oreRTHandler..." << std::endl;
	int tCount = oreRTHandler.getTriangleCountFromContainers();
	std::cout << "count of triangles from oreRTHandler: " << tCount << std::endl;

	std::map<int, EnclaveBlock> newBlocksMethodMap = oreRTHandler.produceBlockCopies();

	std::cout << "Size of new blocks method: " << newBlocksMethodMap.size() << std::endl;

	std::cout << "+++++++++++++++++ Testing init from message. " << std::endl;
	int originalContainerCount = oreRTHandler.getTriangleCountFromContainers();
	std::cout << "Original container count: " << originalContainerCount << std::endl;
	oreRTHandler.printData();

	Message testMessage = oreRTHandler.convertHandlerToBDM(EnclaveKeyDef::EnclaveKey(0, 0, 0), EnclaveKeyDef::EnclaveKey(0, 0, 0));

	// For the sake of this test only, 
	// remove the equivalent of 2 EnclaveKeys from the front of the Message (because we insert the blueprint and OREkeys, in that order.)
	// Normally, these would be read to determine which ORE to populate in a specific blueprint, but we don't care about that
	// here.
	testMessage.removeIntsFromFrontAndResetIter(6);

	RenderableTriangleHandler testHandler(testMessage);

	int newContainerCount = testHandler.getTriangleCountFromContainers();
	std::cout << "New container count: " << newContainerCount << std::endl;
	testHandler.printData();

}