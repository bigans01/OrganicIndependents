#pragma once

#ifndef OrganicRawEnclave_H
#define OrganicRawEnclave_H

#include "EnclavePolyFractureResults.h"
#include "OrganicTriangleSecondary.h"
#include "EnclaveBlock.h"
#include "EnclaveBlockState.h"
#include "EnclaveTriangleContainer.h"
#include "EnclaveTriangleSkeletonContainer.h"
#include "ORELodState.h"
#include "OrganicTransformUtils.h"
#include "EnclaveBlockSkeleton.h"
#include "OrganicTriangleSecondarySupergroupManager.h"
#include "EnclaveTriangleContainerSupergroupManager.h"
#include "EnclaveTriangleSkeletonSupergroupManager.h"
#include "GroupSetPair.h"
#include "ORELodState.h"
#include "OREAppendedState.h"
#include "OREDependencyState.h"
#include "Operable3DEnclaveKeySet.h"
#include "BlockCopyQuery.h"
#include "Operable3DEnclaveKeySet.h"
#include "MessageContainer.h"

/*

Description:

This single class is meant to store and render EnclaveTriangles, and EnclaveBlocks. Numerous subsystems in an OrganicSystem
will likely interface with this class. Many of the flags, interfacing, and setup must be done by other subsystems (except for the private 
members of this class). Information about how this class operates (it's short hand being called ORE) will be updated sporadically, over time,
either here in the header file or through a separate document (such as a Google doc). See "Notes" below for primers on important
components of this class

---------------------------------------------------------------------------------------------------------------------------------------------------
NOTE #1	->	How EnclaveBlocks are produced 

	There are several ways to produce EnclaveBlocks, such as in calling spawnRenderableBlocks(), but in all cases, 
	some data must be already be populated befor this is possible:

	1.	The skeletonSGM instance must be populated with relevant EnclaveTriangleSkeleton supergroups.

	2.  The data from step 1 above is used to inflate EnclaveTriangleContainer instances, which are then used to populate the etcSGM member. This
		correlates with "PART 1" of spawnEnclaveTriangleContainers.

	3.	After the EnclaveTriangleContainers have been inflated in the etcSGM member, the executeRun() function is called on each EnclaveTriangle in each
		EnclaveTriangleContainer. This populates the OrganicTriangleTertiary of each EnclaveTriangle, which contains a map of OrganicWrappedBBFans 
		that were produced by the EnclaveTriangle. Invalid EnclaveTriangles are also purged here.
		This correlates with "PART 2" of spawnEnclaveTriangleContainers.

	4.	We cycle through etcSGM once again, except this time, we take create an OrganicTriangleSecondary from each EnclaveTriangleContainer in each supergroup
		of the etcSGM. Each OrganicTriangleSecondary is then inserted into organicTriangleSecondarySGM. This is "PART 3" of spawnEnclaveTriangleContainers.

	Steps 2 through 4 above are handled by the function spawnEnclaveTriangleContainers().
	Once step 4 is complete, blocks can be spawned, via functions such as createBlocksFromOrganicTriangleSecondaries.



*/

class OrganicRawEnclave
{
public:

	OrganicRawEnclave();
	OrganicRawEnclave(ORELodState in_ORELodState);	// For the time being (circa ~8/17/2023), this is pretty much exclusively used by the function OrganicMassDriverElevator::applyMass in OrganicCoreLib;
													// that function uses a value of ORELodState::FULL. When the input parameter is detected as ORELodState::FULL, all 64 entries
													// of the ORE become skeletons. In the future, this logic should be improved so that different types of block skeletons can be inserted.

