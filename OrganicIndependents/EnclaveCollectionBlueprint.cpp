#include "stdafx.h"
#include <stdio.h>
#include <chrono>
#include "EnclaveCollectionBlueprint.h"

EnclaveCollectionBlueprint::EnclaveCollectionBlueprint()
{

}

EnclaveCollectionBlueprint::EnclaveCollectionBlueprint(const EnclaveCollectionBlueprint& blueprint_a)
{
	primaryPolygonMap = blueprint_a.primaryPolygonMap;
	bpTracker = blueprint_a.bpTracker;
}

MessageContainer EnclaveCollectionBlueprint::convertBlueprintTOBDMFormat(EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	MessageContainer blueprintConvertedContainer;

	// The returning MessageContainer should have the following:
	//
	// 1 blueprint header message (BDM_BLUEPRINT_HEADER)
	// 1 message for EACH ECB_POLY.
	// The appended contents of each MessageContainer that is returned by the call to OrganicRawEnclave::convertOREToBDMFormat for each ORE in fractureResults

	// Step 1: construct and insert the BDM_BLUEPRINT HEADER.
	//	...code for building this is TBD

	// Step 2: construct and insert Message for each ECB_POLY.
	//	...code for building this is TBD

	// Step 3: append the contents of each MessageContainer returned by the call to OrganicRawEnclave::convertOREToBDMFormat;
	//			The returned MessageContainer will contain the following MessageTypes:
	//			-BDM_ORE_HEADER
	//			-BDM_ORE_SKELETONSGM
	//			-BDM_BLOCK_TAGGED
	//			-BDM_SKELETONBLOCK_TAGGED
	for (auto& currentOREToConvert : fractureResults.fractureResultsContainerMap)
	{
		MessageContainer currentOREData = currentOREToConvert.second.convertOREToBDMFormat(in_blueprintKey, currentOREToConvert.first);
		blueprintConvertedContainer += currentOREData;
	}

	return blueprintConvertedContainer;
}

OperableIntSet EnclaveCollectionBlueprint::produceECBPolyIDSet()
{
	OperableIntSet returnSet;
	auto polyIDsBegin = primaryPolygonMap.begin();
	auto polyIDsEnd = primaryPolygonMap.end();
	for (; polyIDsBegin != polyIDsEnd; polyIDsBegin++)
	{
		returnSet.intSet.insert(polyIDsBegin->first);
	}
	return returnSet;
}

std::map<int, ECBPoly>::iterator EnclaveCollectionBlueprint::getPolygonMapBeginIter()
{
	return primaryPolygonMap.begin();
}

std::map<int, ECBPoly>::iterator EnclaveCollectionBlueprint::getPolyIterFromMap(int in_keyValue)
{
	return primaryPolygonMap.find(in_keyValue);
}

std::map<int, ECBPoly>::iterator EnclaveCollectionBlueprint::getPolygonMapEndIter()
{
	return primaryPolygonMap.end();
}

void EnclaveCollectionBlueprint::insertPolyWithKeyValue(int in_keyValue, ECBPoly in_ecbPoly)
{
	primaryPolygonMap[in_keyValue] = in_ecbPoly;
}


bool EnclaveCollectionBlueprint::insertECBPolyLineIntoPoly(int in_keyValue, int in_lineID, ECBPolyLine in_ecbPolyLine)
{
	bool wasInserted = false;
	auto doesPolyExist = primaryPolygonMap.find(in_keyValue);
	if (doesPolyExist != primaryPolygonMap.end())
	{
		wasInserted = true;
		primaryPolygonMap[in_keyValue].lineMap[in_lineID] = in_ecbPolyLine;
	}
	return wasInserted;
}


void EnclaveCollectionBlueprint::deletePoly(int in_keyValue)
{
	primaryPolygonMap.erase(in_keyValue);
}

