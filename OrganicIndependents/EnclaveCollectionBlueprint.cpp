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
	polyGroupRangeMap = blueprint_a.polyGroupRangeMap;
	fractureResults = blueprint_a.fractureResults;
	bpTracker = blueprint_a.bpTracker;
}

std::string EnclaveCollectionBlueprint::getBlueprintHash()
{
	std::string returnedBlueprintHash = "0";

	// Remember, the OREs of a blueprint are stored in the underyling fractureResults.fractureResultsContainerMap
	// are stored in an unordered map; because of this random order, the SHA256 hash output will be different every time,
	// if we access each ORE in a random order. This is because the hash from an ORE is fed into the next ORE that is found,
	// to produce the hash.
	//
	// To alleviate this, we will first translate each EnclaveKey for each ORE in the fractureResults.fractureResultsContainerMap 
	// into a single value. A reference to that ORE is then stored in an std::map, to guarantee that the single value is ordered appropriately.
	// This should allow for a consistent hash value returned by this function.
	// 
	// Overtime, this function can be enhanced to include hashes for other components besides the OREs, but at this time it just uses OREs.
	std::map<int, OrganicRawEnclave*> orderedPointerMap;

	// Step 1: Get the EnclaveKeyDef to single value conversions for each ORE, and put them into the std::map (orderedPointerMap)
	for (auto& currentOREToTranslateKeyFor : fractureResults.fractureResultsContainerMap)
	{
		EnclaveKeyDef::EnclaveKey currentKeyToTranslate = currentOREToTranslateKeyFor.first;

		int oreX = currentKeyToTranslate.x * 64;
		int oreY = currentKeyToTranslate.y * 8;
		int finalTranslatedKey = oreX + oreY + currentKeyToTranslate.z;

		// place the reference, paired with the translated key.
		orderedPointerMap[finalTranslatedKey] = &currentOREToTranslateKeyFor.second;
	}

	// Step 2: Cycle through the orderedPointerMap, which will process the OREs and their hashes in the same consistent order,
	// to produce a the final result.
	for (auto& currentORE : orderedPointerMap)
	{
		std::string currentOREHash = currentORE.second->getOREHash();
		returnedBlueprintHash = HashUtils::sha256(returnedBlueprintHash + currentOREHash);

		std::cout << "Hash for ORE at " << currentORE.first;
		std::cout << " -> " << currentOREHash << std::endl;
	}

	return returnedBlueprintHash;
}

