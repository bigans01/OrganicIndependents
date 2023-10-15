#include "stdafx.h"
#include "ReconstitutedBlueprint.h"

void ReconstitutedBlueprint::handleBDMMessage(Message in_bdmMessage)
{
	in_bdmMessage.open();
	switch (in_bdmMessage.messageType)
	{
		case MessageType::BDM_BLUEPRINT_HEADER:
		{
			reconBlueprintHeader.setReconstitutionMessage(in_bdmMessage);

			std::cout << "(ReconstitutedBlueprint:: handleBDMMessage): found blueprint header message (MessageType::BDM_BLUEPRINT_HEADER); will send to appropriate dock. " << std::endl;
			int sendWait = 3;
			std::cin >> sendWait;

			break;
		}	

		case MessageType::BDM_BLUEPRINT_ECBPOLY:
		{
			// read in the ECBPoly key, which should be immediately after the blueprint key that was read in, and then strip it out.
			int reconstitutedECBPolyID = in_bdmMessage.readInt();
			in_bdmMessage.removeIntsFromFrontAndResetIter(1);
			reconstitutedECBPolyMap[reconstitutedECBPolyID] = in_bdmMessage;

			break;
		}

		case MessageType::BDM_ORE_HEADER:
		{
			// read the ORE key to determine the appropriate key value to use in the reconstitutedOREMap, delete it, and then send the Message to the appropriate
			// area in the map.
			auto targetOREKey = in_bdmMessage.readEnclaveKey();
			in_bdmMessage.removeEnclaveKeyFromFrontAndResetIter(1);
			reconstitutedOREMap[targetOREKey].checkReconstitutedOREData(in_bdmMessage);

			break;
		}

		case MessageType::BDM_ORE_SKELETONSGM:
		{
			// read the ORE key to determine the appropriate key value to use in the reconstitutedOREMap, delete it, and then send the Message to the appropriate
			// area in the map.
			auto targetOREKey = in_bdmMessage.readEnclaveKey();
			in_bdmMessage.removeEnclaveKeyFromFrontAndResetIter(1);
			reconstitutedOREMap[targetOREKey].checkReconstitutedOREData(in_bdmMessage);

			break;
		}

		case MessageType::BDM_BLOCK_TAGGED:
		{
			// read the ORE key to determine the appropriate key value to use in the reconstitutedOREMap, delete it, and then send the Message to the appropriate
			// area in the map.
			auto targetOREKey = in_bdmMessage.readEnclaveKey();
			in_bdmMessage.removeEnclaveKeyFromFrontAndResetIter(1);
			reconstitutedOREMap[targetOREKey].checkReconstitutedOREData(in_bdmMessage);

			break;
		}

		case MessageType::BDM_SKELETONBLOCK_TAGGED:
		{
			auto targetOREKey = in_bdmMessage.readEnclaveKey();
			in_bdmMessage.removeEnclaveKeyFromFrontAndResetIter(1);
			reconstitutedOREMap[targetOREKey].checkReconstitutedOREData(in_bdmMessage);
			break;
		}
	}
}

void ReconstitutedBlueprint::printReconstitutedMetadata()
{
	for (auto& currentReconstitutedORE : reconstitutedOREMap)
	{
		EnclaveKeyDef::EnclaveKey currentOREKey = currentReconstitutedORE.first;
		currentOREKey.printKey();
		currentReconstitutedORE.second.printReconstitutedOREStats();
	}
	std::cout << "Number of Reconstituted ECBPolys: " << reconstitutedECBPolyMap.size() << std::endl;
	std::cout << "Number of Reconstituted OREs: " << reconstitutedOREMap.size() << std::endl;
	std::cout << "ORE stats: " << std::endl;
}

ReconstitutedBlueprintRunState ReconstitutedBlueprint::getReconstitutedState()
{
	return reconBlueprintState;
}

