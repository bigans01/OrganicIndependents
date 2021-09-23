#pragma once

#ifndef OrganicRawEnclave_H
#define OrganicRawEnclave_H

#include <map>
#include <set>
#include "EnclavePolyFractureResults.h"
#include "OrganicTriangleSecondary.h"
#include "EnclaveBlock.h"
#include "EnclaveTriangleContainer.h"
#include "EnclaveTriangleSkeletonContainer.h"
#include "ORELodState.h"
#include "OrganicTransformUtils.h"
#include "EnclaveBlockSkeleton.h"
#include <mutex>
#include "OperableIntSet.h"
#include "EnclaveBlock.h"
#include "OrganicTriangleSecondarySupergroupManager.h"
#include "EnclaveTriangleContainerSupergroupManager.h"
#include "EnclaveTriangleSkeletonSupergroupManager.h"
#include "GroupSetPair.h"
#include "ORELodState.h"
#include "OREAppendedState.h"
#include <vector>
#include "OREDependencyState.h"

class OrganicRawEnclave
{
public:

	OrganicRawEnclave();
	OrganicRawEnclave(ORELodState in_ORELodState);

	OrganicRawEnclave& operator=(const OrganicRawEnclave& resultsContainer_b)
	{
		currentLodState = resultsContainer_b.currentLodState;
		currentAppendedState = resultsContainer_b.currentAppendedState;
		currentDependencyState = resultsContainer_b.currentDependencyState;

		blockSkeletonMap = resultsContainer_b.blockSkeletonMap;
		skeletonSGM = resultsContainer_b.skeletonSGM;									// copy skeleton container map
		appendedUpdateCount = resultsContainer_b.appendedUpdateCount;
		if (!resultsContainer_b.finalizerBlocks.empty())												// copy finalizer blocks
		{																								// "" 
			auto b_containerBegin = resultsContainer_b.finalizerBlocks.begin();							// ""
			auto b_containerEnd = resultsContainer_b.finalizerBlocks.end();								// ""
			for (b_containerBegin; b_containerBegin != b_containerEnd; b_containerBegin++)
			{																							// ""	
				finalizerBlocks[b_containerBegin->first] = b_containerBegin->second;					// ""
			}																							// ""
		}																								// ""


		etcSGM = resultsContainer_b.etcSGM;					// copy 
		organicTriangleSecondarySGM = resultsContainer_b.organicTriangleSecondarySGM;														// copy, copy, MORE COPYYYYY
		// copy blocks, if the copied OrganicRawEnclave doesn't have its blockMap empty.
		if (!resultsContainer_b.blockMap.empty())														// copy 
		{																								// ""
			auto b_containerBegin = resultsContainer_b.blockMap.begin();								// ""
			auto b_containerEnd = resultsContainer_b.blockMap.end();									// ""
			for (b_containerBegin; b_containerBegin != b_containerEnd; b_containerBegin++)				// ""
			{																							// ""
				blockMap[b_containerBegin->first] = b_containerBegin->second;							// ""
			}																							// ""
		}																								// ""
		total_triangles = resultsContainer_b.total_triangles;
		return *this;
	}

	void insertOrganicTriangleSecondary(int in_polyID, int in_clusterID, OrganicTriangleSecondary in_enclavePolyFractureResults);
	void insertEnclaveTriangleContainer(int in_polyID, int in_clusterID, EnclaveTriangleContainer in_enclaveTriangleContainer);
	void insertBlockSkeleton(EnclaveKeyDef::EnclaveKey in_blockKey, EnclaveBlockSkeleton in_blockSkeleton);
																													// OrganicTriangleSecondaries to be present.			
	void printMapData();
	void printEnclaveTriangleContainers(bool in_pauseBetweenTrianglesFlag);
	void printTriangleMetadata();
	void printMetadata();
	void printTrianglesPerBlock();

	bool checkIfFull();
	int getNumberOfBlockSkeletons();
	void updateOREForRMass();					// switches the ORE to currentLodState of ORELodState::LOD_ENCLAVE_RMATTER,
												// it's currentDependencyState to OREDependencyState::INDEPENDENT, 
												// and clears out the blockSkeletonMap; the skeleton map must be cleared, because it's possible that an OrganicMassDriverElevator can produce
												// skeleton blocks before this function is called (see how it is used in ContouredMountain::runMassDrivers in the OrganicServerLib)

