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
	processableContainers.push(in_containerForProcessing);
}

void ReconstitutionManager::executeContainerProcessing()
{
	// attempts to process any members in the processableContainers, as long as there are things to containers to process.
	while (!processableContainers.empty())
	{
		auto currentQueueFront = processableContainers.front();
		processMessageContainer(&currentQueueFront);
		processableContainers.pop();
	}
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

OrganicRawEnclave ReconstitutionManager::fetchReconstitutedORE(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey, EnclaveKeyDef::EnclaveKey in_containingOREKey)
{
	return generatedBlueprints[in_containingBlueprintKey].fractureResults.fractureResultsContainerMap[in_containingOREKey];
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