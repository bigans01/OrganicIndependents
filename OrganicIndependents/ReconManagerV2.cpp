#include "stdafx.h"
#include "ReconManagerV2.h"


void ReconManagerV2::handlebDMMessageForDockEntry(EnclaveKeyDef::EnclaveKey in_targetBlueprintToLoad, Message in_messageDataCopy)
{
	std::lock_guard<std::mutex> existLock(dockMutex);
	reconstitutionDock[in_targetBlueprintToLoad].handleBDMMessage(in_messageDataCopy);
}

bool ReconManagerV2::hasBlueprintsToProcess()
{
	return !ticketPipe.empty();
}

void ReconManagerV2::printPipelineTickets()
{
	std::cout << "ReconManagerV2: printing pipeline tickets..." << std::endl;
	ticketPipe.printTickets();
	std::cout << "ReconManagerV2: total number of containers to process: ";
	std::cout << processableContainers.size() << std::endl;

	std::cout << "ReconManagerV2: tickets left to process before next batch: ";
	std::cout << getTicketCounterValue() << std::endl;

	std::cout << "ReconManagerV2: size of next batch: ";
	std::cout << processableContainers.size() - getTicketCounterValue() << std::endl;

	std::cout << "ReconManagerV2: number of processed, unexported, BP keys:";
	std::cout << getBPKeyCount() << std::endl;

	std::cout << "ReconManagerV2: number of tickets in pipe: ";
	std::cout << ticketPipe.size() << std::endl;

	std::cout << "ReconManagerV2: ticket counter value is: ";
	std::cout << remainingContainerTicketCounter << std::endl;

	std::cout << "ReconManagerV2: number of ready generated blueprints: ";
	std::cout << getNumberOfReadyBlueprints() << std::endl;
}

void ReconManagerV2::insertMessageContainerForProcessing(MessageContainer in_containerForProcessing)
{
	// The lock guard below is used to ensure that the dedicated network thread will only be inserting into the 
	// processableContainers member, in a safe manner. In reality, this function should only be called on a thread that is completely separate
	// and independent of the dedicated blueprint thread.
	processableContainers.push(std::move(in_containerForProcessing));
}

void ReconManagerV2::checkForNewContainerTickets()
{
	// If there are containers to process, yet no tickets, create some new tickets;
	// if there are containers to process but existing ticekts, don't get another batch of tickets
	// until all of the current ones are processed.
	if 
	(
		!processableContainers.empty()
		&& 
		getTicketCounterValue() == 0
	)
	{
		// Increment the ticket counter by the current container size.
		incrementTicketCounter(processableContainers.size());
		
		// Now, get the ticket value again, to determine how many tickets to add.
		int numberOfTicketsToAdd = getTicketCounterValue();
		for (int x = 0; x < numberOfTicketsToAdd; x++)
		{
			// remember: one container, one ticket.
			ticketPipe.insertTicket(RMWorkPipeTicketEnum::PROCESS_BLUEPRINTS);
		}
	}
}

void ReconManagerV2::manuallyPushBlueprintMoveTicket()
{
	if 
	(ticketPipe.contains_value(RMWorkPipeTicketEnum::PROCESS_BLUEPRINTS) && !manualMovePushCalled)
	{
		ticketPipe.insertTicket(RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK);
		manualMovePushCalled = true;
	}
}

void ReconManagerV2::checkForCompletedBlueprintsReadyToExport()
{
	// If there are any blueprints that are officially done, signal that we have 
	// to move them over to the export map.
	if (doAnyGeneratedBPKeysExist())
	{
		// remember, if there's alreaqy a move ticket in the pipe, this will do nothing.
		ticketPipe.insertTicket(RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK);
	}
}

void ReconManagerV2::incrementTicketCounter(int in_valueToAdd)
{
	std::lock_guard<std::mutex> counterLock(ticketCounterMutex);
	remainingContainerTicketCounter += in_valueToAdd;
}

void ReconManagerV2::decrementTicketCounter() 
{
	std::lock_guard<std::mutex> counterLock(ticketCounterMutex);
	remainingContainerTicketCounter--;
}