void EnclaveCollectionBlueprint::insertPolyWithKeyValueAndCatalogMapping(int in_keyValue, ECBPoly in_ecbPoly, EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	primaryPolygonMap[in_keyValue] = in_ecbPoly;
	if (isOrganicTriangleCatalogEnabled == true)
	{
		otCatalog.insertOrganicTriangleIntoCatalog(&primaryPolygonMap, in_blueprintKey, in_keyValue);
	}
}

void EnclaveCollectionBlueprint::deletePolyAndCatalogMapping(int in_keyValue)
{
	primaryPolygonMap.erase(in_keyValue);
	if (isOrganicTriangleCatalogEnabled == true)
	{
		otCatalog.deleteOrganicTriangleFromCatalog(in_keyValue);
	}
}

void EnclaveCollectionBlueprint::printBDMForORESkeletonSGM(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey)
{
	// Safety: only attempt to fetch the message if the ORE actually exists.
	if (fractureResults.checkIfSpecificOREExists(in_oreKey))
	{
		// The below Message that gets returned from the function call comes from the EnclaveTriangleSkeletonSupergroupManager::convertSkeletonSGMToBDM
		// function. See that function for how a Message of the type BDM_ORE_SKELETONSGM is built (that is the type of Message that is returned here)
		Message generatedMessage = fractureResults.submitBDMForORESGM(in_blueprintKey, in_oreKey);

		std::cout << ">>>>>>>>>>> Beginning test of read of BDM SGM message. " << std::endl;
		generatedMessage.open();

		EnclaveKeyDef::EnclaveKey readBPKey = generatedMessage.readEnclaveKey();
		EnclaveKeyDef::EnclaveKey readOreKey = generatedMessage.readEnclaveKey();
		int totalSkeletons = generatedMessage.readInt();

		std::cout << "BP key: ";
		readBPKey.printKey();
		std::cout << std::endl;

		std::cout << "ORE key: ";
		readOreKey.printKey();
		std::cout << std::endl;

		std::cout << "Number of skeletons: " << totalSkeletons << std::endl;

		std::cout << "Printing out skeleton stats: " << std::endl;
		for (int x = 0; x < totalSkeletons; x++)
		{
			// read out the indices. 
			int supergroupID = generatedMessage.readInt();
			int containerID = generatedMessage.readInt();
			int skeletonID = generatedMessage.readInt();
			std::cout << "[" << supergroupID << "][" << containerID << "][" << skeletonID << "] ||| Points: ";


			// next: read 3 points.
			for (int y = 0; y < 3; y++)
			{
				ECBPolyPoint currentPoint = generatedMessage.readPoint();
				currentPoint.printPointCoords();
				std::cout << " | ";
			}

			// next: read the materialID.
			int currentMaterialID = generatedMessage.readInt();
			std::cout << "Material ID: " << currentMaterialID << " | ";

			// next: read the perfect clamp enum value.
			int currentPerfectClampValue = generatedMessage.readInt();
			std::cout << "Perfect clamp value: " << currentPerfectClampValue << " | ";

			// next: read the empty normal.
			ECBPolyPoint currentEmptyNormal = generatedMessage.readPoint();
			std::cout << "Empty normal: ";
			currentEmptyNormal.printPointCoords();
			std::cout << " | ";

			// next: read the boundary poly indicator data.
			int currentIndicatorData = generatedMessage.readInt();
			BoundaryPolyIndicator tempIndicator(currentIndicatorData);
			std::cout << "Boundary indicator: " << tempIndicator.getPrintableIndicatorValue() << std::endl;
		}
	}
	else
	{
		std::cout << "target ORE ";
		in_oreKey.printKey();
		std::cout << "to run printBDMForORESkeletonSGM not found. " << std::endl;
	}
}

int EnclaveCollectionBlueprint::getPolygonMapSize()
{
	return primaryPolygonMap.size();
}

