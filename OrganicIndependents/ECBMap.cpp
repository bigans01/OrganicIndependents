#include "stdafx.h"
#include <stdio.h>
#include <chrono>
#include "ECBMap.h"
//#include "EnclaveCollectionNeighborList.h"

bool ECBMap::checkIfBlueprintRequiresProcessing(EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	return blueprintMap[in_key].doesBlueprintRequireProcessing();
}

void ECBMap::printAllKeys()
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	for (auto& currentKey : blueprintMap)
	{
		std::cout << "Key: ";
		EnclaveKeyDef::EnclaveKey keyCopy = currentKey.first;
		keyCopy.printKey();
		std::cout << std::endl;
	}
}

bool ECBMap::checkIfBlueprintRequiresMMProcessing(EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	return blueprintMap[in_key].doesBlueprintRequireMMProcessing();
}

bool ECBMap::checkIfBlueprintExists(EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	bool wasFound = false;	// assume false, unless found
	auto findBlueprint = blueprintMap.find(in_key);
	if (findBlueprint != blueprintMap.end())	// it was found.
	{
		wasFound = true;
	}
	return wasFound;
}

bool ECBMap::checkIfBlueprintContainsOREs(EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	bool wasFound = false;	// assume false, unless found
	auto findBlueprint = blueprintMap.find(in_key);
	if (findBlueprint != blueprintMap.end())	// it was found.
	{
		wasFound = findBlueprint->second.fractureResults.checkIfAnyOREsExist();	// returns true if any exist.
	}
	return wasFound;
}

bool ECBMap::checkIfBlueprintHasCatalogEnabled(EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	bool isCatalogEnabled = false;
	auto findBlueprint = blueprintMap.find(in_blueprintKey);
	if (findBlueprint != blueprintMap.end())
	{
		isCatalogEnabled = findBlueprint->second.isCatalogEnabled();
	}
	return isCatalogEnabled;
}

bool ECBMap::checkIfBlueprintContainsSpecificOre(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	bool wasFound = false;
	auto findBlueprint = blueprintMap.find(in_blueprintKey);
	if (findBlueprint != blueprintMap.end())
	{
		wasFound = findBlueprint->second.fractureResults.checkIfSpecificOREExists(in_enclaveKey);
	}
	return wasFound;
}

bool ECBMap::checkIfSpecificOreContainsBlocks(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	bool wasFound = false;
	auto findBlueprint = blueprintMap.find(in_blueprintKey);
	if (findBlueprint != blueprintMap.end())
	{
		wasFound = findBlueprint->second.fractureResults.checkIfSpecificOREContainsAnyBlocks(in_enclaveKey);
	}
	return wasFound;
}

EnclaveBlockState ECBMap::getBlockStateFromEFRM(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	EnclaveBlockState returnState = EnclaveBlockState::NONEXISTENT;
	auto findBlueprint = blueprintMap.find(in_blueprintKey);
	if (findBlueprint != blueprintMap.end())
	{
		returnState = findBlueprint->second.fractureResults.getBlockStateFromORE(in_enclaveKey, in_blockKey);
	}
	return returnState;
}

void ECBMap::printStatsForBlockInORE(EnclaveKeyDef::EnclaveKey in_blueprintKey,
	EnclaveKeyDef::EnclaveKey in_enclaveKey,
	EnclaveKeyDef::EnclaveKey in_blockKey)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	auto findBlueprint = blueprintMap.find(in_blueprintKey);
	if (findBlueprint != blueprintMap.end())
	{
		// check that the ORE exists.
		auto findSpecificORE = findBlueprint->second.fractureResults.fractureResultsContainerMap.find(in_enclaveKey);
		if (findSpecificORE != findBlueprint->second.fractureResults.fractureResultsContainerMap.end())
		{
			findSpecificORE->second.printBlockData(in_blockKey);
		}
	}
}


EnclaveBlockState ECBMap::getBlockStateFromPopulatedORE(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	// this function will only return EXPOSED/UNEXPOSED on blocks in an ORE that has already called spawnRenderableBlocks, and loaded its data for targeting.
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	EnclaveBlockState returnState = EnclaveBlockState::NONEXISTENT;
	int convertedBlockCoords = PolyUtils::convertBlockCoordsToSingle(in_blockKey.x, in_blockKey.y, in_blockKey.z);

	// check that the blueprint exists.
	auto findBlueprint = blueprintMap.find(in_blueprintKey);
	if (findBlueprint != blueprintMap.end())
	{
		// check that the ORE exists.
		auto findSpecificORE = findBlueprint->second.fractureResults.fractureResultsContainerMap.find(in_enclaveKey);
		if (findSpecificORE != findBlueprint->second.fractureResults.fractureResultsContainerMap.end())
		{
			// check blockmap (exposed).
			auto findBlock = findSpecificORE->second.blockMap.find(convertedBlockCoords);
			if (findBlock != findSpecificORE->second.blockMap.end())
			{
				returnState = EnclaveBlockState::EXPOSED;
			}
			// check if its in the skeleton map (unexposed).
			else if (findSpecificORE->second.doesBlockSkeletonExistNoMutex(in_blockKey) == true)
			{
				returnState = EnclaveBlockState::UNEXPOSED;
			}
		}
	}
	return returnState;
}

