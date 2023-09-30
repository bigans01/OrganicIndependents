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

The basic cycle of operations can be described below:

1.	A dedicated network thread, running in parallel to another system, will call insertMessageContainerForProcessing to insert incoming MessageContainer instances,
	whenever it encounters a BDM Message/MessageContainer. This function must utilize a lock_guard that is meant to protect the processableContainers member when it is
	being written to, since the dedicated blueprint processing thread will have to periodically access this queue.

2.	The dedicated blueprint thread that eventually runs, will need to access the processableContainers member, whenever it calls the executeContainerProcessing.
	It will need to hold the lock_guard until it either A.) processes all MessageContainers in the processableContainers queue or B.) hits a Message processing limit, 
	when working with the processableContainers member, so that the thread doesn't  hold the lock too long to a point that the network thread gets backed up.

3.	After the ReconstitutionManager instance has called executeContainerProcessing, it will then have to check if any blueprints are ready to be reconstituted fully (not just OREs).
	The executeContainerProcessing() and checkForReconstitutableBlueprints() functions will have to be called back to back, which should be done via the call to
	processMessageContainersAndCheckForReconstitutables.
	
*/

class ReconstitutionManager
{
	public:
		void insertMessageContainerForProcessing(MessageContainer in_containerForProcessing);	// inserts a MessageContainer into the 
																								// container processing queue (processableContainers); must utilize a 
																								// lock_guard via processingContainerMutex when writing to processableContainers. 
																								// Should only be really called by a networking thread, separate from the dedicated blueprint thread,
																								// when implemented in the designed manner.

		void executeContainerProcessing();	// attempt to process any MessageContainer instances in the processableContainers member; MessageContainer instances
											// are discarded after they are read, via queue pop. All Message instances to process must have a blueprint key that can be 
											// read and then stripped, to determine where each Message in the containers will go. When this class truly functions in the intended manner,
											// this function should only be called by the dedicated blueprint thread.

		void checkForReconstitutableBlueprints();	// Should be called after executeContainerProcessing() function above, by the dedicated blueprint thread; this function
													// will attempt to find blueprints in the reconstitutionDock that ready to be processed and output to the generatedBlueprints map.

		void processMessageContainersAndCheckForReconstitutables();	// calls the executeContainerProcessing() function, and then checkForReconstitutableBlueprints(), for any blueprints to process.

		void printReconstitutedBlueprintStats(EnclaveKeyDef::EnclaveKey in_blueprintStatsToFetch);	// prints out metadata about a ReconstitutedBlueprint in the reconstitutionDock.

		void runOREReconstitutionSpecific(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey, EnclaveKeyDef::EnclaveKey in_containingOREKey);	// run the reconstitution for a specicic ORE,
																																				// and then add the ORE to the correct location in the generatedBlueprints.

		void attemptFullReconstitution(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey);	// attempts to fully reconstitute a blueprint (i.e, all ECBPolys and OREs). 
																								// For this to work successfully, the following must occur:
																								//
																								//	1. The target dock must already exist, and have a valid MessageType::BDM_BLUEPRINT_HEADER in its reconBlueprintHeader
																								//	2. The BDM_BLUEPRINT_HEADER must be stripped of its blueprint key, and contain the number of ECBPolys and ORES (they must be read in in that order)
																								//	3. The number of produced ECBPoly and ORE instances must matched the values that were read in from the BDM_BLUEPRINT_HEADER.

		OrganicRawEnclave fetchReconstitutedORE(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey, EnclaveKeyDef::EnclaveKey in_containingOREKey);	// fetches a reconstituted ORE; 
																																						// doesn't check if it exists, so use cautiously.

		EnclaveCollectionBlueprint fetchReconstitutedBlueprint(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey);	// gets a copy of a reconstituted blueprint; will not do a safety check to make 
																														// sure it got built correctly, so use with caution.
	private:
		std::mutex processingContainerMutex;	// safety: used when writing to processableContainers (by the networking thread), and reading from it (for the dedicated blueprint thread)
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