void EnclaveCollectionBlueprint::copyExistingECBPolysToOtherBlueprint(OperableIntSet in_polyIDsToTransfer, EnclaveCollectionBlueprint* in_otherBlueprintRef)
{
	auto polysToTransferBegin = in_polyIDsToTransfer.intSet.begin();
	auto polysToTransferEnd = in_polyIDsToTransfer.intSet.end();
	for (; polysToTransferBegin != polysToTransferEnd; polysToTransferBegin++)
	{
		int currentPolyID = *polysToTransferBegin;
		ECBPoly polyToCopy = primaryPolygonMap[currentPolyID];
		in_otherBlueprintRef->primaryPolygonMap[currentPolyID] = polyToCopy;
	}
}

void EnclaveCollectionBlueprint::copyFractureResultsMapToOtherBlueprint(EnclaveCollectionBlueprint* in_destinationBlueprintRef)
{
	std::cout << "::: preparing to copy." << std::endl;
	std::cout << ":::: Source ORE count: " << fractureResults.fractureResultsContainerMap.size() << std::endl;
	std::cout << ":::: Destination ORE count: " << in_destinationBlueprintRef->fractureResults.fractureResultsContainerMap.size() << std::endl;
	in_destinationBlueprintRef->fractureResults = fractureResults;
}

int EnclaveCollectionBlueprint::fetchNextECBPolyKeyID()
{
	int nextECBPolyID = 0;
	if (primaryPolygonMap.size() != 0)
	{
		auto lastECBPolyID = primaryPolygonMap.rbegin();
		nextECBPolyID = (lastECBPolyID->first + 1);
	}
	return nextECBPolyID;
}

std::set<int> EnclaveCollectionBlueprint::fetchNextIDs(int in_numberOfIdsToFetch)
{
	std::set<int> returnSet;
	int nextECBPolyID = 0;

	if (primaryPolygonMap.size() != 0)
	{
		auto lastECBPolyID = primaryPolygonMap.rbegin();
		nextECBPolyID = (lastECBPolyID->first + 1);
	}

	returnSet.insert(nextECBPolyID);	// if it's just 1 ID we need to get, just insert this and be done with it.
	if (in_numberOfIdsToFetch > 1)
	{
		int remainingIDsToFetch = in_numberOfIdsToFetch - 1;
		for (int x = 0; x < remainingIDsToFetch; x++)
		{
			nextECBPolyID++;	// iterate the ID first, then insert.
			returnSet.insert(nextECBPolyID);
		}
	}

	return returnSet;
}

bool EnclaveCollectionBlueprint::doesECBPolyWithKeyAlreadyExist(int in_keyValue)
{
	bool doesExist = false;
	auto keyFinder = primaryPolygonMap.find(in_keyValue);
	if (keyFinder != primaryPolygonMap.end())
	{
		doesExist = true;
	}
	return doesExist;
}

bool EnclaveCollectionBlueprint::isCatalogEnabled()
{
	return isOrganicTriangleCatalogEnabled;
}

std::set<int> EnclaveCollectionBlueprint::fetchMorphLodToBlockAffectedOrganicTriangles(EnclaveKeyDef::EnclaveKey in_oreKey)
{
	return otCatalog.getMorphLodToBlockAffectedOrganicTriangles(in_oreKey);
}

void EnclaveCollectionBlueprint::enableAndRebuildOrganicTriangleCatalog(EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	otCatalog.rebuildCatalog(&primaryPolygonMap, in_blueprintKey);
	isOrganicTriangleCatalogEnabled = true;
}

bool EnclaveCollectionBlueprint::doesBlueprintRequireProcessing()
{
	return requiresProcessing;
}

bool EnclaveCollectionBlueprint::doesBlueprintRequireMMProcessing()
{
	return requiresMMProcessing;
}

int EnclaveCollectionBlueprint::createPolyGroupRangeAndIncrement(PolyGroupRange in_polyGroupRange)
{
	//std::cout << "!!! Poly group ID will be: " << currentPolyGroup << std::endl;
	polyGroupRangeMap[currentPolyGroup] = in_polyGroupRange;
	int returnVal = currentPolyGroup;		// get the key value we just used as the return value.
	currentPolyGroup++;						// increment the currentPolyGroup before returning.
	return returnVal;
}