	void morphLodToBlock(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey);	// updates the ORE's currentLodState to be LOD_BLOCK, from LOD_ENCLAVE_RMATTER or LOD_ENCLAVE_SMATTER.
												
	bool doesOREContainRenderableData();																	// determines if the ORE contains any renderable data, be it generated via EnclaveTriangles, or EnclaveBlocks; 
	void appendSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, EnclaveTriangleSkeletonSupergroupManager in_enclaveTriangleSkeletonContainer);		// appends new enclave triangle skeletons, to the existing ones
	void reloadSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, EnclaveTriangleSkeletonSupergroupManager in_enclaveTriangleSkeletonContainer);		// clears out the old values in existingEnclaveTriangleSkeletonContainerTracker,
																																									// and wipes out the existing skeletons in skeletonSGM, 
																																									// before appending an entirely new series of skeleton containers. Needed by 
																																									// OREMatterCollider::extractResultsAndSendToORE in OrganicCoreLib.
	int getTotalTriangles();																				// returns the number of total triangles in the ORE.
	void loadSkeletonContainersFromEnclaveContainers();														// ***********WARNING: do not use this function if the ORE is part of a shared resource (multiple threads) ***************
																											// populates the skeletonSGM of this ore, by spawning skeletons from
																											// the etcSGM. This function should NOT

	int getNumberOfTrianglesByLOD();		// calculates the total number of triangles that would be returned by this ORE, if it was used to render GL data based on its LOD.															
	void spawnRenderableBlocks(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey);			// signals the ORE to produce the renderable blocks; used by OrganicSystem::jobProduceBlocksInORE
	void setOREasIndependent();																				// flags the ORE to have dependency state of INDEPENDENT; used by the function BlueprintMassManager::updatePersistentBlueprintPolys() 
																											// in OrganicServerLib.

	void simulateBlockProduction();			// debug function; will simulate block production by reading from skeletonSGM, without modifying contents of the ORE.
	void clearBlockSkeletons(std::mutex* in_mutexRef);		// clears the blockSkeletonMap
	std::set<int> getTouchedBlockList();																	// retrieves a list of blocks that were "touched" by the OrganicTriangleSecondaries; requires 

	EnclaveBlock* getBlockRefViaBlockKey(EnclaveKeyDef::EnclaveKey in_key);
	EnclaveTriangleSkeletonSupergroupManager spawnEnclaveTriangleSkeletonContainers();						// reads from the EnclaveTriangleContainer map to produce their corresponding EnclaveTriangleSkeletonContainers; the produced skeleton containers can then be sent (appended) to 
																											// a different instance of OrganicRawEnclave, using the function appendSpawnedEnclaveTriangleSkeletonContainers; see usage in 
																											// spawnAndAppendEnclaveTriangleSkeletonsToBlueprint.
	OperableIntSet getExistingEnclaveTriangleSkeletonContainerTracker();
																									// used by the RJPhasedBlueprintMM class in OrganicCoreLib to determine whether or not to use this ORE in rendering.
	ORELodState getLodState();																// returns the level-of-detail state of the ORE.
	OREAppendedState getAppendedState();													// returns the appended state of the ORE.
	OREDependencyState getDependencyState();												// returns the current dependency state of the ORE; needed by the function OrganicRawManifest::getNumberOfTrianglesFromModifiedOREs
																							// in OrganicCoreLib.
	GroupSetPair appendEnclaveTrianglesFromOtherORE(std::mutex* in_mutexRef, OrganicRawEnclave* in_otherEnclave);	// will append enclave triangles from another ORE; will also 
																													// set 
	std::vector<EnclaveTriangle> retriveAllEnclaveTrianglesForSupergroup(int in_superGroupID);				// returns a vector that contains all EnclaveTriangles found in a 
																											// EnclaveTriangleContainerSupergroup with a given ID.

	std::map<int, EnclaveBlock>::iterator getBlockMapBeginIter();		// fetch a begin iterator for the blockMap.
	std::map<int, EnclaveBlock>::iterator getBlockMapEndIter();			// fetch a end iterator for the blockMap.
	std::map<int, EnclaveBlock>::iterator getSpecificBlockIter(EnclaveKeyDef::EnclaveKey in_blockKey);	// attempts to find a specific iterator in blockMap.

	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| GROUP 1: For storing "Deflated" data;

	// skeletonSGM
	//
	// used to produce or "inflate" the data in a whole EnclaveTriangle; these are read from disk and used to populate the etcSGM;
	// it is designed to save memory and disk space.
	//
	// it has two main functions:
	// 1.) When "deflating" of EnclaveTriangles needs to be done, the results are stored here.
	// 2.) When "inflating" of EnclaveTriangles needs to be done, this map is read by this class's function,
	//     spawnEnclaveTriangleContainers.
	// contains blocks that must be run in a final pass, if there are any to run.
	EnclaveTriangleSkeletonSupergroupManager skeletonSGM;

	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| GROUP 2: For storing "Inflated" data; generated at run time during high-LOD terrain requests.

	// etcSGM
	// 
	// This map has two functions:
	// 1.) When "deflating" of EnclaveTriangles needs to be done, each element in this map is read to produce an EnclaveTriangleSkeletonContainer,
	//     each of which is inserted into the skeletonSGM.
	//
	// 2.) When "inflating" of EnclaveTriangles needs to be done, each element in the skeletonSGM is read to produce a corresponding
	//     enclave triangle container here.

	//std::map<int, EnclaveTriangleContainerSupergroup> etcSGM;	
	EnclaveTriangleContainerSupergroupManager etcSGM;
	OrganicTriangleSecondarySupergroupManager organicTriangleSecondarySGM;
	std::map<int, EnclaveBlock> blockMap;	// this map is built when the OrganicSystem requires a high LOD terrain job, such as RJPhasedBlueprintMM (see OrganicCoreLib)
											// contains individual blocks which can be used to render (can be read from a file as well); otherwise, they are generated from OrganicTriangleSecondaries.

