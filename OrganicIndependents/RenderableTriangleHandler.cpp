#include "stdafx.h"
#include "RenderableTriangleHandler.h"

void RenderableTriangleHandler::insertSkeletonContainerIntoSupergroup(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleSkeletonContainer in_skeletonContainer)
{
	// For each skeleton, make a RenderableTiledTriangle. Insert the supergroup (unique id) and TERRAIN_TILE_1 keyed value in uniqueIDLookup; 
	// then, insert into rTypesMap.
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

		// We must ensure a proper lookup value exists, for functions that only know a given ID of an ECBPoly.
		uniqueIDLookup[in_supergroupID] = RTypeEnum::TERRAIN_TILE_1;

		// Once all data has been setup for the tiled renderable triangle, send it on down to the appropriate container.
		rTypesMap[RTypeEnum::TERRAIN_TILE_1].insertRenderableTriangleIntoContainer(in_supergroupID, &newTiledTriangle);

	}
}

void RenderableTriangleHandler::generateBlockTrianglesFromSecondaries(std::map<int, EnclaveBlockSkeleton>* in_skeletonMapRef,	
	std::map<int, EnclaveBlock>* in_enclaveBlockMapRef,												
	int* in_totalTrianglesRef)
{
	
	// This function should only really be touching TERRAIN_TILE_1 entry; so cycle through each container here.
	for (auto& currentTiledData : rTypesMap[RTypeEnum::TERRAIN_TILE_1].mappedContainers)
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

std::vector<ORETerrainTriangle> RenderableTriangleHandler::produceTerrainTriangles()
{
	std::vector<ORETerrainTriangle> returnTriangles;

	
	for (auto& currentTiledData : rTypesMap[RTypeEnum::TERRAIN_TILE_1].mappedContainers)
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
}

int RenderableTriangleHandler::getTriangleCountFromContainers()
{
	int triangleCount = 0;

	
	for (auto& currentTiledData : rTypesMap[RTypeEnum::TERRAIN_TILE_1].mappedContainers)
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