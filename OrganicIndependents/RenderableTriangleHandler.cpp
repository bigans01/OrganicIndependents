#include "stdafx.h"
#include "RenderableTriangleHandler.h"

Message RenderableTriangleHandler::convertHandlerToBDM(EnclaveKeyDef::EnclaveKey in_blueprintKey,
	EnclaveKeyDef::EnclaveKey in_oreKey)
{
	Message handlerToBDMMsg(MessageType::BDM_ORE_SKELETONSGM);
	
	// Each triangle to insert needs the following:
	//
	//	-the type (represented by an RCategoryEnum value)
	//	-the corresponding supergroup ID
	int totalNumbeOfHandlerEntries = 0;
	for (auto& currentTypeContainer : rTypesMap)
	{
		for (auto& currentContainer : currentTypeContainer.second.mappedContainers)
		{
			for (auto& currentContainerTriangle : currentContainer.second.rtVector)
			{
				// The category of triangles to insert is based on the RCategoryEnum key.
				//
				// -RCategoryEnum::TERRAIN_TILE_1 should correspond to any RDerivedTypeEnum::R_TILED derivative.
				switch (currentTypeContainer.first)
				{
					case RCategoryEnum::TERRAIN_TILE_1:
					{
						// The order of values, in this case, is the following:
						// 
						// -the category type (ie. RCategoryEnum)
						// -the corresponding supergroup
						// -data from the actual triangle

						handlerToBDMMsg.insertInt(int(RCategoryEnum::TERRAIN_TILE_1));	// TERRAIN_TILE_1 should always be this
						handlerToBDMMsg.insertInt(currentContainer.first);			// insert the super group ID
						handlerToBDMMsg.insertInt(int(currentContainerTriangle->getRenderingType()));	// get the derived type, store it

						// Append the actual data
						//handlerToBDMMsg.appendOtherMessage(&currentContainerTriangle->writeRTToMessage());	// insert the actual data; works with C++_17

						Message messageToAppend = currentContainerTriangle->writeRTToMessage();	// works with C++_20
						handlerToBDMMsg.appendOtherMessage(&messageToAppend);


						totalNumbeOfHandlerEntries++;
						break;
					}
				}
			}
		}
	}

	// After we have acquired all the skeletons and sent them into the message, we must put the following into the 
	// front of the message, in this order:
	// 
	// -total number of RenderableTriangleBase entries
	// -the key of the ORE 
	// -the blueprint key.
	handlerToBDMMsg.insertIntFront(totalNumbeOfHandlerEntries);
	handlerToBDMMsg.insertEnclaveKeyFront(in_oreKey);
	handlerToBDMMsg.insertEnclaveKeyFront(in_blueprintKey);


	return handlerToBDMMsg;
}

RenderableTriangleHandler::RenderableTriangleHandler(Message in_buildingMessage)
{
	// Step 1: open the message.
	in_buildingMessage.open();

	// Step 2: Read the total number of RenderableTriangleBase entries to build.
	int totalEntries = in_buildingMessage.readInt();

	for (int x = 0; x < totalEntries; x++)
	{
		RCategoryEnum currentRType = RCategoryEnum(in_buildingMessage.readInt());	// Read the RCategoryEnum (eg, TERRAIN_TILE_1), 
																			// so we know which category in the map it goes into.

		int currentSupergroup = in_buildingMessage.readInt();	// get the supergroup.

		RDerivedTypeEnum currentRenderingType = RDerivedTypeEnum(in_buildingMessage.readInt());	// get enum value that represents the derived type;
																								// this value will determine what derived 
																								// class of RenderableTriangleBase that we will use.

		// We must switch off of currentRenderingType, to determine the derived to class to use for the pointer. 
		// The category (currentRType) isn't as critical, as a category can have multiple RDerivedTypeEnum types.
		switch (currentRenderingType)
		{
			case RDerivedTypeEnum::R_TILED:
			{
				std::unique_ptr<RenderableTriangleBase> newTiledTriangle(new (RenderableTiledTriangle));
				newTiledTriangle->initFromMessage(&in_buildingMessage);
				newTiledTriangle->setRenderingType(RDerivedTypeEnum::R_TILED);
				rTypesMap[currentRType].insertRenderableTriangleIntoContainer(currentSupergroup, &newTiledTriangle);
				break;
			}
		}

	}
}