EnclaveFractureResultsMap* ECBMap::getFractureResultsMapRef(EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	return &blueprintMap[in_key].fractureResults;
}

OrganicRawEnclave* ECBMap::getOrganicRawEnclaveRef(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_rawEnclaveKey)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	return &blueprintMap[in_blueprintKey].fractureResults.fractureResultsContainerMap[in_rawEnclaveKey];
}

OrganicRawManifest* ECBMap::getOrganicRawManifestRef(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_rawEnclaveKey)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	return &blueprintMap[in_blueprintKey].fractureResults.rawManifestMap[in_rawEnclaveKey];
}

EnclaveCollectionBlueprint* ECBMap::getBlueprintRef(EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	return &blueprintMap[in_blueprintKey];
}

void ECBMap::addBlueprintViaCopy(EnclaveKeyDef::EnclaveKey in_key, EnclaveCollectionBlueprint in_blueprint)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	blueprintMap[in_key] = in_blueprint;
}

void ECBMap::addPolyToBlueprint(EnclaveKeyDef::EnclaveKey in_key, int in_newPolyID, ECBPoly in_polyToAdd)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	auto isCatalogingEnabledOnTarget = blueprintMap[in_key].isCatalogEnabled();
	if (isCatalogingEnabledOnTarget == false)
	{
		blueprintMap[in_key].insertPolyWithKeyValue(in_newPolyID, in_polyToAdd);
	}
	else if (isCatalogingEnabledOnTarget == true)
	{
		blueprintMap[in_key].insertPolyWithKeyValueAndCatalogMapping(in_newPolyID, in_polyToAdd, in_key);
	}
}

void ECBMap::deletePolyFromBlueprint(EnclaveKeyDef::EnclaveKey in_key, int in_polyIDToDelete)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	auto isCatalogingEnabledOnTarget = blueprintMap[in_key].isCatalogEnabled();
	if (isCatalogingEnabledOnTarget == false)
	{
		blueprintMap[in_key].deletePoly(in_polyIDToDelete);
	}
	else if (isCatalogingEnabledOnTarget == true)
	{
		blueprintMap[in_key].deletePolyAndCatalogMapping(in_polyIDToDelete);
	}
}

std::set<int> ECBMap::getMorphLodToBlockAffectedForBlueprint(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	return blueprintMap[in_blueprintKey].fetchMorphLodToBlockAffectedOrganicTriangles(in_oreKey);
}

void ECBMap::rebuildBlueprintCatalog(EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	blueprintMap[in_key].enableAndRebuildOrganicTriangleCatalog(in_key);
}

void ECBMap::addBlueprintViaRef(EnclaveKeyDef::EnclaveKey in_key, EnclaveCollectionBlueprint* in_blueprint)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	blueprintMap[in_key] = *in_blueprint;
}

void ECBMap::setBlueprintRunMode(EnclaveKeyDef::EnclaveKey in_key, int in_runMode)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	//blueprintMap[in_key].bpRunMode = in_runMode;
}

void ECBMap::setBlueprintState(EnclaveKeyDef::EnclaveKey in_key, BlueprintState in_blueprintStateToSet)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	auto existingIter = blueprintMap.find(in_key);
	if (existingIter != blueprintMap.end())
	{
		blueprintMap[in_key].bpTracker.setSyncState(in_blueprintStateToSet);
	}
}

void ECBMap::insertBlueprintIfNonExistent(EnclaveKeyDef::EnclaveKey in_key)
{
	std::lock_guard<std::mutex> lock(blueprintMapMutex);	// lock for safety.
	auto existingIter = blueprintMap.find(in_key);
	if (existingIter != blueprintMap.end())
	{
		EnclaveCollectionBlueprint blankBlueprint;
		blueprintMap[in_key] = blankBlueprint;
	}
}

void ECBMap::clearAllBlueprints()
{
	blueprintMap.clear();
}

std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator ECBMap::getSpecificBlueprintIter(EnclaveKeyDef::EnclaveKey in_blueprintToFind)
{
	return blueprintMap.find(in_blueprintToFind);
}

std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator ECBMap::getBlueprintBeginIter()
{
	return blueprintMap.begin();
}

std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator ECBMap::getBlueprintEndIter()
{
	return blueprintMap.end();
}

