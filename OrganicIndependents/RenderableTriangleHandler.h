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
		// For below: in reality, this will be refactored later; but it's being done in this way so that existing code that uses the definition from 
		// EnclaveTriangleContainerSupergroupManager will still be happy with the below call. The value of in_skeletonContainerID would not be used.
		// should only touch the entry in the rTypesMap that is keyed to RTypeEnum::TERRAIN_TILE_!, for the time being.
		void insertSkeletonContainerIntoSupergroup(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleSkeletonContainer in_skeletonContainer);


		// For below: this function is from OrganicTriangleSecondarySupergroupManager. It is what actually produces the fan data that goes into 
		// blocks; the total number of triangles from an insertion is then added to the referenced in_totalTrianglesRef. For the time being, 
		// this should only be used with the rTypesMap entry at RTypeEnum::TERRAIN_TILE_1.
		void generateBlockTrianglesFromSecondaries(std::map<int, EnclaveBlockSkeleton>* in_skeletonMapRef,	// generates the triangles for EnclaveBlocks, and loads them into the passed in map, 
			std::map<int, EnclaveBlock>* in_enclaveBlockMapRef,												// and also updates the number of total triangles in the ORE.
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

	private:
		std::map<RTypeEnum, RenderableTriangleContainerManager> rTypesMap;
		std::map<int, RTypeEnum> uniqueIDLookup;	// used to determine the corresponding container that a specific unique ID can be found, in the rTypesMap;
													// Needed by functions such as OrganicRawEnclave::retriveAllEnclaveTrianglesForSupergroup, which need to retrieve
													// EnclaveTriangles, but only knows a given ECBPoly ID.

		std::set<int> touchedBlockList;	// Still in-development/testing: blocks that were generated as a result of a call to generateBlockTrianglesFromSecondaries 
										// (or another similiar function) can go here.
};

#endif
