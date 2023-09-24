#pragma once

#ifndef RECONSTITUTIONMANAGER_H
#define RECONSTITUTIONMANAGER_H

#include "ReconstitutedBlueprint.h"
#include "MessageContainer.h"
#include "EnclaveCollectionBlueprint.h"

/*

Description: This class processes MessageContainer instances, in order to put them into the correct ReconstitutedBlueprint instance that exists in 
the reconstitutionDock.

In the future, this class will need to be able to check for which ReconstitutedBlueprint instances in the map have been completed, and be 
able to put these into a separate map that indicates that they are ready for "pick up" or "export."

This class assumes that all Message objects in each MessageContainer have a blueprint key sitting at the front of the Message; this key must
be read, and then stripped, to determine where the Message will go. See this methodology in the function definition of processMessageContainer of this class.

This class is also responsible for generating a ReconstitutedBlueprint in it's entirety, or reconstituting an individual ORE via fetchReconstitutedORE.
The generatedBlueprints member of this class effectively serves as the "output" of reconstitution operations; in other words, the "baked" blueprints
formed as a result of reconstitution operatinos will be put into this map.

*/

class ReconstitutionManager
{
	public:
		void insertMessageContainerForProcessing(MessageContainer in_containerForProcessing);	// inserts a MessageContainer into the 
																								// container processing queue (processableContainers)
		void executeContainerProcessing();	// attempt to process any MessageContainer instances in the processableContainers member; MessageContainer instances
											// are discarded after they are read, via queue pop. All Message instances to process must have a blueprint key that can be 
											// read and then stripped, to determine where each Message in the containers will go.

		void printReconstitutedBlueprintStats(EnclaveKeyDef::EnclaveKey in_blueprintStatsToFetch);	// prints out metadata about a ReconstitutedBlueprint in the reconstitutionDock.

		void runOREReconstitutionSpecific(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey, EnclaveKeyDef::EnclaveKey in_containingOREKey);	// run the reconstitution for a specicic ORE,
																																				// and then add the ORE to the correct location in the generatedBlueprints.

		OrganicRawEnclave fetchReconstitutedORE(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey, EnclaveKeyDef::EnclaveKey in_containingOREKey);	// fetches a reconstituted ORE; 
																																						// doesn't check if it exists, so use cautiously.
	private:
		std::mutex dockMutex;					// safety: should be used with any function that needs to access the reconstitutionDock.
		std::mutex generatedBlueprintsMutex;	// safety: should be used when accessing the generatedBlueprints map of this class.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, ReconstitutedBlueprint, EnclaveKeyDef::KeyHasher> reconstitutionDock;	// an unordered_map map of ReconstitutedBlueprint instances,
																															// this is where Message instances handled by the call to handlebDMMessageForDockEntry
																															// end up getting sent to.

		std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher> generatedBlueprints;	// unordered map for generated blueprints;
																																	// this is the map where reconstiuted ECBPolys and OREs get appended/written/updated.

		std::queue<MessageContainer> processableContainers;		// a queue of MessageContainers (which contain BDM) that will be processed by the call to executeContainerProcessing().


		void processMessageContainer(MessageContainer* in_messageContainer);

		// critical, thread-safe functions
		bool doesBlueprintExistInDock(EnclaveKeyDef::EnclaveKey in_blueprintToCheck);	// thread safe check to see if a blueprint exists in the dock. 
		bool doesOREExistInDock(EnclaveKeyDef::EnclaveKey in_blueprintToCheck, EnclaveKeyDef::EnclaveKey in_OREToCheck);	// thread safe check to see if a blueprint exists AND
																															// contains a specific ORE.

		void handlebDMMessageForDockEntry(EnclaveKeyDef::EnclaveKey in_targetBlueprintToLoad, Message in_messageDataCopy);	// thread safe; sends a BDM-style Message to the appropriate blueprint in the dock, 
																															// for processing.
};

#endif