OperableIntSet RenderableTriangleHandler::appendSkeletonContainers(RenderableTriangleHandler* in_otherHandler)
{
	OperableIntSet appendedSupergroupValues;

	
	// Cycle through the TERRAIN_TILE_1 entry in the mappedContainers of the other referenced handler (in_otherHandler)
	for (auto& currentSupergroupToCopy : in_otherHandler->rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers)
	{
		// For each supergroup we find in the handler we are appending from, 
		// we must ensure a proper lookup value exists, for all the triangles that belong to a supergroup;
		// This is for functions that only know a given ID of an ECBPoly.
		uniqueIDLookup[currentSupergroupToCopy.first] = RCategoryEnum::TERRAIN_TILE_1;

		// Append the supegroup ID to the return value set.
		appendedSupergroupValues += currentSupergroupToCopy.first;

		// We won't be creating the individual blocks, but we will be doing a 1:1 copy of each triangle into the 
		// appropriate group.
		//
		// To do this correctly, we will create new unique_pointer that will contain a copy of the data in the referenced one;
		// we will then move this copy. This has to be done because we are appending existing data to this one;
		// however, because of unique_ptr ownership, using std::move via the referenced object will invalidate
		// the referenced unique_ptr (which we still want to keep), which will result in an error/crash. This is the reason
		// for the copy.
		for (auto& currentOtherTiledTriangle : currentSupergroupToCopy.second.rtVector)
		{
			std::unique_ptr<RenderableTriangleBase> triangleToTransferPtr;
			RenderableTiledTriangle* castedPtr = static_cast<RenderableTiledTriangle*>(currentOtherTiledTriangle.get());
			triangleToTransferPtr.reset(new RenderableTiledTriangle(*castedPtr));

			// make the triangle generate data, to fetch the touched blocks; so that we may insert the touchedBlockList.			
			RenderableGenerationResult currentGenerationResult = triangleToTransferPtr->generateData();
			for (auto& currentGroup : currentGenerationResult.producedFans)
			{
				touchedBlockList.insert(currentGroup.first);
			}
			

			rTypesMap[RCategoryEnum::TERRAIN_TILE_1].insertRenderableTriangleIntoContainer(currentSupergroupToCopy.first, &triangleToTransferPtr);
		}
	}

	return appendedSupergroupValues;
}

void RenderableTriangleHandler::insertTiledTriangles(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleContainer in_containerToConvert)
{
	uniqueIDLookup[in_supergroupID] = RCategoryEnum::TERRAIN_TILE_1;

	for (auto& currentEnclaveTriangleToConvert : in_containerToConvert.triangles)
	{
		std::unique_ptr<RenderableTriangleBase> newTiledTriangle(new (RenderableTiledTriangle));

		newTiledTriangle->initRenderable(currentEnclaveTriangleToConvert.second.points[0],
			currentEnclaveTriangleToConvert.second.points[1],
			currentEnclaveTriangleToConvert.second.points[2],
			currentEnclaveTriangleToConvert.second.emptyNormal,
			currentEnclaveTriangleToConvert.second.enclaveTriangleBoundaryPolyIndicator,
			currentEnclaveTriangleToConvert.second.isEnclaveTrianglePolyPerfectlyClamped);
		newTiledTriangle->setRenderingType(RDerivedTypeEnum::R_TILED);
		newTiledTriangle->setMaterialID(currentEnclaveTriangleToConvert.second.enclaveTriangleMaterialID);

		// make the triangle generate data, to fetch the touched blocks; so that we may insert the touchedBlockList.	
		RenderableGenerationResult currentGenerationResult = newTiledTriangle->generateData();
		for (auto& currentGroup : currentGenerationResult.producedFans)
		{
			touchedBlockList.insert(currentGroup.first);
		}
		

		rTypesMap[RCategoryEnum::TERRAIN_TILE_1].insertRenderableTriangleIntoContainer(in_supergroupID, &newTiledTriangle);

	}
}