void ReconstitutedBlueprint::setStateAsWaitingToRun()
{
	reconBlueprintState = ReconstitutedBlueprintRunState::WAITING_FOR_RUN;
}

void ReconstitutedBlueprint::setStateAsFailedRun()
{
	reconBlueprintState = ReconstitutedBlueprintRunState::RECONSTITUTION_FAILURE;
}

void ReconstitutedBlueprint::setStateAsSuccessfulRun()
{
	reconBlueprintState = ReconstitutedBlueprintRunState::RECONSTITUTION_SUCCESS;
}

void ReconstitutedBlueprint::ReconstitutableORE::checkReconstitutedOREData(Message in_message)
{
	in_message.open();
	switch (in_message.messageType)
	{
		case MessageType::BDM_ORE_HEADER:
		{
			// send the Message to the reconstitutedOREHeader
			reconstitutedOREHeader.setReconstitutionMessage(in_message);
			break;
		}

		case MessageType::BDM_ORE_SKELETONSGM:
		{
			// send the Message to the reconstitutedSkeletonSGMHeader
			reconstitutedSkeletonSGMHeader.setReconstitutionMessage(in_message);
			break;
		}

		case MessageType::BDM_BLOCK_TAGGED:
		{
			// read the block key, then use that as the key to store the rest of the Message in the reconstitutedBlockMessages.
			auto targetBlocKey = in_message.readEnclaveKey();
			in_message.removeEnclaveKeyFromFrontAndResetIter(1);
			reconstitutedBlockMessages[targetBlocKey] = in_message;
			break;
		}

		case MessageType::BDM_SKELETONBLOCK_TAGGED:
		{
			// read the block key, then use that as the key to store the rest of the Message in the reconstitutedSkeletonMessages.
			auto targetBlocKey = in_message.readEnclaveKey();
			in_message.removeEnclaveKeyFromFrontAndResetIter(1);
			reconstitutedSkeletonMessages[targetBlocKey] = in_message;
			break;
		}
	}
}

void ReconstitutedBlueprint::ReconstitutableORE::printReconstitutedOREStats()
{
	if (reconstitutedOREHeader.reconstituted)
	{
		Message tempMessage = reconstitutedOREHeader.reconstitutionMessage;
		tempMessage.open();
		int currentORELodState = tempMessage.readInt();
		ORELodState tempState = ORELodState(currentORELodState);
		if (tempState == ORELodState::LOD_BLOCK)
		{
			std::cout << "Lod: LOD_BLOCK           | ";
		}
		else if (tempState == ORELodState::LOD_ENCLAVE_SMATTER)
		{
			std::cout << "Lod: LOD_ENCLAVE_SMATTER | ";
		}
		else if (tempState == ORELodState::FULL)
		{
			std::cout << "Lod: FULL                | ";
		}
		else
		{
			std::cout << "Lod: <UNKNOWN>           | ";
		}
	}

	std::cout << " Blocks: " << reconstitutedBlockMessages.size() << " | Skeletons: " << reconstitutedSkeletonMessages.size();

	if (reconstitutedSkeletonSGMHeader.reconstituted)
	{
		Message tempMessage = reconstitutedSkeletonSGMHeader.reconstitutionMessage;
		tempMessage.open();
		int totalSkeletons = tempMessage.readInt();

		std::cout << " | SGM entires: " << totalSkeletons << std::endl;
	}
	else
	{
		std::cout << " | **No Sgm entries found.** " << std::endl;
	}
}


