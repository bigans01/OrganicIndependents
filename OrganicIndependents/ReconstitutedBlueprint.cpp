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
	std::cout << "Number of Reconstituted ECBPolys: " << reconstitutedECBPolyMap.size() << std::endl;
	std::cout << "Number of Reconstituted OREs: " << reconstitutedOREMap.size() << std::endl;
	std::cout << "ORE stats: " << std::endl;
	for (auto& currentReconstitutedORE : reconstitutedOREMap)
	{
		EnclaveKeyDef::EnclaveKey currentOREKey = currentReconstitutedORE.first;
		currentOREKey.printKey();
		currentReconstitutedORE.second.printReconstitutedOREStats();
	}
}