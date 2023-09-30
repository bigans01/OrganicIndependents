#include "stdafx.h"
#include "ReconstitutionManager.h"

void ReconstitutionManager::processMessageContainer(MessageContainer* in_messageContainer)
{
	// cycle through each Message, and read the blueprint key before stripping it out, and then send the rest of the message to
	// the appropriate mapped ReconstitutedBlueprint.
	//
	// This function doesn't validate the contentes of the MessageContainer in any shape or form; that must be done outside of this function.

	auto currentMessage = in_messageContainer->begin();
	auto messageEnd = in_messageContainer->end();
	for (; currentMessage != messageEnd; currentMessage++)
	{
		Message currentMessageCopy = *currentMessage;
		currentMessageCopy.open();
		
		// read the enclave key, then be sure to remove it.
		EnclaveKeyDef::EnclaveKey targetBlueprintKey = currentMessageCopy.readEnclaveKey();
		currentMessageCopy.removeEnclaveKeyFromFrontAndResetIter(1);

		// send to the appropriate instance of ReconstitutedBlueprint.
		//reconstitutionDock[targetBlueprintKey].handleBDMMessage(currentMessageCopy);
		handlebDMMessageForDockEntry(targetBlueprintKey, currentMessageCopy);
	}
}

void ReconstitutionManager::handlebDMMessageForDockEntry(EnclaveKeyDef::EnclaveKey in_targetBlueprintToLoad, Message in_messageDataCopy)
{
	std::lock_guard<std::mutex> existLock(dockMutex);
	reconstitutionDock[in_targetBlueprintToLoad].handleBDMMessage(in_messageDataCopy);
}

void ReconstitutionManager::insertMessageContainerForProcessing(MessageContainer in_containerForProcessing)
{
	// The lock guard below is used to ensure that the dedicated network thread will only be inserting into the 
	// processableContainers member, in a safe manner. In reality, this function should only be called on a thread that is completely separate
	// and independent of the dedicated blueprint thread.
	std::lock_guard<std::mutex> insertGuard(processingContainerMutex);
	processableContainers.push(in_containerForProcessing);
}

void ReconstitutionManager::executeContainerProcessing()
{
	// attempts to process any members in the processableContainers, as long as there are things to containers to process OR until the MessageContainer 
	// processing limit is reached (this limit is not yet implemented, as of 9/30/2023). Should only be run by
	// the dedicated blueprint thread.
	std::lock_guard<std::mutex> insertGuard(processingContainerMutex);
	while (!processableContainers.empty())
	{
		auto currentQueueFront = processableContainers.front();
		processMessageContainer(&currentQueueFront);
		processableContainers.pop();
	}
}

void ReconstitutionManager::checkForReconstitutableBlueprints()
{
	// Cycle through the dock, check for any blueprints that are ready for processing, and if they are, process them (i.e, build OREs, ECBPolys, etc)
}

void ReconstitutionManager::processMessageContainersAndCheckForReconstitutables()
{
	executeContainerProcessing();	// use a lock guard to block the networking thread while processing the Messages;
	checkForReconstitutableBlueprints();	// after the BDM Message instances have been processed into the dock, check the dock for reconstitutable blueprints. 
}

void ReconstitutionManager::printReconstitutedBlueprintStats(EnclaveKeyDef::EnclaveKey in_blueprintStatsToFetch)
{
	//auto existingBlueprintFinder = reconstitutionDock.find(in_blueprintStatsToFetch);
	//if (existingBlueprintFinder != reconstitutionDock.end())
	if (doesBlueprintExistInDock(in_blueprintStatsToFetch));
	{
		std::cout << "(ReconstitutionManager):: found stats to print, for blueprint having key: "; 
		in_blueprintStatsToFetch.printKey();
		std::cout << std::endl;

		reconstitutionDock[in_blueprintStatsToFetch].printReconstitutedMetadata();
	}
}

void ReconstitutionManager::runOREReconstitutionSpecific(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey, EnclaveKeyDef::EnclaveKey in_containingOREKey)
{
	//auto existingBlueprintFinder = reconstitutionDock.find(in_containingBlueprint);
	//if (existingBlueprintFinder != reconstitutionDock.end())
	if (doesBlueprintExistInDock(in_containingBlueprintKey))
	{
		std::cout << "Found existing blueprint to run ORE reconstitution..." << std::endl;
		//auto existingOREFinder = reconstitutionDock[in_containingBlueprint].reconstitutedOREMap.find(in_containingORE);
		//if (existingOREFinder != reconstitutionDock[in_containingBlueprint].reconstitutedOREMap.end())
		if (doesOREExistInDock(in_containingBlueprintKey, in_containingOREKey))
		{
			std::cout << "Found existing ORE for reconstitution..." << std::endl;

			// Remember: the call to runConstitution READS from its Messsage data, to generate an ORE, then WRITES out the ORE to the referenced generatedBlueprints
			// map member of this class.
			reconstitutionDock[in_containingBlueprintKey].reconstitutedOREMap[in_containingOREKey].runReconstitution(in_containingBlueprintKey,
																													 in_containingOREKey,
																													&generatedBlueprintsMutex, 
																													&generatedBlueprints);
		}
	}
}

