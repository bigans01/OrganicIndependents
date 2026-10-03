/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 9/15/2017)

Description: Header file for ECBMap.cpp

Summary: An ECBMap will contain one or more EnclaveCollectionBlueprint in an unordered_map, with the key being a valid EnclaveKey. This class
will be used by the OrganicSystem to determine what to do with a collection of enclaves that has valid key.


------------------------------------------------------------------------------------------*/


#pragma once

#ifndef ENCLAVECOLLECTIONblueprintMatrix_H
#define ENCLAVECOLLECTIONblueprintMatrix_H

#include "EnclaveKeyDef.h"
#include "EnclaveCollectionBlueprint.h"
//#include "EnclaveCollectionBorderFlags.h"
#include <unordered_map>
#include <mutex>
#include "EnclaveFractureResultsMap.h"
#include "OrganicRawEnclave.h"
#include "OrganicRawManifest.h"
#include "BlueprintLockController.h"
#include "EnclaveBlockState.h"

class EnclaveCollectionNeighborList;
class OGLMBufferManager;
class ECBMap
{
public:
	friend OrganicSystem;
	friend OGLMBufferManager;

	std::mutex blueprintMapMutex;
	BlueprintLockController blueprintLocks;

	void printAllKeys();	// print the EnclaveKeyDef key values of all entries in the blueprintMap.

	bool checkIfBlueprintRequiresProcessing(EnclaveKeyDef::EnclaveKey in_key);
	bool checkIfBlueprintRequiresMMProcessing(EnclaveKeyDef::EnclaveKey in_key);
	bool checkIfBlueprintExists(EnclaveKeyDef::EnclaveKey in_key);
	bool checkIfBlueprintContainsOREs(EnclaveKeyDef::EnclaveKey in_key);
	bool checkIfBlueprintHasCatalogEnabled(EnclaveKeyDef::EnclaveKey in_blueprintKey);
	bool checkIfBlueprintContainsSpecificOre(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_enclaveKey);
	bool checkIfSpecificOreContainsBlocks(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_enclaveKey);
	EnclaveBlockState getBlockStateFromEFRM(EnclaveKeyDef::EnclaveKey in_blueprintKey,	// -----> this function will call the getBlockStatus function from the specified ORE,
		EnclaveKeyDef::EnclaveKey in_enclaveKey,	//		if that ORE exists. For any ORE that is not in a currentLodState of ORELodState::LOD_BLOCK,
		EnclaveKeyDef::EnclaveKey in_blockKey);		//		the, getBlockStatus will have to do a "dry run" attempt to construct all available blocks, and this is
	//		is a heavy operation. Using this with the JEV3BlockTargeter::traverseToNextBlock was seen
	//		to have caused major performance issues per frame. As a result, it's call was replaced with
	//		getBlockStateFromPopulatedORE, which assumes the ORE is a "targetable" (that is, spawnRenderableBlocks has been called)

	void printStatsForBlockInORE(EnclaveKeyDef::EnclaveKey in_blueprintKey,		// -----> this function will print out the vertices and triangle fans that exist in an EnclaveBlock,
		EnclaveKeyDef::EnclaveKey in_enclaveKey,		//		  if that block exists, in a specified ORE. Currently executed when a valid "printblockstats" command
		EnclaveKeyDef::EnclaveKey in_blockKey);			//		  is issued via the OpenGL command input.

	EnclaveBlockState getBlockStateFromPopulatedORE(EnclaveKeyDef::EnclaveKey in_blueprintKey,	// -----> this function assumes that the ORE has 
		EnclaveKeyDef::EnclaveKey in_enclaveKey,	//		already called spawnRenderableBlocks; it will return
		EnclaveKeyDef::EnclaveKey in_blockKey);		//		a EnclaveBlockState value of NONEXISTENT otherwise.
	//		it is meant to be used by JEV3BlockTargeter::traverseToNextBlock.

	EnclaveFractureResultsMap* getFractureResultsMapRef(EnclaveKeyDef::EnclaveKey in_key);
	OrganicRawEnclave* getOrganicRawEnclaveRef(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_rawEnclaveKey);
	OrganicRawManifest* getOrganicRawManifestRef(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_rawEnclaveKey);
	EnclaveCollectionBlueprint* getBlueprintRef(EnclaveKeyDef::EnclaveKey in_blueprintKey);
	void addBlueprintViaCopy(EnclaveKeyDef::EnclaveKey in_key, EnclaveCollectionBlueprint in_blueprint);
	void addPolyToBlueprint(EnclaveKeyDef::EnclaveKey in_key, int in_newPolyID, ECBPoly in_polyToAdd);
	void deletePolyFromBlueprint(EnclaveKeyDef::EnclaveKey in_key, int in_polyIDToDelete);
	void rebuildBlueprintCatalog(EnclaveKeyDef::EnclaveKey in_key);
	void addBlueprintViaRef(EnclaveKeyDef::EnclaveKey in_key, EnclaveCollectionBlueprint* in_blueprint);
	void setBlueprintRunMode(EnclaveKeyDef::EnclaveKey in_key, int in_runMode);
	void setBlueprintState(EnclaveKeyDef::EnclaveKey in_key, BlueprintState in_blueprintStateToSet);	// sets the state of a blueprint, if that blueprint exists.
	void insertBlueprintIfNonExistent(EnclaveKeyDef::EnclaveKey in_key);
	void clearAllBlueprints();
	std::set<int> getMorphLodToBlockAffectedForBlueprint(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey);

	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator getSpecificBlueprintIter(EnclaveKeyDef::EnclaveKey in_blueprintToFind);
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator getBlueprintBeginIter();
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>::iterator getBlueprintEndIter();

private:
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher> blueprintMap;
};

#endif