private:
	ORELodState currentLodState = ORELodState::LOD_ENCLAVE_SMATTER;							// the level-of-detail state; always assumed to be LOD_ENCLAVE when initialized, but can be overriden with constructor #2 (see above)
	OREAppendedState currentAppendedState = OREAppendedState::NONE;							// the AppendedState reflects how many different attempts there have been to add EnclaveTriangles to this ORE.
	OREDependencyState currentDependencyState = OREDependencyState::DEPENDENT_ON_PARENTS;	// this state determines whether or not this ORE should be rendered when trying to render the entire blueprint's contents;
																							// if this state is INDEPENDENT, the ORE will be rendere when trying to render side-by-side with a blueprint's ECBPolys.

	void updateCurrentAppendedState();		// updates the appended state to SINGLE_APPEND or MULTIPLE_APPEND
	void resetBlockDataAndTriangleCount();																							// clears the blockMap, and resets triangle count
	void spawnEnclaveTriangleContainers(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey);			// reads from the skeletonSGM to produce their corresponding EnclaveTriangleContainers
	void createBlocksFromOrganicTriangleSecondaries(std::mutex* in_mutexRef);										// spawn the EnclaveBlocks, and Fans from the OrganicTriangleSecondaries; used when the currentState is
																													// in LOD_ENCLAVE or LOD_ENCLAVE_MODIFIED

	void insertOrganicTriangleSecondaryIntoRefedManager(OrganicTriangleSecondarySupergroupManager* in_refedManager,	// inserts OrganicTriangleSecondaries into a refed OrganicTriangleSecondarySupergroupManager;
														int in_polyID,												// needed by the function, simulateBlockProduction(). 
														int in_clusterID, 
														OrganicTriangleSecondary in_enclavePolyFractureResults);

	std::map<int, EnclaveBlockSkeleton> blockSkeletonMap;
	std::map<int, EnclaveBlock> finalizerBlocks;							// unused for now	
	OperableIntSet existingEnclaveTriangleSkeletonContainerTracker;			// returns a set that represents the unique IDs of EnclaveTriangles; used by OrganicSystem::spawnAndAppendEnclaveTriangleSkeletonsToBlueprint,
																			// in order to distinguish between "old" and "new" sets of an SPolySet.
	int total_triangles = 0;								// the total number of renderable triangles; incremented/decremented according to the number of triangles per BBFan; determined at run time
	int appendedUpdateCount = 0;							// the total number of times the appended state has been updated in the ORE.
};

#endif
