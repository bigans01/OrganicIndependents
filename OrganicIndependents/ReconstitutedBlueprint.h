#pragma once

#ifndef RECONSTITUTEDBLUEPRINT_H
#define RECONSTITUTEDBLUEPRINT_H

#include "EnclaveCollectionBlueprint.h"
#include "OrganicRawEnclave.h"
#include "PolyUtils.h"

/*

Description:

This class is designed to store BDM Message types pertaining to a specific blueprint, that can later be used to rebuild
the entire blueprint, when the correct conditions are met.

All messages that are sent to handleBDMMessage MUST already have their blueprint key that sits of the BDM Message already stripped off
(this should be done by the parent ReconstitutionManager instance).



*/

class ReconstitutedBlueprint
{
	public:
		ReconstitutedBlueprint() {};
		void handleBDMMessage(Message in_bdmMessage);	// read a Message, to determine where it goes; remember, the blueprint key at the front of the BDM Message 
														// must already be stripped off, by the parent ReconstitutionManager instance.
		void printReconstitutedMetadata();	// print out metadata about this instance of ReconstitutedBlueprint, such as the number of ECBPolys,
							// the number of OREs, etc; it will also call ReconstitutableORE::printReconstitutedOREStats() for all associated OREs
							// in reconstitutedOREMap.
																																			
	private:
		friend class ReconstitutionManager;
		struct ReconstitutedMessageHeader
		{
			bool reconstituted = false;
			Message reconstitutionMessage;

			void setReconstitutionMessage(Message in_reconstitutionDataMessage)
			{
				reconstitutionMessage = in_reconstitutionDataMessage;
				reconstituted = true;
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
			// The Messages analyzed in this function should have the ORE key that sits at the front of the Message already stripped (which is done in the call to handleBDMMessage) 
			// before being passed as the argument to this function.
			void checkReconstitutedOREData(Message in_message);

			void printReconstitutedOREStats();

			// Below: this function attempts to reconstitute an ORE, thusly creating a new ORE that has all the necessary data it needs to operate correctly.
			// The resulting ORE is then inserted into the appropriate EnclaveCollectionBlueprint in the referenced map.
			void runReconstitution(EnclaveKeyDef::EnclaveKey in_targetBlueprintKey,
									EnclaveKeyDef::EnclaveKey in_targetOREKey, 
									std::mutex* in_generatedBlueprintMapMutex,
									std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>* in_generatedBlueprintMapRef);

		};

		ReconstitutedMessageHeader reconBlueprintHeader;	// reserved for messages of the type BDM_BLUEPRINT_HEADER.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, ReconstitutableORE, EnclaveKeyDef::KeyHasher> reconstitutedOREMap;	// stores the individual reconstituted OREs.
		std::map<int, Message> reconstitutedECBPolyMap;		// stores Messages that would be used to reconstitute an ECBPoly.

		// Below: read from all the ECBPoly BDM Messages in the reconstitutedECBPolyMap, to rebuild each ECBPoly and place it into the referenced map.
		void runReconstitutionOnAllBDMPolyData(EnclaveKeyDef::EnclaveKey in_targetBlueprintKey, 
											   std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>* in_blueprintMapRef);
};

#endif
