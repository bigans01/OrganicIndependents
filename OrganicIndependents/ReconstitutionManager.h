#pragma once

#ifndef RECONSTITUTIONMANAGER_H
#define RECONSTITUTIONMANAGER_H

#include "ReconstitutedBlueprint.h"
#include "MessageContainer.h"

/*

Description: This class processes MessageContainer instances, in order to put them into the correct ReconstitutedBlueprint instance that exists in 
the reconstitutionDock.

In the future, this class will need to be able to check for which ReconstitutedBlueprint instances in the map have been completed, and be 
able to put these into a separate map that indicates that they are ready for "pick up" or "export."

This class assumes that all Message objects in each MessageContainer have a blueprint key sitting at the front of the Message; this key must
be read, and then stripped, to determine where the Message will go. See this methodology in the function definition of processMessageContainer of this class.

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
	private:
		std::unordered_map<EnclaveKeyDef::EnclaveKey, ReconstitutedBlueprint, EnclaveKeyDef::KeyHasher> reconstitutionDock;
		std::queue<MessageContainer> processableContainers;
		void processMessageContainer(MessageContainer* in_messageContainer);
};

#endif
