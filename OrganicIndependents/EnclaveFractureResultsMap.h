#pragma once

#ifndef ENCLAVEFRACTURERESULTSMAP_H
#define ENCLAVEFRACTURERESULTSMAP_H

#include <unordered_map>
#include "EnclaveKeyDef.h"
#include "OrganicRawEnclave.h"
#include "OrganicRawManifest.h"
#include "OrganicTriangleSecondary.h"
#include "OrganicTriangle.h"
#include "EnclaveTriangleContainer.h"
#include <mutex>
#include "MassDriverLimitArray.h"
#include "EnclaveBlockState.h"

class EnclaveFractureResultsMap
{
public:
	//std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclavePoly, EnclaveKeyDef::KeyHasher>* in_enclavePolyMapRef,
	//OrganicRawEnclave& operator=(const OrganicRawEnclave& resultsContainer_b)

	EnclaveFractureResultsMap& operator=(EnclaveFractureResultsMap&& resultsMap_b)
	{
		// clear out this instance's fractureResultsContainerMap.
		fractureResultsContainerMap.clear();

		// must manually cycle through each element in the to-be copied container.
		if (!resultsMap_b.fractureResultsContainerMap.empty())
		{
			auto bContainerBegin = resultsMap_b.fractureResultsContainerMap.begin();
			auto bContainerEnd = resultsMap_b.fractureResultsContainerMap.end();
			for (bContainerBegin; bContainerBegin != bContainerEnd; bContainerBegin++)
			{
				fractureResultsContainerMap[bContainerBegin->first] = bContainerBegin->second;
			}

		}
		return *this;
	}


	EnclaveFractureResultsMap& operator=(const EnclaveFractureResultsMap& resultsMap_b)
	{
		//fractureResultsContainerMap = resultsMap_b.fractureResultsContainerMap;		// cannot be used, will cause error. 
		// clear out this instance's fractureResultsContainerMap.
		fractureResultsContainerMap.clear();

		// must manually cycle through each element in the to-be copied container.
		if (!resultsMap_b.fractureResultsContainerMap.empty())
		{
			auto bContainerBegin = resultsMap_b.fractureResultsContainerMap.begin();
			auto bContainerEnd = resultsMap_b.fractureResultsContainerMap.end();
			for (bContainerBegin; bContainerBegin != bContainerEnd; bContainerBegin++)
			{
				fractureResultsContainerMap[bContainerBegin->first] = bContainerBegin->second;
			}

		}
		return *this;
	}
	std::unordered_map<EnclaveKeyDef::EnclaveKey, OrganicRawEnclave, EnclaveKeyDef::KeyHasher> fractureResultsContainerMap;
	std::unordered_map<EnclaveKeyDef::EnclaveKey, OrganicRawManifest, EnclaveKeyDef::KeyHasher> rawManifestMap;
	void transferEnclaveTrianglesIntoOREs(int in_polygonID, OrganicTriangle* in_organicTriangleRef, std::mutex* in_mutexRef);
	void insertFractureResults(EnclaveKeyDef::EnclaveKey in_enclaveKey,
		int in_polygonId, OrganicTriangleSecondary in_container,
		EnclaveTriangleContainer* in_enclaveTriangleContainer,
		std::mutex* in_mutexRef);


	void insertNewRawManifest(EnclaveKeyDef::EnclaveKey in_enclaveKey, std::mutex* in_mutexRef);
	MassDriverLimitArray produceMassDriverLimitArray();
	bool checkIfAnyOREsExist();
	bool checkIfSpecificOREExists(EnclaveKeyDef::EnclaveKey in_enclaveKey);
	bool checkIfSpecificOREContainsAnyBlocks(EnclaveKeyDef::EnclaveKey in_enclaveKey);
	Message submitBDMForORESGM( EnclaveKeyDef::EnclaveKey in_blueprintKeyForRequest,	// attempts to produce an BDM_ORE_SKELETONSGM Message, if
								EnclaveKeyDef::EnclaveKey in_enclaveKeyForRequest);		// the ORE exists. If the ORE doesn't exist, a blank Message is returned;
																						// So make sure that the ORE exists before calling this.

	EnclaveBlockState getBlockStateFromORE(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey);
};

#endif
