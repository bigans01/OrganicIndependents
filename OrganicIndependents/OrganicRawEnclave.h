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
#include "OrganicRawEnclaveState.h"
#include "OrganicTransformUtils.h"
#include "EnclaveBlockSkeleton.h"
#include <mutex>
#include "OperableIntSet.h"
#include "EnclaveBlock.h"
#include "OrganicTriangleSecondarySupergroupManager.h"
#include "EnclaveTriangleContainerSupergroupManager.h"
#include "EnclaveTriangleSkeletonSupergroupManager.h"

class OrganicRawEnclave
{
public:

	OrganicRawEnclave();
	OrganicRawEnclave(OrganicRawEnclaveState in_organicRawEnclavestate);

	OrganicRawEnclave& operator=(const OrganicRawEnclave& resultsContainer_b)
	{
		currentState = resultsContainer_b.currentState;
		blockSkeletonMap = resultsContainer_b.blockSkeletonMap;
		skeletonSGM = resultsContainer_b.skeletonSGM;									// copy skeleton container map
		//finalizerBlocks = resultsContainer_b.finalizerBlocks;
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

	OrganicRawEnclaveState currentState = OrganicRawEnclaveState::LOD_ENCLAVE;	// the state; always assumed to be LOD_ENCLAVE when initialized, but can be overriden with constructor #2 (see above)

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
	std::map<int, EnclaveBlockSkeleton> blockSkeletonMap;
	std::map<int, EnclaveBlock> finalizerBlocks;							// unused for now	

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
	// this map is built when the OrganicSystem requires a high LOD terrain job, such as RJPhasedBlueprintMM (see OrganicCoreLib)
	std::map<int, EnclaveBlock> blockMap;					// contains individual blocks which can be used to render (can be read from a file as well); otherwise, they are generated from OrganicTriangleSecondaries.
	int total_triangles = 0;								// the total number of renderable triangles; incremented/decremented according to the number of triangles per BBFan; determined at run time

	OperableIntSet existingEnclaveTriangleSkeletonContainerTracker;


	void insertOrganicTriangleSecondary(int in_polyID, int in_clusterID, OrganicTriangleSecondary in_enclavePolyFratureResults);
	void insertEnclaveTriangleContainer(int in_polyID, int in_clusterID, EnclaveTriangleContainer in_enclaveTriangleContainer);
	void insertBlockSkeleton(EnclaveKeyDef::EnclaveKey in_blockKey, EnclaveBlockSkeleton in_blockSkeleton);
	void createBlocksFromOrganicTriangleSecondaries(std::mutex* in_mutexRef);										// spawn the EnclaveBlocks, and BBFans from the OrganicTriangleSecondaries
	std::set<int> getTouchedBlockList();																			// retrieves a list of blocks that were "touched" by the OrganicTriangleSecondaries; requires 
																													// OrganicTriangleSecondaries to be present.			
	void printMapData();
	void printEnclaveTriangleContainers();
	bool checkIfFull();
	int getNumberOfBlockSkeletons();
	void resetBlockData();																							// clears the blockMap, and resets triangle count
	EnclaveBlock* getBlockRefViaBlockKey(EnclaveKeyDef::EnclaveKey in_key);
	EnclaveTriangleSkeletonSupergroupManager spawnEnclaveTriangleSkeletonContainers();						// reads from the EnclaveTriangleContainer map to produce their corresponding EnclaveTriangleSkeletonContainers; the produced skeleton containers can then be sent (appended) to 
																											// a different instance of OrganicRawEnclave, using the function appendSpawnedEnclaveTriangleSkeletonContainers; see usage in 
																											// spawnAndAppendEnclaveTriangleSkeletonsToBlueprint.
	void appendSpawnedEnclaveTriangleSkeletonContainers(std::mutex* in_mutexRef, EnclaveTriangleSkeletonSupergroupManager in_enclaveTriangleSkeletonContainer);
	void spawnEnclaveTriangleContainers(std::mutex* in_mutexRef, EnclaveKeyDef::EnclaveKey in_enclaveKey);			// reads from the skeletonSGM to produce their corresponding EnclaveTriangleContainers
	OperableIntSet getExistingEnclaveTriangleSkeletonContainerTracker();

};

#endif
