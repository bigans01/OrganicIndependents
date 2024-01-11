/*------------------------------------------------------------------------------------------

--OrganicSystem.h		(Last update 9/15/2017)

Description: Header file for EnclaveCollectionBlueprint.cpp

Summary: An EnclaveCollectionBlueprint will store metadata that will tell the Organic system how to "shape" a collection of enclaves. It will include information on which chunks in the collection are solid, which solids are "exposed to the surface (air)",
and has the ability to store EnclavePainters, which have the ability to change the block id of a series of blocks in a chunk.


------------------------------------------------------------------------------------------*/

#pragma once

#ifndef ENCLAVECOLLECTIONBLUEPRINT_H
#define ENCLAVECOLLECTIONBLUEPRINT_H

#include "ECBPoly.h"
#include "CursorPathTraceContainer.h"
#include "EnclaveFractureResultsMap.h"
#include "PolyGroupRange.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "OperableIntSet.h"
#include "OrganicTriangleCatalog.h"
#include "BlueprintSyncTracker.h"
#include "HashUtils.h"

class EnclaveCollectionBlueprint {
public:
	EnclaveCollectionBlueprint();
	EnclaveCollectionBlueprint(const EnclaveCollectionBlueprint& blueprint_orig);


	EnclaveCollectionBlueprint& operator=(EnclaveCollectionBlueprint&& blueprint_a)
	{
		// check for OrganicTriangleCatalog, if it's enabled.
		if (blueprint_a.isOrganicTriangleCatalogEnabled == true)
		{
			isOrganicTriangleCatalogEnabled = blueprint_a.isOrganicTriangleCatalogEnabled;
			otCatalog = blueprint_a.otCatalog;
		}

		bpTracker = blueprint_a.bpTracker;
		primaryPolygonMap = blueprint_a.primaryPolygonMap;
		polyGroupRangeMap = blueprint_a.polyGroupRangeMap;
		fractureResults = blueprint_a.fractureResults;

		return *this;
	}

	EnclaveCollectionBlueprint& operator=(const EnclaveCollectionBlueprint& blueprint_a)
	{
		// check for OrganicTriangleCatalog, if it's enabled.
		if (blueprint_a.isOrganicTriangleCatalogEnabled == true)
		{
			isOrganicTriangleCatalogEnabled = blueprint_a.isOrganicTriangleCatalogEnabled;
			otCatalog = blueprint_a.otCatalog;
		}

		bpTracker = blueprint_a.bpTracker;
		primaryPolygonMap = blueprint_a.primaryPolygonMap;
		polyGroupRangeMap = blueprint_a.polyGroupRangeMap;
		fractureResults = blueprint_a.fractureResults;

		return *this;
	}

	void copyExistingECBPolysToOtherBlueprint(OperableIntSet in_polyIDsToTransfer, EnclaveCollectionBlueprint* in_otherBlueprintRef);	// transfers ECBPolys ONLY!
	void copyFractureResultsMapToOtherBlueprint(EnclaveCollectionBlueprint* in_destinationBlueprintRef);
	int fetchNextECBPolyKeyID();
	std::set<int> fetchNextIDs(int in_numberOfIdsToFetch);
	bool doesECBPolyWithKeyAlreadyExist(int in_keyValue);
	void enableAndRebuildOrganicTriangleCatalog(EnclaveKeyDef::EnclaveKey in_blueprintKey);
	bool isCatalogEnabled();
	std::set<int> fetchMorphLodToBlockAffectedOrganicTriangles(EnclaveKeyDef::EnclaveKey in_oreKey);

	OperableIntSet produceECBPolyIDSet();
	std::map<int, ECBPoly>::iterator getPolygonMapBeginIter();				// fetches the begin iterator for primaryPolygonMap.
	std::map<int, ECBPoly>::iterator getPolyIterFromMap(int in_keyValue);	// returns an iterator specific keyed ECBPoly.
	std::map<int, ECBPoly>::iterator getPolygonMapEndIter();				// fetches the end iterator for primaryPolygonMap.
	void insertPolyWithKeyValue(int in_keyValue, ECBPoly in_ecbPoly);		// inserts an ECBPoly at a specific key.

	bool insertECBPolyLineIntoPoly(int in_keyValue, int in_lineID, ECBPolyLine in_ecbPolyLine);	// inserts an ECBPolyLine having a specific lineID, into the specified keyed ECBPoly;
																								// the ECBPoly must already exit for this to work.

	int getPolygonMapSize();			// returns the size of primaryPolygonMap.
	void deletePoly(int in_keyValue);	// deletes an ECBPoly at the specified key, in primaryPolygonMap.

	void insertPolyWithKeyValueAndCatalogMapping(int in_keyValue, ECBPoly in_ecbPoly, EnclaveKeyDef::EnclaveKey in_blueprintKey);
	void deletePolyAndCatalogMapping(int in_keyValue);

	// debug/diag functions; not critical to application but handy for development/debug.
	void printBDMForORESkeletonSGM(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey);		// attempts to query an ORE, if it exists, to
																														// produce a BDM_ORE_SKELETONSGM Message to parse, from that ORE's oreRTHandler.
																														// This function just prints out the contents in a readable manner, for testing purposes.

	MessageContainer convertBlueprintTOBDMFormat(EnclaveKeyDef::EnclaveKey in_blueprintKey);	// generate all data necessary to make an instance of this class, in BDM format.

	std::string getBlueprintHash();	// analyzes the contents of the blueprint to produce a SHA256 for integrity/verification purposes.

	std::map<int, PolyGroupRange> polyGroupRangeMap;
	EnclaveFractureResultsMap fractureResults;
	BlueprintSyncTracker bpTracker;

private:
	friend class EnclaveCollection;
	friend class OrganicSystem;
	friend class Enclave;
	friend class EnclaveCollectionMap;
	friend class ECBMap;

	OrganicTriangleCatalog otCatalog;
	int currentPolyGroup = 0;
	bool requiresProcessing = true;								// does the blueprint need to be run through the engine? the default is always true, so that new blueprints are run through terrain processing. changed to false when a collection has been processed. 
	bool requiresMMProcessing = true;							// does the blueprint need to be run through the MM pipeline?
	bool isOrganicTriangleCatalogEnabled = false;				// determines whether or not the OrganicTriangleCatalog is active in this blueprint.

	std::map<int, ECBPoly> primaryPolygonMap;			// causes strange false positive in MSVC??

	bool doesBlueprintRequireProcessing();
	bool doesBlueprintRequireMMProcessing();
	int createPolyGroupRangeAndIncrement(PolyGroupRange in_polyGroupRange);		// create the poly group, return its group ID.

	Message createBDMBlueprintHeaderMessage(EnclaveKeyDef::EnclaveKey in_blueprintKey);	// creates a Message of the type MessageType::BDM_BLUEPRINT_HEADER,
																						// that contains necessary metadata of the EnclaveCollectionBlueprint, that 
																						// can be checked against when reconstituting a blueprint to make sure it is rebuilt correctly.
																						// Currently, a Message of this type includes:
																						//	-the key of the blueprint that this Message will be created for
																						//	-the number of ECBPolys in the primaryPolygonMap
																						//	-the total number of OREs in the blueprint

};

#endif