int ReconManagerV2::getTicketCounterValue()
{
	std::lock_guard<std::mutex> counterLock(ticketCounterMutex);
	return remainingContainerTicketCounter;
}

void ReconManagerV2::moveGeneratedBlueprintsToReady()
{
	std::lock_guard<std::mutex> readyMoveLock(readyBlueprintsMutex);
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> keysToErase;
	
	// transfer each blueprint.
	for (auto& currentGeneratedBP : generatedBlueprints)
	{
		readyBlueprints[currentGeneratedBP.first] = currentGeneratedBP.second;
		keysToErase.insert(currentGeneratedBP.first);
	}

	// erase the ones that were transferred, from generatedBlueprints;
	// also remove from reconstitutionDock.
	for (auto currentKeyToErase : keysToErase)
	{
		reconstitutionDock.erase(currentKeyToErase);
		generatedBlueprints.erase(currentKeyToErase);
		eraseBPKey(currentKeyToErase);
	}
}

int ReconManagerV2::getNumberOfReadyBlueprints()
{
	std::lock_guard<std::mutex> readyMoveLock(readyBlueprintsMutex);
	return readyBlueprints.size();
}

std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> ReconManagerV2::getReadyBlueprintKeys()
{
	std::lock_guard<std::mutex> readyMoveLock(readyBlueprintsMutex);
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> readySet;

	for (auto& currentBlueprintKey : readyBlueprints)
	{
		readySet.insert(currentBlueprintKey.first);
	}
	return readySet;
}


void ReconManagerV2::transferBlueprintOut(EnclaveKeyDef::EnclaveKey in_transferKey,
	std::mutex* in_transferTargetMutex,
	std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>* in_targetBlueprintMap
)
{
	// Two locks needed: one for readyBlueprints, one for the exterior map we're transferring into...
	std::lock_guard<std::mutex> readyMoveLock(readyBlueprintsMutex);
	std::lock_guard<std::mutex> exteriorLock(*in_transferTargetMutex);

	// transfer the blueprint over.
	(*in_targetBlueprintMap)[in_transferKey] = readyBlueprints[in_transferKey];

	// delete the transfred blueprints from readyBlueprints, as it should no longer have any use.
	readyBlueprints.erase(in_transferKey);
	
}

void ReconManagerV2::runManagerOnThread()
{
	setManagerRunState(true);

	while (getManagerRunState())
	{
		// always: check if loop is halted via the CV.
		checkForReconstitutionHalt();

		// Check if there's tickets in the work pipe.
		if (!ticketPipe.empty())
		{
			auto currentTicket = ticketPipe.front();

			// if PROCESS_BLUEPRINTS, process exactly 1 blueprint. Process means, attempt full reconstitution
			// and then move it into the generatedBlueprints map.
			if (currentTicket == RMWorkPipeTicketEnum::PROCESS_BLUEPRINTS)
			{
				// 1. do execute container processsing...
				// 2. reconstitute the blueprint...
				// 3. insert the key of the processed blueprint into generatedBPSet.
				processAndReconstituteNextContainer();

				// decrement the blueprint ticket counter, by 1.
				decrementTicketCounter();
			}

			// ...other wise, if the ticket is MOVE_BLUEPRINTS_FROM_DOCK (which must be set by another thread),
			// move over all finished blueprints from the generatedBlueprints into the readyBlueprints (sp?) map.
			// The associated EnclaveKey of each blueprint that has been moved into readyBlueprints, should have its key
			// removed from the generatedBPSet.
			else if (currentTicket == RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK)
			{
				moveGeneratedBlueprintsToReady();
			}

			ticketPipe.pop();
		}

	}
}