	OrganicRawEnclave& operator=(const OrganicRawEnclave& resultsContainer_b)
	{
		currentLodState = resultsContainer_b.currentLodState;
		currentAppendedState = resultsContainer_b.currentAppendedState;
		currentDependencyState = resultsContainer_b.currentDependencyState;

		blockSkeletonMap = resultsContainer_b.blockSkeletonMap;
		skeletonSGM = resultsContainer_b.skeletonSGM;									// copy skeleton container map
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

	// ||||||||||| EnclaveTriangle and OrganicTriangleSecondary manipulation (only known usage is from EnclaveFractureResultsMap::insertFractureResults in OrganicCoreLib)
	void insertOrganicTriangleSecondary(int in_polyID, int in_clusterID, OrganicTriangleSecondary in_enclavePolyFractureResults);
	void insertEnclaveTriangleContainer(int in_polyID, int in_clusterID, EnclaveTriangleContainer in_enclaveTriangleContainer);




	// ||||||||||| Block manipulation and fetching
	void insertBlockSkeleton(EnclaveKeyDef::EnclaveKey in_blockKey, EnclaveBlockSkeleton in_blockSkeleton);
	int getNumberOfBlockSkeletons();
	void eraseBlock(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_blockKey);				// erases a block if it exists, and decrements the total_triangle count by that amount (if it had triangles).
																									// The mutex used should be the mutex of the class/map/container that the ORE belongs in, for multithreading safety.

	void insertBlock(std::mutex* in_mutexRef, int in_blockIndex, EnclaveBlock in_blockToInsert);	// operationally the same as eraseBlock, with the added fact that we insert a whole new EnclaveBlock as well.
	void clearBlockSkeletons(std::mutex* in_mutexRef);		// clears the blockSkeletonMap
	int printBlockData(EnclaveKeyDef::EnclaveKey in_blockKey);								// prints any existing "secondaries" (triangle fans) in an EnclaveBlock, if that block exists.
	bool doesBlockSkeletonExistNoMutex(EnclaveKeyDef::EnclaveKey in_blockKey);				// checks to see if a block exists as a skeleton, should only ever be used with the function
																							// ECBMap::getBlockStateFromPopulatedORE.
	Operable3DEnclaveKeySet spawnRenderableBlocks(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey);			// -->	signals the ORE to produce the renderable blocks; used by OrganicSystem::jobProduceBlocksInORE.
																																// The return value of this function should indicate any blocks that the EnclaveTriangles attempted to produce, but couldn't.
																																// This functioanality is needed to keep track of individual EnclaveBlock instances that need to be mitigated/logged.
	std::map<int, EnclaveBlock>::iterator getBlockMapBeginIter();		// fetch a begin iterator for the blockMap.
	std::map<int, EnclaveBlock>::iterator getBlockMapEndIter();			// fetch a end iterator for the blockMap.
	std::map<int, EnclaveBlock>::iterator getSpecificBlockIter(EnclaveKeyDef::EnclaveKey in_blockKey);	// attempts to find a specific iterator in blockMap.

	EnclaveBlock* getBlockRefViaBlockKey(EnclaveKeyDef::EnclaveKey in_key);	// gets a specific block given a key (NOTE: make sure the block exists via other means, before calling this!)

	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchUnexposedBlockKeys();	// loads any associated keys of blocks that are classified as UNEXPOSED into a set, if any exist.
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchExposedBlockKeys(); // loads any associated keys of blocks that are classified as EXPOSED into a set, if any exist.
	BlockCopyQuery queryForBlockCopy(EnclaveKeyDef::EnclaveKey in_blockKey);	// Currently this function is not designed to be used with any ORE that has a currentLodState of FULL. It is not designed to be used on those (yet).
																				// similiar in structure to fetchExposedBlockKeys(), this function attempts to acquire a copy of an EnclaveBlock, if that block exists, can be generated, 
																				// or is even UNEXPOSED; the bool value of BlockCopyQuery will determine whether or not it was appropriately set; A 
																				// bool value of false in BlockCopyQuery.wasFound means that a block was not found, while true indicates it was found. 
																				// In other words, false would only be returned for a non-existent block.



	
	// ||||||||||| Miscellaneous print functions
	void printMapData();
	void printEnclaveTriangleContainers(bool in_pauseBetweenTrianglesFlag);
	void printTriangleMetadata();
	void printMetadata();
	void printTrianglesPerBlock();

	// ||||||||||| Various getter functions
	int getNumberOfTrianglesByLOD();		// calculates the total number of triangles that would be returned by this ORE, if it was used to render GL data based on its LOD.															
	int getTotalTriangles();				// returns the number of total triangles in the ORE.
	int getTotalUnexposedBlocks();			// gets the total number of blocks in the blockSkeletonMap.
	std::set<int> getTouchedBlockList();																	// retrieves a list of blocks that were "touched" by the OrganicTriangleSecondaries; requires
	std::set<int> getUnexposedBlockList();
	std::map<int, EnclaveBlockSkeleton> getUnexposedBlocksCopy();

	// ||||||||||| BDM conversion functions.
	MessageContainer convertOREToBDMFormat(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey);		//	Constructs a series of Messages relevant to an ORE instance,
																																//	that would allow the ORE to be recreated from these Messages.
																																//  The container should contain the following Messages:
																																//
																																//	1 BDM_ORE_HEADER
																																//	1 BDM_ORE_SKELETONSGM
																																//  Up to 64 BDM_BLOCK_TAGGED
																																//
																																// The values of the in_blueprintKey and in_oreKey paramaters are used to append tagging data
																																// to whichever Messages may need them.

	// ||||||||||| Specialized functions.
	std::map<int, EnclaveBlock> produceBlockCopies();	// reads data straight from the skeletonSGM, to produce a copy of a map of exposed EnclaveBlocks that 
														// would be produced by this SGM; the simulated copies are put into the value returned by this function.
														// Currently only used by printBlockCategorizations below, but can be used elsewhere. This will do nothing if 
														// the ORE's currentLodState is already in LOD_BLOCK, as there will be nothing in etcSGM to read from.
														// 
														// The use of this function is mostly private; but some classes such as ORELightweightCollider may use it as well.

	bool checkIfFull();		// analyzes the number of unexposed blocks, to determine if the ORE is FULL. If the number equals 64, the currentLodState is updated to FULL,
							// and the skeletonSGM, etcSGM and organicTriangleSecondarySGM are cleared out; the total_triangles value also gets reset to 0. This case can occur in OrganicCoreLib functions,
							// OrganicMassDriverElevator::proceedToNextFloorAndUpdateMassDrivers() and OrganicMassDriverElevator::runDriversForStartingFloor(),
							// which call this function. This function should only be used by the OrganicMassDriverElevator class.
	void updateOREForRMass();					// switches the ORE to currentLodState of ORELodState::LOD_ENCLAVE_RMATTER,
												// it's currentDependencyState to OREDependencyState::INDEPENDENT, 
												// and clears out the blockSkeletonMap; the skeleton map must be cleared, because it's possible that an OrganicMassDriverElevator can produce
												// skeleton blocks before this function is called (see how it is used in ContouredMountain::runMassDrivers in the OrganicServerLib).
												// It will then set the value of the blockSkeletonMap to be the rMassSolidMap, which is the only way 
												// to update the blockSkeletonMap with new solid blocks, when in an ORELodState::LOD_ENCLAVE_RMATTER.

	void morphLodToBlock(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey);	// updates the ORE's currentLodState to be LOD_BLOCK, from a state of LOD_ENCLAVE_RMATTER, LOD_ENCLAVE_SMATTER, or FULL.
												
	bool doesOREContainRenderableData();																	// determines if the ORE contains any renderable data, be it generated via EnclaveTriangles, or EnclaveBlocks; 
	void appendSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, EnclaveTriangleSkeletonSupergroupManager in_enclaveTriangleSkeletonContainer);		// appends new enclave triangle skeletons, to the existing ones; also updates the appended state of the ORE.
	void reloadSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, EnclaveTriangleSkeletonSupergroupManager in_enclaveTriangleSkeletonContainer);		// clears out the old values in existingEnclaveTriangleSkeletonContainerTracker,
																																									// and wipes out the existing skeletons in skeletonSGM, 
																																									// before appending an entirely new series of skeleton containers. Needed by 
																																									// OREMatterCollider::extractResultsAndSendToORE in OrganicCoreLib.
	void removeSkeletonSupergroup(std::mutex* in_mutexRef, int in_supergroupIDToRemove);
	void loadSkeletonContainersFromEnclaveContainers();														// ***********WARNING: do not use this function if the ORE is part of a shared resource (multiple threads) ***************
																											// populates the skeletonSGM of this ore, by spawning skeletons from
																											// the etcSGM. This function should NOT

	void setOREasIndependent();																				// flags the ORE to have dependency state of INDEPENDENT; used by the function BlueprintMassManager::updatePersistentBlueprintPolys() 
																											// in OrganicServerLib.

	void setPendingRMatterSolids(std::mutex* in_mutexRef,													// sets "pending" solid blocks for each EnclaveKey in the passed-in set as skeletons. This must be called whenever 
								Operable3DEnclaveKeySet in_skeletonBlockSet,								// the solid blocks of an ORE in a currentLodState of ORELodState::LOD_ENCLAVE_RMATTER needs to be updated, AND
								Operable3DEnclaveKeySet in_filledClaimedNonSolids);							// before the call to updateOREForRmass. It also checks to ensure that RMorphableMesh objects (see OrganicGLWinLib)
																											// that were originally LANDLOCKED (i.e, a blockSkeleton) but flagged for removal --AND-- had inner mass detected, actually
																											// produced exposed blocks. If the case was that they didn't produce an EXPOSED block, the LANDLOCKED block is put back in, 
																											// producing a blockSkeleton.
																											
																																																					


	void instantiateBlockAndRemoveSkeleton(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_key);		// this function will remove a block from the skeleton map, and replace
																											// the corresponding entry in blockMap with an entirely new, fresh, empty EnclaveBlock.
	void instantiateBlockAndRemoveSkeletonIfNonExistent(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_key);		// works the same as instantiateBlockAndRemoveSkeleton, but only if the EnclaveBlock doesn't exist yet. 

	void insertVectoredBBFansIntoBlock(std::mutex* in_mutexRef,												// --> this function inserts a vector of OrganicWrappedBBFans into an EnclaveBlock.
									   std::vector<OrganicWrappedBBFan> in_fanVector,						// this function assumes that the EnclaveBlock exists.
									   EnclaveKeyDef::EnclaveKey in_key);


	EnclaveTriangleSkeletonSupergroupManager spawnEnclaveTriangleSkeletonContainers();						// reads from the EnclaveTriangleContainer map to produce their corresponding EnclaveTriangleSkeletonContainers; the produced skeleton containers can then be sent (appended) to 
																											// a different instance of OrganicRawEnclave, using the function appendSpawnedEnclaveTriangleSkeletonContainers; see usage in 
																											// OrganicSystem::spawnAndAppendEnclaveTriangleSkeletonsToBlueprint.
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



	// **************************** START DEBUG FUNCTIONS *********************************************

	void simulateBlockProduction();			// debug function; will simulate block production by reading from skeletonSGM, without modifying contents of the ORE.
	EnclaveBlockState getBlockStatus(EnclaveKeyDef::EnclaveKey in_blockKey);
	void printBlockCategorizations();
	Message fetchBDMMessageForSkeletonSGM(EnclaveKeyDef::EnclaveKey in_blueprintKeyForBDM, EnclaveKeyDef::EnclaveKey in_oreKeyForBDM);

	// **************************** END DEBUG FUNCTIONS *********************************************




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
	EnclaveTriangleContainerSupergroupManager etcSGM;
	OrganicTriangleSecondarySupergroupManager organicTriangleSecondarySGM;	// used to store produced OrganicTriangleSecondary instances, in their appropriate supergrpup.
	std::map<int, EnclaveBlock> blockMap;	// this map is built when the OrganicSystem requires a high LOD terrain job, such as RJPhasedBlueprintMM (see OrganicCoreLib)
											// contains individual blocks which can be used to render (can be read from a file as well); otherwise, they are generated from OrganicTriangleSecondaries.