MessageContainer EnclaveCollectionBlueprint::convertBlueprintTOBDMFormat(EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	// Remember: the returning MessageContainer must be MessageContainerType::MC_BDM, so that
	// if we are sending it to an OrganicSystem instance, the MessageCable knows what to do with it.
	// See the ServerMessageInterpreter class in OrganicServerLib, and CoreMessageInterpreter in OrganicCoreLib,
	// for examples of this usage.
	MessageContainer blueprintConvertedContainer(MessageContainerType::MC_BDM);

	// The returning MessageContainer should have the following:
	//
	// 1 blueprint header message (BDM_BLUEPRINT_HEADER)
	// 1 message for EACH ECB_POLY.
	// The appended contents of each MessageContainer that is returned by the call to OrganicRawEnclave::convertOREToBDMFormat for each ORE in fractureResults
	blueprintConvertedContainer.insertMessage(createBDMBlueprintHeaderMessage(in_blueprintKey));

	// Step 1: construct and insert the BDM_BLUEPRINT HEADER.
	//	...code for building this is TBD

	// Step 2: construct and insert Message for each ECB_POLY (BDM_BLUEPRINT_ECBPOLY)
	//	...code for building this is TBD
	for (auto& currentECBPolyToConvert : primaryPolygonMap)
	{
		int currentKeyedPolyID = currentECBPolyToConvert.first;
		Message currentECBPolyConvertedMessage = currentECBPolyToConvert.second.produceBDMFormatECBPolyMessage(in_blueprintKey, currentKeyedPolyID);
		blueprintConvertedContainer.insertMessage(currentECBPolyConvertedMessage);
	}

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

Message EnclaveCollectionBlueprint::createBDMBlueprintHeaderMessage(EnclaveKeyDef::EnclaveKey in_blueprintKey)
{
	Message blueprintHeaderMessage(MessageType::BDM_BLUEPRINT_HEADER);

	// first value: Key value that represents the blueprint.
	blueprintHeaderMessage.insertEnclaveKey(in_blueprintKey);

	// second value: number of ECBPolys.
	blueprintHeaderMessage.insertInt(primaryPolygonMap.size());

	// third value: number of OREs.
	blueprintHeaderMessage.insertInt(fractureResults.fractureResultsContainerMap.size());

	return blueprintHeaderMessage;
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

		// UPDATE 6
		//primaryPolygonMap[in_keyValue].ecbPolyPoints[in_lineID] = in_ecbPolyLine;
		primaryPolygonMap[in_keyValue].ecbPolyPoints[in_lineID] = in_ecbPolyLine.pointA;
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

		// All of the associated triangles to print out will come from a Message
		// that is returned from a call to fetchBDMForORERTHandler. The methodology to
		// interpret that Message must be done in this function.
		Message generatedMessage = fractureResults.fetchBDMForORERTHandler(in_blueprintKey, in_oreKey);

		std::cout << ">>>>>>>>>>> Beginning test of read of BDM oreRTHandler data message. " << std::endl;
		generatedMessage.open();

		// Optional for output, but the blueprint and ORE keys must be read / stripped out first.
		EnclaveKeyDef::EnclaveKey readBPKey = generatedMessage.readEnclaveKey();
		EnclaveKeyDef::EnclaveKey readOreKey = generatedMessage.readEnclaveKey();
		
		// The next value in the Message should be the total number of RenderableTriangleBase objects
		// that we will be reading.
		int totalRenderableTriangleBaseEntries = generatedMessage.readInt();	// the total number of entries found in the oreRTHandler member of the ORE.

		// As we iterate through each set of data the pertains to a RenderableTriangleBase, 
		// we must first get the RCategoryEnum, related supergroup, and RDerivedTypeEnum, in that order, from
		// the currently opened Message. This will determine the next steps to take for interpreting
		// the contents of the Message.
		for (int x = 0; x < totalRenderableTriangleBaseEntries; x++)
		{
			RCategoryEnum currentRType = RCategoryEnum(generatedMessage.readInt());	// Read the RCategoryEnum (eg, TERRAIN_TILE_1), 
																			// so we know which category in the map it goes into.

			int currentSupergroup = generatedMessage.readInt();	// get the supergroup.

			RDerivedTypeEnum currentRenderingType = RDerivedTypeEnum(generatedMessage.readInt());	// get enum value that represents the derived type;
																									// this value will determine what derived 
																									// class of RenderableTriangleBase that we will use.

			// The current rendering type needs to be used, to determine how to print out the data for the current RenderableTriangleBase entry 
			// that we are looking at.
			switch (currentRenderingType)
			{
				// R_TILED matches to RenderableTiledTriangle
				case RDerivedTypeEnum::R_TILED:
				{
					// Print the supergroup
					std::cout << "Supergroup: " << currentSupergroup << " | ";

					// Determine the category to print out
					std::cout << "RCategoryEnum: ";
					switch (currentRType)
					{
						case RCategoryEnum::TERRAIN_TILE_1:
						{
							std::cout << "TERRAIN_TILE_1";
							break;
						}
					}
					std::cout << " |";

					// Determine the derived type to print out
					std::cout << "RDerivedTypeEnum: ";
					switch (currentRenderingType)
					{
						case RDerivedTypeEnum::R_TILED:
						{
							std::cout << "R_TILED";
							break;
						}
					}
					std::cout << " |";

					// Now, print in this order: points (3), material, clamp value, empty normal, boundary indicator
					std::cout << " | Points: ";
					for (int x = 0; x < 3; x++)
					{
						FTrianglePoint currentPoint = generatedMessage.readPoint();
						currentPoint.printPointData();
						std::cout << " | ";
					}

					// next: set the material ID.
					TriangleMaterial tilingMaterial = TriangleMaterial(generatedMessage.readInt());

					// next: set the perfect clamp enum value.
					PerfectClampEnum rClampEnum = PerfectClampEnum(generatedMessage.readInt());

					// next: set the empty normal.
					ECBPolyPoint rEmptyNormal = generatedMessage.readPoint();

					// next: set the indicator data.
					BoundaryPolyIndicator rBoundaryIndicator;
					rBoundaryIndicator.setIndicatorData(unsigned char(generatedMessage.readInt()));

					std::cout << std::endl;


					break;
				}
			}
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