void ReconManagerV2::runPipeProcessingTick()
{
	// Check if there's tickets in the work pipe.
	if (!ticketPipe.empty())
	{
		auto currentTicket = ticketPipe.front();

		// if PROCESS_BLUEPRINTS, process exactly 1 blueprint. Process means, attempt full reconstitution
		// and then move it into the generatedBlueprints map.
		if (currentTicket == RMWorkPipeTicketEnum::PROCESS_BLUEPRINTS)
		{
			// 1. do execute container processsing...
			// 2. reconstitute the blueprint...
			// 3. insert the key of the processed blueprint into generatedBPSet.
			processAndReconstituteNextContainer();

			// decrement the blueprint ticket counter, by 1.
			decrementTicketCounter();
		}

		// ...other wise, if the ticket is MOVE_BLUEPRINTS_FROM_DOCK (which must be set by another thread),
		// move over all finished blueprints from the generatedBlueprints into the readyBlueprints (sp?) map.
		// The associated EnclaveKey of each blueprint that has been moved into readyBlueprints, should have its key
		// removed from the generatedBPSet.
		else if (currentTicket == RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK)
		{
			moveGeneratedBlueprintsToReady();
		}

		ticketPipe.pop();
	}
}

bool ReconManagerV2::getManagerRunState()
{
	std::lock_guard<std::mutex> lock(managerRuntimeMutex);
	return continueRunningFlag;
}

void ReconManagerV2::setManagerRunState(bool in_value)
{
	std::lock_guard<std::mutex> lock(managerRuntimeMutex);
	continueRunningFlag = in_value;
}

void ReconManagerV2::checkForReconstitutionHalt()
{
	if (getReconstitutionLoopRunValue() == 0)
	{
		std::cout << "ReconstitutionManager reconstitution loop halted!" << std::endl;
		std::unique_lock<std::mutex> reconLock(reconstitutionLoopMutex);
		reconstitutionLoopCV.wait(reconLock, [&] { return getReconstitutionLoopRunValue() == 1;});
		std::cout << "ReconstitutionManager reconstitution loop resumed!" << std::endl;
	}
}

void ReconManagerV2::haltReconstitutionLoop()
{
	std::lock_guard<std::mutex> lock(reconPipelineLoopMutex);
	shouldReconstitutionLoopRun = 0;
}

void ReconManagerV2::resumeReconstitutionLoop()
{
	std::lock_guard<std::mutex> lock(reconPipelineLoopMutex);
	shouldReconstitutionLoopRun = 1;
	reconstitutionLoopCV.notify_all();
}

int ReconManagerV2::getReconstitutionLoopRunValue()
{
	std::lock_guard<std::mutex> lock(reconPipelineLoopMutex);
	return shouldReconstitutionLoopRun;
}

void ReconManagerV2::processAndReconstituteNextContainer()
{
	if (!processableContainers.empty())
	{
		// although a container could theoretically contain more than one blueprint, it should really only contain a single blueprint.
		// See how CoreMessageInterpreter::handleResponseFromServerDirectBDMData in OrganicCoreLib is utilized, for more context.
		auto currentQueueFront = processableContainers.front();
		EnclaveKeyDef::EnclaveKey processedKey = processMessageContainer(&currentQueueFront);
		processableContainers.pop();

		// use the processed key, to reconstitute that specific blueprint.
		attemptReconstitution(processedKey);

	}
}

