#pragma once

#ifndef RENDERABLETRIANGLEHANDLER_H
#define RENDERABLETRIANGLEHANDLER_H

#include "EnclaveTriangleSkeletonContainer.h"

#include "RenderableTriangleContainerManager.h"
#include "RTypeEnum.h"
#include "RenderableTiledTriangle.h"
#include "EnclaveBlockSkeleton.h"
#include "EnclaveBlock.h"
#include "ORETerrainTriangle.h"
#include "EnclaveTriangleContainer.h"

/*

Description: This class is meant to serve as a single object that manages various types of renderable/usable data for an OrganicRawEnclave. 
It is meant to replace the skeletonSGM, etcSGM and organicTriangleSecondarySGM members of the ORE, while utilizing all of their functionality.
Once all of this the functionality has been tested, these members will be replaced/deleted.

The old model of these members only allowed storage of EnclaveTriangle and EnclaveBlock data in a limited manner; this new object is intended
to have advanced features that its predecessors did not, such as the ability to store different types of rendering data. When properly built,
this will allow for the querying of many potental types of data (thanks to the RenderableTriangleBase-derived objects stored in RenderableTriangleContainers).

The main feature of this object will be the ability to store both tiled texture data, and typical rendering texture data.

*/

class RenderableTriangleHandler
{
	public:

		// For below: meant to mirror EnclaveTriangleContainerSupergroupManager::insertEnclaveTriangle; behaves similiar to 
		// insertSkeletonContainerIntoSupergroup in this class, except that the data comes from EnclaveTriangleContainer.
		// 
		// The function EnclaveFractureResultsMap::transferEnclaveTrianglesIntoOREs is the primary method for EnclaveTriangle insertion, from an 
		// existing OrganicTriangle; the function calls EnclaveFractureResultsMap::insertFractureResults for each detected EnclavePoly in 
		// a referenced OrganicTriangle, which subsequently calls OrganicRawEnclave::insertEnclaveTriangleContainer. The last function mentioned,
		// is what should be responsible for calling this below function.
		void insertEnclaveTriangles(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleContainer in_containerToConvert);

		// For below: this is a mirror of EnclaveTriangleSkeletonSupergroupManager::eraseSupergroup. Would be needed when a call to 
		// OrganicRawEnclave::removeSkeletonSupergroup is issued.
		void eraseSupergroup(int in_supergroupID);

		// For below: meant to mirror EnclaveTriangleSkeletonSupergroupManager::appendSkeletonContainers;
		// Should be utilized when the function OrganicRawEnclave::appendSpawnedEnclaveTriangleSkeletonContainers is called.
		OperableIntSet appendSkeletonContainers(RenderableTriangleHandler* in_otherHandler);

		// For below: in reality, this will be refactored later; but it's being done in this way so that existing code that uses the definition from 
		// EnclaveTriangleContainerSupergroupManager will still be happy with the below call. The value of in_skeletonContainerID would not be used.
		// should only touch the entry in the rTypesMap that is keyed to RTypeEnum::TERRAIN_TILE_1, for the time being.
		//
		// This is needed by classes that seek to load up data into a fresh RenderableTriangleHandler, before passing it down to the ORE -- so it should be public.
		// An example of a function using this, in this manner, is OREMatterCollider::extractResultsAndSendToORE, in OrganicCoreLib.
		void insertSkeletonContainerIntoSupergroup(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleSkeletonContainer in_skeletonContainer);


		// For below: this function is from OrganicTriangleSecondarySupergroupManager. It is what actually produces the fan data that goes into 
		// blocks; the total number of triangles from an insertion is then added to the referenced in_totalTrianglesRef. For the time being, 
		// this should only be used with the rTypesMap entry at RTypeEnum::TERRAIN_TILE_1.
		//
		// Should mirror the usage in OrganicRawEnclave::createBlocksFromOrganicTriangleSecondaries.
		void generateBlockTrianglesFromSecondaries(std::map<int, EnclaveBlockSkeleton>* in_skeletonMapRef,	// generates the triangles for EnclaveBlocks, and loads them into the passed in map, 
			std::map<int, EnclaveBlock>* in_enclaveBlockMapRef,												// and also updates the number of total triangles in the ORE.
			int* in_totalTrianglesRef);

		// For below: this function should functionally all of the combined code in the OrganicRawEnclave::produceBlockCopies function.
		std::map<int, EnclaveBlock> produceBlockCopies();

		// For below: this is performs in a logical manner to OrganicRawEnclave::spawnContainersAndCreateBlocks, minus the call to OrganicRawEnclave::resetBlockDataAndTriangleCount();
		// before the below function is called, the ORE instance that calls this function needs to call resetBlockDataAndTriangleCount().
		Operable3DEnclaveKeySet produceBlocksAndInvalids(std::map<int, EnclaveBlockSkeleton>* in_skeletonMapRef,
														std::map<int, EnclaveBlock>* in_enclaveBlockMapRef,
														int* in_totalTrianglesRef);

		// For below: this function should produce the same results as found in the function, EnclaveTriangleSkeletonSupergroupManager::produceTerrainTriangles;
		// it just has to do it in a compatible way. For the time being, this should only be working with TERRAIN_TILE_1 rTypesMap entry.
		std::vector<ORETerrainTriangle> produceTerrainTriangles();


		// For below: functionally the same as OrganicTriangleSecondarySupergroupManager::generateTouchedBlockList. Would have to go through all entries in rTypesMap
		// to produce correctly. Does not store the result anywhere, so constant calls to this function will always generate RenderableGenerationResults that are then
		// parsed and quickly discarded. Needed for functions such as OrganicMassDriverElevator::applyMass in OrganicCoreLib.
		std::set<int> generateTouchedBlockList();

		int getTriangleCountFromContainers();	// A functional copy of EnclaveTriangleSkeletonSupergroupManager::getTriangleCountFromContainers(). The count should only
												// come from the TERRAIN_TILE_1 entry in rTypesMap. Needed by OrganicRawEnclave::getNumberOfTrianglesByLOD. This would be called in
												// cases where a remote terrain rendering job has to scan for independent OREs to combine those polys with actual ECBPolys, for a single draw call.

		void clear();	// wipe out everything, start from scratch, clean up memory etc; equivalent of calling clear() on all 3 of the old data maps in OrganicRawEnclave
						// (i.e., how OrganicRawEnclave::checkIfFull() calls clear on skeletonSGM, etcSGM, and organicTriangleSecondarySGM.

		void printData();

	private:
		std::map<RTypeEnum, RenderableTriangleContainerManager> rTypesMap;
		std::map<int, RTypeEnum> uniqueIDLookup;	// used to determine the corresponding container that a specific unique ID can be found, in the rTypesMap;
													// Needed by functions such as OrganicRawEnclave::retriveAllEnclaveTrianglesForSupergroup, which need to retrieve
													// EnclaveTriangles, but only knows a given ECBPoly ID.

		std::set<int> touchedBlockList;	// Still in-development/testing: blocks that were generated as a result of a call to generateBlockTrianglesFromSecondaries 
										// (or another similiar function) can go here.
};

#endif