void ReconstitutionManager::attemptFullReconstitution(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey)
{
	if (reconstitutionDock[in_containingBlueprintKey].reconBlueprintHeader.reconstituted)
	{
		std::cout << "Found reconsituted blueprint header for blueprint having key: ";
		in_containingBlueprintKey.printKey();
		std::cout << std::endl;

		Message storedMessage = reconstitutionDock[in_containingBlueprintKey].reconBlueprintHeader.reconstitutionMessage;
		storedMessage.open();

		// Step 1: Read the total number of ECBPolys, and OREs; we must make sure the reconstituted number of each of these
		// matches what is in the MessageType::BDM_BLUEPRINT_HEADER Message. The value for total ECBPolys is read first, then the total OREs.
		int targetNumberOfPolys = storedMessage.readInt();
		int targetNumberOfORES = storedMessage.readInt();

		// Step 2: Reconstitute ECBPoly instances, and export them to the associated EnclaveCollectionBlueprint. 
		reconstitutionDock[in_containingBlueprintKey].runReconstitutionOnAllBDMPolyData(in_containingBlueprintKey, &generatedBlueprints);

		// Step 3: Reconstitute the OREs, and export them to the associated EnclaveCollectionBlueprint.
		for (auto& currentOREToProcess : reconstitutionDock[in_containingBlueprintKey].reconstitutedOREMap)
		{
			EnclaveKeyDef::EnclaveKey currentOREToProcessKey = currentOREToProcess.first;
			currentOREToProcess.second.runReconstitution(in_containingBlueprintKey,
				currentOREToProcessKey,
				&generatedBlueprintsMutex,
				&generatedBlueprints);
		}

		// Step 4: Check the number of generated ECBPoly and ORE instances.
		if
		(
			(generatedBlueprints[in_containingBlueprintKey].getPolygonMapSize() == targetNumberOfPolys)
			&&
			(generatedBlueprints[in_containingBlueprintKey].fractureResults.fractureResultsContainerMap.size() == targetNumberOfORES)
		)
		{
			std::cout << "Number of generated ECBPolys and OREs matched blueprint header! " << std::endl;
		}
	}
}

OrganicRawEnclave ReconstitutionManager::fetchReconstitutedORE(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey, EnclaveKeyDef::EnclaveKey in_containingOREKey)
{
	return generatedBlueprints[in_containingBlueprintKey].fractureResults.fractureResultsContainerMap[in_containingOREKey];
}

EnclaveCollectionBlueprint ReconstitutionManager::fetchReconstitutedBlueprint(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey)
{
	return generatedBlueprints[in_containingBlueprintKey];
}

bool ReconstitutionManager::doesBlueprintExistInDock(EnclaveKeyDef::EnclaveKey in_blueprintToCheck)
{
	std::lock_guard<std::mutex> existLock(dockMutex);
	bool blueprintExists = false;
	auto existingBlueprintFinder = reconstitutionDock.find(in_blueprintToCheck);
	if (existingBlueprintFinder != reconstitutionDock.end())
	{
		blueprintExists = true;
	}
	return blueprintExists;
}

bool ReconstitutionManager::doesOREExistInDock(EnclaveKeyDef::EnclaveKey in_blueprintToCheck, EnclaveKeyDef::EnclaveKey in_OREToCheck)
{
	std::lock_guard<std::mutex> existLock(dockMutex);
	bool oreExists = false;
	auto existingBlueprintFinder = reconstitutionDock.find(in_blueprintToCheck);
	if (existingBlueprintFinder != reconstitutionDock.end())
	{
		auto existingOREFinder = reconstitutionDock[in_blueprintToCheck].reconstitutedOREMap.find(in_OREToCheck);
		if (existingOREFinder != reconstitutionDock[in_blueprintToCheck].reconstitutedOREMap.end())
		{
			//reconstitutionDock[in_blueprintToCheck].reconstitutedOREMap[in_OREToCheck].runReconstitution();
			oreExists = true;
		}
	}
	return oreExists;
}