void ReconManagerV2::attemptReconstitution(EnclaveKeyDef::EnclaveKey in_blueprintToReconstitute)
{
	if (reconstitutionDock[in_blueprintToReconstitute].getReconstitutedState() == ReconstitutedBlueprintRunState::WAITING_FOR_RUN)
	{
		if
		(
			(reconstitutionDock[in_blueprintToReconstitute].reconBlueprintHeader.reconstituted)
		)
		{
			std::cout << "Found reconsituted blueprint header for blueprint having key: ";
			//in_blueprintToReconstitute.printKey();
			//std::cout << "-----" << std::endl;



			Message storedMessage = reconstitutionDock[in_blueprintToReconstitute].reconBlueprintHeader.reconstitutionMessage;
			storedMessage.open();

			// Step 1: Read the total number of ECBPolys, and OREs; we must make sure the reconstituted number of each of these
			// matches what is in the MessageType::BDM_BLUEPRINT_HEADER Message. The value for total ECBPolys is read first, then the total OREs.
			int targetNumberOfPolys = storedMessage.readInt();
			int targetNumberOfORES = storedMessage.readInt();

			// Step 2: Reconstitute ECBPoly instances, and export them to the associated EnclaveCollectionBlueprint. 
			reconstitutionDock[in_blueprintToReconstitute].runReconstitutionOnAllBDMPolyData(in_blueprintToReconstitute, &generatedBlueprints);

			// Step 3: Reconstitute the OREs, and export them to the associated EnclaveCollectionBlueprint.
			for (auto& currentOREToProcess : reconstitutionDock[in_blueprintToReconstitute].reconstitutedOREMap)
			{
				EnclaveKeyDef::EnclaveKey currentOREToProcessKey = currentOREToProcess.first;
				currentOREToProcess.second.runReconstitution(in_blueprintToReconstitute,
					currentOREToProcessKey,
					&generatedBlueprintsMutex,
					&generatedBlueprints);
			}

			// Step 4: Check the number of generated ECBPoly and ORE instances; if these corresponding numbers match with what
			// is expected in the reconBlueprintHeader Message, this can be considered successful. Otherwise, it's a failure.
			// (this success/failure logic may be changed in the future)
			if
				(
					(generatedBlueprints[in_blueprintToReconstitute].getPolygonMapSize() == targetNumberOfPolys)
					&&
					(generatedBlueprints[in_blueprintToReconstitute].fractureResults.fractureResultsContainerMap.size() == targetNumberOfORES)
					)
			{
				// Flag that this operation was successful, and also update the appropriate entry in generatedBPSet.
				reconstitutionDock[in_blueprintToReconstitute].setStateAsSuccessfulRun();	
				insertGeneratedBPKey(in_blueprintToReconstitute);

				std::cout << "Number of generated ECBPolys and OREs matched blueprint header! " << std::endl;
			}
			else
			{
				reconstitutionDock[in_blueprintToReconstitute].setStateAsFailedRun();	// Flag that this operation was a failure.
			}
		}
	}
}

void ReconManagerV2::insertGeneratedBPKey(EnclaveKeyDef::EnclaveKey in_keyToInsert)
{ 
	std::lock_guard<std::mutex> lock(generatedSetMutex);
	generatedBPSet.insert(in_keyToInsert);
}

bool ReconManagerV2::doAnyGeneratedBPKeysExist()
{
	std::lock_guard<std::mutex> lock(generatedSetMutex);
	return !generatedBPSet.empty();
}

EnclaveKeyDef::EnclaveKey ReconManagerV2::fetchFirstBPKey()
{
	std::lock_guard<std::mutex> lock(generatedSetMutex);
	return *generatedBPSet.begin();
}

void ReconManagerV2::eraseBPKey(EnclaveKeyDef::EnclaveKey in_keyToErase)
{
	std::lock_guard<std::mutex> lock(generatedSetMutex);
	generatedBPSet.erase(in_keyToErase);
}

void ReconManagerV2::printBPKeys()
{
	std::lock_guard<std::mutex> lock(generatedSetMutex);
	std::cout << "ReconManagerV2: printing processed BP keys:" << std::endl;
	for (auto& currentKey : generatedBPSet)
	{
		EnclaveKeyDef::EnclaveKey keyCopy = currentKey;
		keyCopy.printKey();
		std::cout << std::endl;
	}
}

int ReconManagerV2::getBPKeyCount()
{
	std::lock_guard<std::mutex> lock(generatedSetMutex);
	return generatedBPSet.size();
}

EnclaveKeyDef::EnclaveKey ReconManagerV2::processMessageContainer(MessageContainer* in_messageContainer)
{
	// cycle through each Message, and read the blueprint key before stripping it out, and then send the rest of the message to
	// the appropriate mapped ReconstitutedBlueprint.
	//
	// This function doesn't validate the contentes of the MessageContainer in any shape or form; that must be done outside of this function.

	EnclaveKeyDef::EnclaveKey returnKey;

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
		handlebDMMessageForDockEntry(targetBlueprintKey, currentMessageCopy);

		returnKey = targetBlueprintKey;
	}

	return returnKey;
}