void RenderableTriangleHandler::eraseSupergroup(int in_supergroupID)
{
	// First, lookup where the supergroup is.
	RCategoryEnum targetSupergroupCategory = uniqueIDLookup[in_supergroupID];
	rTypesMap[targetSupergroupCategory].eraseContainer(in_supergroupID);
}

bool RenderableTriangleHandler::containsRenderableTriangles()
{
	bool hasRenderables = false;

	// This function should only really be touching TERRAIN_TILE_1 entry; but will need to be updated at a later time
	// 
	for (auto& currentTiledData : rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers)
	{
		// Below: check each triangle in the rtVector, generate the results, and attempt to append into the block map.
		if (currentTiledData.second.rtVector.size() > 0)
		{
			// there is at least one triangle found; break and return
			hasRenderables = true;
			break;
		}
	}
	return hasRenderables;
}

void RenderableTriangleHandler::insertSkeletonContainerIntoSupergroup(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleSkeletonContainer in_skeletonContainer)
{
	// For each skeleton, make a RenderableTiledTriangle. Insert the supergroup (unique id) and TERRAIN_TILE_1 keyed value in uniqueIDLookup; 
	// then, insert into rTypesMap.

	// We must ensure a proper lookup value exists, for all the triangles that belong to a supergroup, 
	// for functions that only know a given ID of an ECBPoly.
	uniqueIDLookup[in_supergroupID] = RCategoryEnum::TERRAIN_TILE_1;

	for (auto& currentSkeletonToConvert : in_skeletonContainer.skeletons)
	{
		// The points from the in_skeletonContainer must be converted to FTrianglePoint instances;
		// this will require the usage of the new FTrianglePoint constructor that allows it to be built from a single ECBPolyPoint.
		// Remember, this means no UV or other data will be allocated to the FTrianglePoint.
		//std::shared_ptr<RenderableTriangleBase> newTiledTriangle(new (RenderableTiledTriangle));
		std::unique_ptr<RenderableTriangleBase> newTiledTriangle(new (RenderableTiledTriangle));

		newTiledTriangle->initRenderable(currentSkeletonToConvert.second.points[0],
			currentSkeletonToConvert.second.points[1],
			currentSkeletonToConvert.second.points[2],
			currentSkeletonToConvert.second.emptyNormal,
			currentSkeletonToConvert.second.skeletonBoundaryIndicatorValue,
			currentSkeletonToConvert.second.isPolyPerfectlyClamped);
		newTiledTriangle->setRenderingType(RDerivedTypeEnum::R_TILED);
		newTiledTriangle->setMaterialID(currentSkeletonToConvert.second.materialID);


		// Once all data has been setup for the tiled renderable triangle, send it on down to the appropriate container.
		rTypesMap[RCategoryEnum::TERRAIN_TILE_1].insertRenderableTriangleIntoContainer(in_supergroupID, &newTiledTriangle);

	}
}