void ReconstitutedBlueprint::ReconstitutableORE::runReconstitution(EnclaveKeyDef::EnclaveKey in_targetBlueprintKey,
																   EnclaveKeyDef::EnclaveKey in_targetOREKey,
																   std::mutex* in_generatedBlueprintMapMutex,
															       std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>* in_generatedBlueprintMapRef)
{
	// the below lock guard is to ensure that produced ORE can safely get loaded into the blueprint map.
	std::lock_guard<std::mutex> loadLock(*in_generatedBlueprintMapMutex);

	if (reconstitutedOREHeader.reconstituted)
	{
		Message tempMessage = reconstitutedOREHeader.reconstitutionMessage;
		tempMessage.open();
		int currentORELodState = tempMessage.readInt();
		ORELodState tempState = ORELodState(currentORELodState);

		OrganicRawEnclave producedORE;

		// dummy mutex for now, for inserting...realistically needs to be something else.
		std::mutex dummyTestMutex;

		switch (tempState)
		{
			// For an ORE in the LDO_BLOCK state, we must load:
			case ORELodState::LOD_BLOCK:
			{
				producedORE.reconstituteAsLodBlock(reconstitutedOREHeader.reconstitutionMessage,
												&reconstitutedBlockMessages,
												&reconstitutedSkeletonMessages);

				// Final step: insert the produced ORE into the referenced generatedBlueprints member, which comes from the ReconstitutionManager class.
				(*in_generatedBlueprintMapRef)[in_targetBlueprintKey].fractureResults.fractureResultsContainerMap[in_targetOREKey] = producedORE;

				break;
			}

			// For an ORE in the FULL state, we must load:
			case ORELodState::FULL:
			{
				producedORE.reconstituteAsFull(reconstitutedOREHeader.reconstitutionMessage,
											   &reconstitutedSkeletonMessages);

				// Final step: insert the produced ORE into the referenced generatedBlueprints member, which comes from the ReconstitutionManager class.
				(*in_generatedBlueprintMapRef)[in_targetBlueprintKey].fractureResults.fractureResultsContainerMap[in_targetOREKey] = producedORE;

				break;
			}

			// For an ORE in the LOD_ENCLAVE_SMATTER state
			case ORELodState::LOD_ENCLAVE_SMATTER:
			{
				if (reconstitutedSkeletonSGMHeader.reconstituted)
				{
					//std::cout << "Found reconstitutable LOD_ENCLAVE_SMATTER ORE, attempting to rebuild SGM. " << std::endl;

					producedORE.reconstituteAsSMatter(reconstitutedOREHeader.reconstitutionMessage,
													&reconstitutedSkeletonMessages,
													reconstitutedSkeletonSGMHeader.reconstitutionMessage);
													  
					(*in_generatedBlueprintMapRef)[in_targetBlueprintKey].fractureResults.fractureResultsContainerMap[in_targetOREKey] = producedORE;
				}

				break;
			}
		}
	}
}

void ReconstitutedBlueprint::runReconstitutionOnAllBDMPolyData(EnclaveKeyDef::EnclaveKey in_targetBlueprintKey, 
															   std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>* in_blueprintMapRef)
{
	int insertionCounter = 0;
	for (auto& currentBDMPolyMessage : reconstitutedECBPolyMap)
	{
		// Fetch the ECBPoly ID, which is stored as the key value in the map.
		int currentPolyID = currentBDMPolyMessage.first;

		// Use the actual Message (of the type, MessageType::BDM_BLUEPRINT_ECBPOLY) to initialize the ECBPoly.
		ECBPoly reconstrutedECBPoly(currentBDMPolyMessage.second);

		// Insert the results into the referenced map.
		//std::cout << "---> inserting ECBPoly, with ID: " << currentPolyID << std::endl;
		(*in_blueprintMapRef)[in_targetBlueprintKey].insertPolyWithKeyValue(currentPolyID, reconstrutedECBPoly);

		insertionCounter++;
	}

	/*
	std::cout << "insertionCounter value: " << insertionCounter << std::endl;
	std::cout << "(ReconstitutedBlueprint::runReconstitutionOnAllBDMPolyData) -> total number of ECBPolys reconstituted into referenced map: " << (*in_blueprintMapRef)[in_targetBlueprintKey].getPolygonMapSize() << std::endl;
	int totalProcessed = 3;
	std::cin >> totalProcessed;
	*/
}