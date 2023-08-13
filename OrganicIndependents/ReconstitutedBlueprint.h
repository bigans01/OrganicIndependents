#pragma once

#ifndef RECONSTITUTEDBLUEPRINT_H
#define RECONSTITUTEDBLUEPRINT_H

#include "EnclaveCollectionBlueprint.h"
#include "OrganicRawEnclave.h"

/*

Description:

This class is designed to store BDM Message types pertaining to a specific blueprint, that can later be used to rebuild
the entire blueprint, when the correct conditions are met.

All messages that are sent to handleBDMMessage MUST already have their blueprint key that sits of the BDM Message already stripped off.

*/

class ReconstitutedBlueprint
{
	public:
		ReconstitutedBlueprint() {};
		void handleBDMMessage(Message in_bdmMessage);	// read a Message, to determine where it goes; remember, the blueprint key at the front of the BDM Message 
														// must already be stripped off.
	private:

		struct ReconstitutedMessageHeader
		{
			bool reconstituted = false;
			Message reconstitutionMessage;

			void setReconstitutionMessage(Message in_reconstitutionDataMessage)
			{
				reconstitutionMessage = in_reconstitutionDataMessage;
			};
		};

		struct ReconstitutableORE
		{
			ReconstitutedMessageHeader reconstitutedOREHeader;	// reservred for messages of the type BDM_ORE_HEADER
			ReconstitutedMessageHeader reconstitutedSkeletonSGMHeader;	// reserved for messages of the type BDM_ORE_SKELETONSGM

			// Below: reserved for Messages of the type BDM_BLOCK_TAGGED format;
			// the front two keys (blueprint, ORE) will need to be stripped, in order to get to the block key, to put it here.
			std::unordered_map<EnclaveKeyDef::EnclaveKey, Message, EnclaveKeyDef::KeyHasher> reconstitutedBlockMessages; 

			// Below: reserved for Messages of the type BDM_SKELETONBLOCK_TAGGED format;
			// the front two keys (blueprint, ORE) will need to be stripped, in order to get to the block key, to put it here.
			std::unordered_map<EnclaveKeyDef::EnclaveKey, Message, EnclaveKeyDef::KeyHasher> reconstitutedSkeletonMessages;
																														
			OrganicRawEnclave reconstitutedORE;	// the ORE that will eventually be produced by this class, which will then be put into the EnclaveCollectionBlueprint of this class.

			// Below: this attempts to take a Message and put it in the appropriate place. 
			// The Messages analyzed in this function should have the ORE key that sits at the front of the Message already stripped before being 
			// passed as the argument to this function.
			void checkReconstitutedOREData(Message in_message)	
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
		};

		ReconstitutedMessageHeader reconBlueprintHeader;	// reserved for messages of the type BDM_BLUEPRINT_HEADER.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, ReconstitutableORE, EnclaveKeyDef::KeyHasher> reconstitutedOREMap;	// stores the individual reconstituted OREs.
};

#endif
