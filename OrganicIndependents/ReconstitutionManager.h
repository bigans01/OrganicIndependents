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

*/

class ReconstitutionManager
{
	public:
		void insertMessageContainerForProcessing(MessageContainer in_containerForProcessing);	// inserts a MessageContainer into the 
																								// container processing queue (processableContainers)
		void executeContainerProcessing();	// attempt to process any MessageContainer instances in the processableContainers member; MessageContainer instances
											// are discarded after they are read, via queue pop.
	private:
		std::unordered_map<EnclaveKeyDef::EnclaveKey, ReconstitutedBlueprint, EnclaveKeyDef::KeyHasher> reconstitutionDock;
		std::queue<MessageContainer> processableContainers;
		void processMessageContainer(MessageContainer* in_messageContainer);
};

#endif
