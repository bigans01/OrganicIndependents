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
#include "IndependentUtils.h"

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

		// Boost serialization
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			// writing operations
			if (Archive::is_saving::value)
			{
				std::cout << "########### Starting write out of EnclaveFractureResultsMap. ##########" << std::endl;

				// Step 1: Find all OREs in the fractureResultsContainerMap that have a state that is not SMART_FULL.
				// SMART_FULL OREs will go into their own list later.
				std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> nonSmartFullORElist;
				std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> smartFullORElist;

				for (auto& currentORE : fractureResultsContainerMap)
				{
					ORELodState currentOREState = currentORE.second.getLodState();
					if (currentOREState != ORELodState::SMART_FULL)
					{
						nonSmartFullORElist.insert(currentORE.first);
					}
					else
					{
						smartFullORElist.insert(currentORE.first);
					}
				}

				// Step 2: Write the number of nonSmartFullORElist OREs; then cycle 
				// through each member of nonSmartFullORElist and insert the ORE's key, followed by the ORE itself, 
				// for each ORE.
				int numberOfNonSmartFulls = nonSmartFullORElist.size();
				ar& numberOfNonSmartFulls;
				for (auto& currentNonSmartFullORE : nonSmartFullORElist)
				{
					EnclaveKeyDef::EnclaveKey tempNonSmartFullOREKey = currentNonSmartFullORE;
					std::cout << ">>> Writing out key for non-smart full ORE: ";
					tempNonSmartFullOREKey.printKey();
					std::cout << std::endl;

					int convertedOREKey = IndependentUtils::convertOREKeyCoordsToSingle(currentNonSmartFullORE);
					ar& convertedOREKey;
					ar& fractureResultsContainerMap[currentNonSmartFullORE];
				}

				// Step 3: Write the number of SMART_FULL OREs, then cycle through each SMART_FULL ORE
				// and write out it's key. We do not need to write out the SMART_FULL data, as it should effectively
				// contain nothing.
				int numberOfSmartFulls = smartFullORElist.size();
				ar& numberOfSmartFulls;
				for (auto& currentSmartFullORE : smartFullORElist)
				{
					EnclaveKeyDef::EnclaveKey tempSmartFullOREKey = currentSmartFullORE;
					std::cout << ">>> Writing out key for smart full ORE: ";
					tempSmartFullOREKey.printKey();
					std::cout << std::endl;


					int convertedSmartFULLOreKey = IndependentUtils::convertOREKeyCoordsToSingle(currentSmartFullORE);
					ar& convertedSmartFULLOreKey;
				}

				// Step 4: Write out each ECBPoly ID, and then the ECBPoly itself. 

				std::cout << "########### Ended write out of EnclaveFractureResultsMap. ##########" << std::endl;

			}

			// reading operations
			else
			{
				std::cout << "########### Starting read in of EnclaveFractureResultsMap. ##########" << std::endl;

				// Step 1: the first set of ORE data, should be the non-SMART_FULL OREs. The first value should be the
				// number of these to read. For each entry, the first value to read should be the ORE's single value key. Read this
				// value in, convert it to a key, then read in the ORE. Once both are read-in, store them in fractureResultsContainerMap.
				int numberOfNonSmartFullOREs = 0;
				ar& numberOfNonSmartFullOREs;
				for (int x = 0; x < numberOfNonSmartFullOREs; x++)
				{
					// Read the single value key.
					int singleValueOREKey = 0;
					ar& singleValueOREKey;
					EnclaveKeyDef::EnclaveKey convertedKey = IndependentUtils::convertSingleToOREKeyCoords(singleValueOREKey);

					std::cout << ">>> Found key to read in, for non-SMART full ore: ";
					convertedKey.printKey(); 
					std::cout << std::endl;

					// Read in the ORE.
					OrganicRawEnclave newORE;
					ar& newORE;

					// Store the ORE in fractureResultsContainerMap.
					fractureResultsContainerMap[convertedKey] = newORE;
				}

				// Step 2: the next set will be the SMART_FULL OREs. Read in the number of SMART_FULLs, convert the key.
				int numberOfSmartFullOREs = 0;
				ar& numberOfSmartFullOREs;
				for (int x = 0; x < numberOfSmartFullOREs; x++)
				{
					int currentSmartFULLOreSingleValue = 0;
					ar& currentSmartFULLOreSingleValue;

					EnclaveKeyDef::EnclaveKey convertedSmartFullKey = IndependentUtils::convertSingleToOREKeyCoords(currentSmartFULLOreSingleValue);

					std::cout << ">>> Found key to read in, for SMART full ore: ";
					convertedSmartFullKey.printKey();
					std::cout << std::endl;


					OrganicRawEnclave smartFullORE(ORELodState::SMART_FULL);

					fractureResultsContainerMap[convertedSmartFullKey] = smartFullORE;
				}

				// Step 3: Read-in each ECBPoly ID, and then the ECBPoly itself.

				std::cout << "########### Ended read in of EnclaveFractureResultsMap. ##########" << std::endl;
			}
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
		MassDriverLimitArray produceMassDriverLimitArrayDebug(EnclaveKeyDef::EnclaveKey in_debugOreKey);

		bool checkIfAnyOREsExist();
		bool checkIfSpecificOREExists(EnclaveKeyDef::EnclaveKey in_enclaveKey);
		bool checkIfSpecificOREContainsAnyBlocks(EnclaveKeyDef::EnclaveKey in_enclaveKey);
		Message fetchBDMForORERTHandler( EnclaveKeyDef::EnclaveKey in_blueprintKeyForRequest,	// attempts to produce an BDM_ORE_SKELETONSGM Message, if
									EnclaveKeyDef::EnclaveKey in_enclaveKeyForRequest);		// the ORE exists. If the ORE doesn't exist, a blank Message is returned;
																							// So make sure that the ORE exists before calling this.

		EnclaveBlockState getBlockStateFromORE(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey);
};

#endif
