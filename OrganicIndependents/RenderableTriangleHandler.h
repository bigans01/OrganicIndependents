#pragma once

#ifndef RENDERABLETRIANGLEHANDLER_H
#define RENDERABLETRIANGLEHANDLER_H

#include "EnclaveTriangleSkeletonContainer.h"

#include "RenderableTriangleContainerManager.h"
#include "RCategoryEnum.h"
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

		RenderableTriangleHandler(Message in_buildingMessage);	// populates the rTypesMap of the RenderableTriangleHandler
																// with data from a Message; the Message will have data for instantianting various data types,
																// such as RenderableTiledTriangle. The blueprint and ORE keys that were originally part
																// of the Message must have already been stripped off before calling this function,
																// otherwise an error will likely occur.

		RenderableTriangleHandler() {};	// required default constructor to make C++ happy, since we have a constructor that instantiates from a Message.

		// For below: meant to mirror EnclaveTriangleContainerSupergroupManager::insertEnclaveTriangle; behaves similiar to 
		// insertSkeletonContainerIntoSupergroup in this class, except that the data comes from EnclaveTriangleContainer.
		// 
		// The function EnclaveFractureResultsMap::transferEnclaveTrianglesIntoOREs is the primary method for EnclaveTriangle insertion, from an 
		// existing OrganicTriangle; the function calls EnclaveFractureResultsMap::insertFractureResults for each detected EnclavePoly in 
		// a referenced OrganicTriangle, which subsequently calls OrganicRawEnclave::insertEnclaveTriangleContainer. The last function mentioned,
		// is what should be responsible for calling this below function.
		void insertTiledTriangles(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleContainer in_containerToConvert);

		// For below: this is a mirror of EnclaveTriangleSkeletonSupergroupManager::eraseSupergroup. Would be needed when a call to 
		// OrganicRawEnclave::removeSkeletonSupergroup is issued.
		void eraseSupergroup(int in_supergroupID);

		bool containsRenderableTriangles();	// Scans the RCategoryEnum::TERRAIN_TILE_1 entry in rTypesMap, to see if any triangles exist; will return true 
											// upon detecting the first available triangle.

		// For below: meant to mirror EnclaveTriangleSkeletonSupergroupManager::appendSkeletonContainers;
		// Should be utilized when the function OrganicRawEnclave::appendSpawnedEnclaveTriangleSkeletonContainers is called.
		//
		// Designed for the RCategoryEnum::TERRAIN_TILE_1-keyed entry in rTypesMap,
		// this function will analyze all the unique_ptr base instances of the other handler, and create equivalent 
		// versions of each one, before inserting each one into the calling container.
		OperableIntSet appendSkeletonContainers(RenderableTriangleHandler* in_otherHandler);

		// For below: in reality, this will be refactored later; but it's being done in this way so that existing code that uses the definition from 
		// EnclaveTriangleContainerSupergroupManager will still be happy with the below call. The value of in_skeletonContainerID would not be used.
		// should only touch the entry in the rTypesMap that is keyed to RCategoryEnum::TERRAIN_TILE_1, for the time being.
		//
		// This is needed by classes that seek to load up data into a fresh RenderableTriangleHandler, before passing it down to the ORE -- so it should be public.
		// An example of a function using this, in this manner, is OREMatterCollider::extractResultsAndSendToORE, in OrganicCoreLib.
		void insertSkeletonContainerIntoSupergroup(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleSkeletonContainer in_skeletonContainer);


		// For below: this function is from OrganicTriangleSecondarySupergroupManager. It is what actually produces the fan data that goes into 
		// blocks; the total number of triangles from an insertion is then added to the referenced in_totalTrianglesRef. For the time being, 
		// this should only be used with the rTypesMap entry at RCategoryEnum::TERRAIN_TILE_1.
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

		std::vector<EnclaveTriangle> retrieveTiledTrianglesForSupergroup(int in_superGroupID);	// return all tiled triangles for a given supergroup, if they exist; the RenderableTiledTriangle instances
																								// are converted to an equivalent EnclaveTriangle format, and put into a vector.

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

		bool scanForNullPointers();	// utility/debug: scans through all RenderableTriangleBase pointers, to see if any are null. 

		Message convertHandlerToBDM(EnclaveKeyDef::EnclaveKey in_blueprintKey,	// converts this instance of RenderableTriangleHandler into a 
								    EnclaveKeyDef::EnclaveKey in_oreKey);		// digestable Message format; the Message should contain the blueprint key and ORE key (in that order)
																				// at the front of the Message.


	private:
		std::map<RCategoryEnum, RenderableTriangleContainerManager> rTypesMap;
		std::map<int, RCategoryEnum> uniqueIDLookup;	// used to determine the corresponding container that a specific unique ID can be found, in the rTypesMap;
													// Needed by functions such as OrganicRawEnclave::retrieveHandlerTiledTriangles, which need to retrieve
													// RenderableTiledTriangles, but when only a given supergroup ID is available.

		std::set<int> touchedBlockList;	// NOTE: Currently, this won't get copied over if instantiating a RenderableTriangleHandler from a Message; this is because 
										// the touchedBlockList only needs to really be used for mass driving; Building a RenderableTriangleHandler from a Message is really
										// only utilized when using the BDM methodology to reconstruct a blueprint, which would be after mass driving has completed (the blueprint
										// would then be sent over to an OrganicSYstem instance, over networking etc)
										//
										// When mass-driving is being performed, this std::set is checked against to see what blocks in an ORE are considered "exposed",
										// when an EnclaveTriangle has executed its run. The list gets updated when during the following calls of this class:
										//
										// -insertTiledTriangles
										// -appendSkeletonContainers
										// -produceBlocksAndInvalids
};

#endif
