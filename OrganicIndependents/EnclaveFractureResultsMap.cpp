#include "stdafx.h"
#include "EnclaveFractureResultsMap.h"


void EnclaveFractureResultsMap::insertFractureResults(EnclaveKeyDef::EnclaveKey in_enclaveKey, int in_polygonId, OrganicTriangleSecondary in_container, EnclaveTriangleContainer* in_enclaveTriangleContainer, std::mutex* in_mutexRef)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	//fractureResultsContainerMap[in_enclaveKey].insertResults(in_polygonId, in_container);

	// remember, since we're adding the results of a fracture, we want to add to cluster ID 0.
	fractureResultsContainerMap[in_enclaveKey].insertTiledTriangleIntoRTHandler(in_polygonId, 
																			   0, 
																			   *in_enclaveTriangleContainer,
																			   in_container);
}

void EnclaveFractureResultsMap::transferEnclaveTrianglesIntoOREs(int in_polygonID, OrganicTriangle* in_organicTriangleRef, std::mutex* in_mutexRef)
{
	auto enclaveBegin = in_organicTriangleRef->polyMap.enclavePolyMap.begin();
	auto enclaveEnd = in_organicTriangleRef->polyMap.enclavePolyMap.end();
	for (enclaveBegin; enclaveBegin != enclaveEnd; enclaveBegin++)
	{
		OrganicTriangleSecondary container;
		//std::cout << "!# Loading into container... " << std::endl;
		container.loadDataFromEnclaveTriangleContainer(&enclaveBegin->second.producedResultsContainer);
		insertFractureResults(enclaveBegin->first, in_polygonID, container, &enclaveBegin->second.producedResultsContainer, in_mutexRef);
	}
}

void EnclaveFractureResultsMap::insertNewRawManifest(EnclaveKeyDef::EnclaveKey in_enclaveKey, std::mutex* in_mutexRef)
{
	std::lock_guard<std::mutex> lock(*in_mutexRef);
	OrganicRawManifest newManifest;
	rawManifestMap[in_enclaveKey] = newManifest;
}

MassDriverLimitArray EnclaveFractureResultsMap::produceMassDriverLimitArray()
{
	MassDriverLimitArray returnLimitArray;
	auto organicRawEnclavesBegin = fractureResultsContainerMap.begin();
	auto organicRawEnclavesEnd = fractureResultsContainerMap.end();
	for (organicRawEnclavesBegin; organicRawEnclavesBegin != organicRawEnclavesEnd; organicRawEnclavesBegin++)
	{
		std::set<int> touchedBlocks = organicRawEnclavesBegin->second.getTouchedBlockList();	// get the touched block list
		auto touchedBlocksBegin = touchedBlocks.begin();
		auto touchedBlocksEnd = touchedBlocks.end();
		//std::cout << "Gathering touched blocks for key: " << organicRawEnclavesBegin->first.x << ", " << organicRawEnclavesBegin->first.y << ", " << organicRawEnclavesBegin->first.z << "; there are " << touchedBlocks.size() << " touched blocks in this OrganicRawEnclave. " << std::endl;
		for (touchedBlocksBegin; touchedBlocksBegin != touchedBlocksEnd; touchedBlocksBegin++)
		{
			EnclaveKeyDef::EnclaveKey blockKey = PolyUtils::convertSingleToBlockKey(*touchedBlocksBegin);
			returnLimitArray.updateLimit(organicRawEnclavesBegin->first, blockKey);
		}
	}
	return returnLimitArray;
}

bool EnclaveFractureResultsMap::checkIfAnyOREsExist()
{
	return !fractureResultsContainerMap.empty();	// returns true if there are OREs
}

bool EnclaveFractureResultsMap::checkIfSpecificOREExists(EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	bool wasOREFound = false;
	auto findSpecificORE = fractureResultsContainerMap.find(in_enclaveKey);
	if (findSpecificORE != fractureResultsContainerMap.end())	// it was found
	{
		wasOREFound = true;
	}
	return wasOREFound;
}

bool EnclaveFractureResultsMap::checkIfSpecificOREContainsAnyBlocks(EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	bool wasOREFound = false;
	auto findSpecificORE = fractureResultsContainerMap.find(in_enclaveKey);
	if (findSpecificORE != fractureResultsContainerMap.end())	// it was found
	{
		wasOREFound = !findSpecificORE->second.blockMap.empty();	// return true if it is NOT empty (has blocks)
	}
	return wasOREFound;
}

EnclaveBlockState EnclaveFractureResultsMap::getBlockStateFromORE(EnclaveKeyDef::EnclaveKey in_enclaveKey, EnclaveKeyDef::EnclaveKey in_blockKey)
{
	EnclaveBlockState returnState = EnclaveBlockState::NONEXISTENT;

	auto findSpecificORE = fractureResultsContainerMap.find(in_enclaveKey);
	if (findSpecificORE != fractureResultsContainerMap.end())	// it was found
	{
		// return true if the status of the block is EXPOSED or UNEXPOSED, as this signifies it exists (assumes that the ORE is set up correctly)
		returnState = findSpecificORE->second.getBlockStatus(in_blockKey);
	}
	return returnState;

}

Message EnclaveFractureResultsMap::fetchBDMForORERTHandler(EnclaveKeyDef::EnclaveKey in_blueprintKeyForRequest, EnclaveKeyDef::EnclaveKey in_enclaveKeyForRequest)
{
	// Remember, a blank Message will be returned if the ORE isn't found. So the Message type will be NONE.
	// Regardless, the best practice is to check that the ORE exists before calling this.
	Message returnMessage;
	auto findSpecificORE = fractureResultsContainerMap.find(in_enclaveKeyForRequest);
	if (findSpecificORE != fractureResultsContainerMap.end())	// it was found
	{
		returnMessage = findSpecificORE->second.fetchBDMMessageForRTHandler(in_blueprintKeyForRequest, in_enclaveKeyForRequest);
	}
	return returnMessage;
}