void RenderableTriangleHandler::generateBlockTrianglesFromSecondaries(std::map<int, EnclaveBlockSkeleton>* in_skeletonMapRef,	
	std::map<int, EnclaveBlock>* in_enclaveBlockMapRef,												
	int* in_totalTrianglesRef)
{
	
	// This function should only really be touching TERRAIN_TILE_1 entry; so cycle through each container here.
	for (auto& currentTiledData : rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers)
	{
		// Below: check each triangle in the rtVector, generate the results, and attempt to append into the block map.
		for (auto& currentTiledContainer : currentTiledData.second.rtVector)
		{
			// Call on the current derivative class of the RenderableTriangleBase function, to generate the applicable data.
			RenderableGenerationResult currentGenerationResult = currentTiledContainer->generateData();

			//	...
			//	TODO: need to do something with incalculable results here...
			//	...

			for (auto& currentGroup : currentGenerationResult.producedFans)
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

bool RenderableTriangleHandler::scanForNullPointers()
{
	bool nullFound = false;

	for (auto& currentTiledData : rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers)
	{
		for (auto& currentTiledContainer : currentTiledData.second.rtVector)
		{
			if (!currentTiledContainer)
			{
				nullFound = true;
			}
		}
	}


	return nullFound;
}

std::map<int, EnclaveBlock> RenderableTriangleHandler::produceBlockCopies()
{
	std::map<int, EnclaveBlock> producedBlocks;

	// Ideally, this function should be able to handle more than just RCategoryEnum::TERRAIN_TILE_1; it
	// should theoretically be able to combine all necessary data and insert it into the returned block map.
	//
	// At this time, only RCategoryEnum::TERRAIN_TILE_1 is being used, but this will likely be changed sometime in 
	// the future, to accomodate for non-tile textures (logic isn't implemented yet, as of 11/30/2023)
	for (auto& currentTiledData : rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers)
	{
		// Below: check each triangle in the rtVector, generate the results, and attempt to append into the block map.
		for (auto& currentTiledContainer : currentTiledData.second.rtVector)
		{
			// Call on the current derivative class of the RenderableTriangleBase function, to generate the applicable data.
			RenderableGenerationResult currentGenerationResult = currentTiledContainer->generateData();

			//	...
			//	TODO: need to do something with incalculable results here (potentially)...
			//	...
		
			for (auto& currentGroup : currentGenerationResult.producedFans)
			{
				EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(currentGroup.first);
				int keyToSingle = PolyUtils::convertBlockCoordsToSingle(blockKey.x, blockKey.y, blockKey.z);
				EnclaveBlock* blockRef = &producedBlocks[keyToSingle];

				// Set the current blockRef to be the BlockSubMode from the group.
				blockRef->setBlockMode(currentGroup.second.groupSubType);

				// Remember, if the currentGroup was filled because incalculable (aka BlockSubType::BLOCK_NORMAL_FILLED), 
				// the block must first be reset before anything else.
				if (currentGroup.second.getFilledBecauseIncalculableValue() == true)
				{
					// Why was a -= operator even acceptable here? WHY :(
					blockRef->resetBlock();	// subtract the number of triangles that were in the block, and reset it.
				}

				// Insert the fan group; there is no need to fetch the number of triangles here.
				blockRef->insertFanGroup(currentGroup.second);
			}			
		}
	}
	return producedBlocks;
}

Operable3DEnclaveKeySet RenderableTriangleHandler::produceBlocksAndInvalids(std::map<int, EnclaveBlockSkeleton>* in_skeletonMapRef,
	std::map<int, EnclaveBlock>* in_enclaveBlockMapRef,
	int* in_totalTrianglesRef)
{
	Operable3DEnclaveKeySet incalculableBlocks;

	// Ideally, this function should be able to handle more than just RCategoryEnum::TERRAIN_TILE_1; it
	// should theoretically be able to combine all necessary data and insert it into the returned block map.
	//
	// At this time, only RCategoryEnum::TERRAIN_TILE_1 is being used, but this will likely be changed sometime in 
	// the future, to accomodate for non-tile textures (logic isn't implemented yet, as of 11/30/2023)
	for (auto& currentTiledData : rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers)
	{
		// Below: check each triangle in the rtVector, generate the results, and attempt to append into the block map.
		for (auto& currentTiledContainer : currentTiledData.second.rtVector)
		{
			// Call on the current derivative class of the RenderableTriangleBase function, to generate the applicable data.
			RenderableGenerationResult currentGenerationResult = currentTiledContainer->generateData();
			
			// Remember, for each call to generateData(), we must append any invalid blocks that were produced.
			incalculableBlocks += currentGenerationResult.invalidBlockSet;

			for (auto& currentGroup : currentGenerationResult.producedFans)
			{
				// insert the touched block into touchedBlockList.
				touchedBlockList.insert(currentGroup.first);

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

	return incalculableBlocks;
}

std::vector<EnclaveTriangle> RenderableTriangleHandler::retrieveTiledTrianglesForSupergroup(int in_superGroupID)
{
	std::vector<EnclaveTriangle> returnTriangles;

	auto targetSGFinder = rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers.find(in_superGroupID);
	if (targetSGFinder != rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers.end())
	{
		for (auto& currentTriangleInTargetGroup : rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers[in_superGroupID].rtVector)
		{
			EnclaveTriangle newTriangle;
			newTriangle.points[0] = IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(currentTriangleInTargetGroup->getPoint(0).point);
			newTriangle.points[1] = IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(currentTriangleInTargetGroup->getPoint(1).point);
			newTriangle.points[2] = IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(currentTriangleInTargetGroup->getPoint(2).point);
			newTriangle.emptyNormal = currentTriangleInTargetGroup->getEmptyNormal();
			newTriangle.enclaveTriangleMaterialID = currentTriangleInTargetGroup->fetchMaterialID();
			newTriangle.enclaveTriangleBoundaryPolyIndicator = currentTriangleInTargetGroup->getRBoundaryIndicator();

			returnTriangles.push_back(newTriangle);
		}
	}

	return returnTriangles;
}

std::vector<ORETerrainTriangle> RenderableTriangleHandler::produceTerrainTriangles()
{
	std::vector<ORETerrainTriangle> returnTriangles;

	
	for (auto& currentTiledData : rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers)
	{
		// Below: check each triangle in the rtVector, generate the results, and attempt to append into the block map.
		for (auto& currentTiledContainer : currentTiledData.second.rtVector)
		{
			ORETerrainTriangle newOTT(currentTiledContainer->getEmptyNormal(),
									currentTiledContainer->fetchMaterialID(),
									IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(currentTiledContainer->getPoint(0).point),
									IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(currentTiledContainer->getPoint(1).point),
									IndependentUtils::convertFTriangleDoublePointToECBPolyPoint(currentTiledContainer->getPoint(2).point)
								);
			returnTriangles.push_back(newOTT);
		}
	}
	

	return returnTriangles;
}

std::set<int> RenderableTriangleHandler::generateTouchedBlockList()
{


	/*
	std::set<int> touchedSet;	// the set to return
	for (auto& currentRenderableType : rTypesMap)
	{
		for (auto& currentRenderableData : currentRenderableType.second.mappedContainers)
		{
			// Below: check each triangle in the rtVector, generate the results, and attempt to append into the block map.
			for (auto& currentDataContainer : currentRenderableData.second.rtVector)
			{
				RenderableGenerationResult currentGenerationResult = currentDataContainer->generateData();
				for (auto& currentGroup : currentGenerationResult.producedFans)
				{
					touchedSet.insert(currentGroup.first);
				}
			}
		}
	}
	

	return touchedSet;
	*/

	/*
	if (touchedBlockList.empty())
	{
		for (auto& currentRenderableType : rTypesMap)
		{
			for (auto& currentRenderableData : currentRenderableType.second.mappedContainers)
			{
				// Below: check each triangle in the rtVector, generate the results, and attempt to append into the block map.
				for (auto& currentDataContainer : currentRenderableData.second.rtVector)
				{
					RenderableGenerationResult currentGenerationResult = currentDataContainer->generateData();
					for (auto& currentGroup : currentGenerationResult.producedFans)
					{
						//touchedSet.insert(currentGroup.first);
						touchedBlockList.insert(currentGroup.first);
					}
				}
			}
		}
	}
	*/

	return touchedBlockList;
}

int RenderableTriangleHandler::getTriangleCountFromContainers()
{
	int triangleCount = 0;

	
	for (auto& currentTiledData : rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers)
	{
		// Each triangle found in rtVector should increment the count by 1.
		for (auto& currentTiledContainer : currentTiledData.second.rtVector)
		{
			triangleCount++;
		}
	}
	

	return triangleCount;
}

void RenderableTriangleHandler::clear()
{
	uniqueIDLookup.clear();
	rTypesMap.clear();
	touchedBlockList.clear();
}

void RenderableTriangleHandler::printData()
{
	for (auto& currentTiledData : rTypesMap[RCategoryEnum::TERRAIN_TILE_1].mappedContainers)
	{
		std::cout << "Printing renderable triangles for supergroup: " << currentTiledData.first << std::endl;
		for (auto& currentContainerEntry : currentTiledData.second.rtVector)
		{
			currentContainerEntry->printStats();
		}
	}
}