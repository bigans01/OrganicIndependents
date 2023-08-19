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
		reconstitutionDock[targetBlueprintKey].handleBDMMessage(currentMessageCopy);
	}
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
	auto existingBlueprintFinder = reconstitutionDock.find(in_blueprintStatsToFetch);
	if (existingBlueprintFinder != reconstitutionDock.end())
	{
		std::cout << "(ReconstitutionManager):: found stats to print, for blueprint having key: "; 
		in_blueprintStatsToFetch.printKey();
		std::cout << std::endl;

		reconstitutionDock[in_blueprintStatsToFetch].printReconstitutedMetadata();
	}
}