private:

	// Below: data members critical to the functioning of the ORE; these will need to be transformed into relavant Messages (i.e, BDM_ORE_HEADER)
	ORELodState currentLodState = ORELodState::LOD_ENCLAVE_SMATTER;							// the level-of-detail state; always assumed to be LOD_ENCLAVE when initialized, but can be overriden with constructor #2 (see above)
	OREAppendedState currentAppendedState = OREAppendedState::NONE;							// the AppendedState reflects how many different attempts there have been to add EnclaveTriangles to this ORE.
	OREDependencyState currentDependencyState = OREDependencyState::DEPENDENT_ON_PARENTS;	// this state determines whether or not this ORE should be rendered when trying to render the entire blueprint's contents
	std::map<int, EnclaveBlockSkeleton> blockSkeletonMap;	// stores the keys of any blocks considered to be "solid" blocks. (aka, skeletons)
	std::map<int, EnclaveBlockSkeleton> rMassSolidsMap;		// used by OREMatterCollider::extractResultsAndSendtoORE (OrganicCoreLib), to store solid blocks formed during the conversion to ORELodState::LOD_ENCLAVE_RMATTER.
	OperableIntSet existingEnclaveTriangleSkeletonContainerTracker;			// returns a set that represents the unique IDs of EnclaveTriangles; used by OrganicSystem::spawnAndAppendEnclaveTriangleSkeletonsToBlueprint,
																			// in order to distinguish between "old" and "new" sets of an SPolySet.
	int total_triangles = 0;								// the total number of renderable triangles; incremented/decremented according to the number of triangles per BBFan; determined at run time

	void updateCurrentAppendedState();		// updates the appended state to SINGLE_APPEND or MULTIPLE_APPEND
	void resetBlockDataAndTriangleCount();																							// clears the blockMap, and resets triangle count
	Operable3DEnclaveKeySet spawnEnclaveTriangleContainers(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey);			// reads from the skeletonSGM to produce their corresponding EnclaveTriangleContainers;
																																		// The return value contains the keys of any blocks that wwre deemed incalculable
																																		// as a result of a call to EnclaveTriangle::executeRun().

	void createBlocksFromOrganicTriangleSecondaries(std::mutex* in_mutexRef);										// spawn the EnclaveBlocks, and Fans from the OrganicTriangleSecondaries; used when the currentState is
																													// in LOD_ENCLAVE or LOD_ENCLAVE_MODIFIED
	void produceAllUnexposedBlocks(std::mutex* in_mutexRef);		// should be used when going from currentLodState of FULL to BLOCK; this will 
																	// make all 64 possible blocks in the ORE be stored in the blockSkeletonMap. 
																	// Currently used by morphLodToBlock function. Default material for each block is 0.
																	

	void insertOrganicTriangleSecondaryIntoRefedManager(OrganicTriangleSecondarySupergroupManager* in_refedManager,	// inserts OrganicTriangleSecondaries into a refed OrganicTriangleSecondarySupergroupManager;
														int in_polyID,												// needed by the function, simulateBlockProduction(). 
														int in_clusterID, 
														OrganicTriangleSecondary in_enclavePolyFractureResults);

	Message buildOREHeaderBDMMessage(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey);		// builds a BDM_ORE_HEADER Message, that contains metadata about the ORE that is required for it's reconstruction;
																															// the EnclaveKeyDef parameters passed in are required for Message construction 
																															// (the blueprint and blueprint-specific ORE key need to be at the front of the Message)

};

#endif
