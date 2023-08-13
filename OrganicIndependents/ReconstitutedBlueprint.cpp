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