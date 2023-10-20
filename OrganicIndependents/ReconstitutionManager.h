#pragma once

#ifndef RECONSTITUTIONMANAGER_H
#define RECONSTITUTIONMANAGER_H

#include "ReconstitutedBlueprint.h"
#include "MessageContainer.h"
#include "EnclaveCollectionBlueprint.h"
#include "ReconBlueprintRunmodeRequest.h"
#include "ReconBlueprintRunmode.h"

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

		// ||||||||||||||||||| START: PROCESSING mode functions -- when running this instance on a dedicated thread, these should only be called 
		// when the value of currentRunMode is set to ReconBlueprintRunmode::PROCESSING.
																	
		void executeContainerProcessing();	// attempt to process any MessageContainer instances in the processableContainers member; MessageContainer instances
											// are discarded after they are read, via queue pop. All Message instances to process must have a blueprint key that can be 
											// read and then stripped, to determine where each Message in the containers will go. When this class truly functions in the intended manner,
											// this function should only be called by the dedicated blueprint thread.

		void checkForReconstitutableBlueprints();	// Should be called after executeContainerProcessing() function above, by the dedicated blueprint thread; this function
													// will attempt to find blueprints in the reconstitutionDock that ready to be processed and output to the generatedBlueprints map.

		void attemptFullReconstitution(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey);	// attempts to fully reconstitute a blueprint (i.e, all ECBPolys and OREs). 
																								// For this to work successfully, the following must occur:
																								//
																								//	1. The target dock must already exist, and have a valid MessageType::BDM_BLUEPRINT_HEADER in its reconBlueprintHeader
																								//	2. The BDM_BLUEPRINT_HEADER must be stripped of its blueprint key, and contain the number of ECBPolys and ORES (they must be read in in that order)
																								//	3. The number of produced ECBPoly and ORE instances must matched the values that were read in from the BDM_BLUEPRINT_HEADER.
																								//	4. The value of reconBlueprintState in the ReconstitutedBlueprint being looked at must be WAITING_FOR_RUN.
																								//
																								//	At the end of the reconstitution attempt, this function will set the reconBlueprintState to RECONSTITUTION_SUCCESS in the event of a successful reconstitution,
																								//	or RECONSTITUTION_FAILURE if the process fails.

		void processMessageContainersAndCheckForReconstitutables();	// calls the executeContainerProcessing() function, and then checkForReconstitutableBlueprints(), for any blueprints to process.
																	// All entries in the reconstitutionDock that have a value of WAITING_FOR_RUN for the reconBlueprintState will undergo an attempted
																	// reconstitution. In it's true implementation, only the blueprint processing thread should be calling this function at a regular interval.
		// ||||||||||||||||||| END: PROCESSING mode functions




		// ||||||||||||||||||| START: RETRIEVAL mode functions -- when running this instance on a dedicated thread, these should only be called 
		// when the value of currentRunMode is set to ReconBlueprintRunmode::PROCESSING.
		bool isReconstitutedBlueprintReadyForTransfer(EnclaveKeyDef::EnclaveKey in_blueprintToCheck);	// checks to see if a blueprint is ready for transfer; returns false if
																									    // the blueprint doesn't exist, or it's not ready.

		EnclaveCollectionBlueprint* fetchReconstitutedBlueprintRef(EnclaveKeyDef::EnclaveKey in_blueprintKeyForFetching);	// fetches a reference to an existing EnclaveCollectionBlueprint
																															// that is sitting in the generatedBlueprintsMap. Used by the function,
																															// OrganicSystem::checkForReconstitutedBlueprints(), in OrganicCoreLib.

		EnclaveKeyDef::EnclaveKey fetchFirstAvailableReconstitutedBlueprintKey();	// scans the reconstitutionDock for the first available successful key. Should only be used after a call to doAnySuccessfulReconstitutionsExist,
																					// which signifies that there are valid keys to fetch. Used by the function, OrganicSystem::checkForReconstitutedBlueprints(), in OrganicCoreLib.
																					
		void eraseReconstitutedBlueprint(EnclaveKeyDef::EnclaveKey in_blueprintKeyForErasing);	// erases a blueprint from generatedBlueprintMap, and from the dock itself. Should be used after a blueprint has been copied by
																								// an OrganicSystem instance. Used by the function, OrganicSystem::checkForReconstitutedBlueprints(), in OrganicCoreLib.

		// |||||||||||||||||||  END: RETRIEVAL mode functions



		// |||||||||||||||||||| START: Debug functions (use cautiously, not thread safe when an instance of this class is running on a dedicated thread.
		void printReconstitutedBlueprintStats(EnclaveKeyDef::EnclaveKey in_blueprintStatsToFetch);	// prints out metadata about a ReconstitutedBlueprint in the reconstitutionDock.

		void runOREReconstitutionSpecific(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey, EnclaveKeyDef::EnclaveKey in_containingOREKey);	// run the reconstitution for a specicic ORE,
																																				// and then add the ORE to the correct location in the generatedBlueprints.

		OrganicRawEnclave fetchReconstitutedORE(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey, EnclaveKeyDef::EnclaveKey in_containingOREKey);	// fetches a reconstituted ORE; 
																																						// doesn't check if it exists, so use cautiously.

		EnclaveCollectionBlueprint fetchReconstitutedBlueprint(EnclaveKeyDef::EnclaveKey in_containingBlueprintKey);	// gets a copy of a reconstituted blueprint; will not do a safety check to make 
																														// sure it got built correctly, so use with caution.
		// |||||||||||||||||||| END: Debug functions

		bool doAnySuccessfulReconstitutionsExist();	// (thread safe) checks the dock for successful runs, which indicates that a blueprint is ready for extraction. Used by external threads
													// to check when it is time to change the mode of an instance of this class when it is running on a dedicated thread. Used by the function, 
													// OrganicSystem::checkForReconstitutedBlueprints(), in OrganicCoreLib.


		void runManagerOnThread();	// starts the manager on a dedicated thread; sets the value of continueRunningFlag to True, so that the while loop continues until that value is False.
		bool getManagerRunState();
		void setManagerRunState(bool in_value);

		void setRunModeRequest(ReconBlueprintRunmodeRequest in_runmodeRequest);		// (thread safe) primarily used by threads outside of the one that this instance runs on, so that that thread can make 
																					// various runmode requests on how this instance should run. A call to this function will directly affect how the determineRunMode()
																					// will work the next time it is called (after this function call). In other words, this is a "switch" or "signal" that is used to 
																					// determine if the currentRunMode will be value such as PROCESSING or RETRIEVAL.

		ReconBlueprintRunmode getRunMode();	// (thread safe) used by external threads to get the run mode of this instance; used by the dedicated thread running this instance
											// to check the current run mode of the instance, at the end of each tick in the while loop of the runManagerOnThread() function. 
											// Not dependent on the value of currentRunMode -- can be called at any time.


	private:
		std::mutex processingContainerMutex;	// safety: used when writing to processableContainers (by the networking thread), and reading from it (for the dedicated blueprint thread)
		std::mutex dockMutex;					// safety: should be used with any function that needs to access the reconstitutionDock.
		std::mutex generatedBlueprintsMutex;	// safety: should be used when accessing the generatedBlueprints map of this class.
		
		// runtime related members
		std::mutex managerRuntimeMutex;		// safety: used when starting an instance of this class on a dedicated logging thread, and is used to check if the continueRunningFlag is set or not.
		bool continueRunningFlag = false;	// used to determine if the while loop in the call to 

		// runmode request members
		std::mutex managerRunmodeRequestMutex;
		ReconBlueprintRunmodeRequest currentRequest = ReconBlueprintRunmodeRequest::RUN_IN_PROCESSING_MODE;	// the value of currentRequest can be set by a call to setRunModeRequest by an external thread, 
																											// which is a thread safe call. The value of this member is checked during a call to determineRunMode(),
																											// which is used in turn to determine the currentRunMode -- which determines the kind of work the corresponding
																											// instance will do in the next tick of runManagerOnThread().

		// runmode members
		std::mutex managerRunmodeMutex;	// safety: used for setting/getting values of currentRunMode in a secure thread-safe manner.
		ReconBlueprintRunmode currentRunMode = ReconBlueprintRunmode::PROCESSING;	// when running this instance via a call to runManagerOnThread(), this value determines
																					// how the instance should function on the next tick of the while loop in that function. 
																					// For instance: 
																					//
																					// --a value of PROCESSING will indicate that the instance will constantly check for work to do, and do it accordingly.
																					//	 Querying the contents of the dock should not be permissible in this state.
																					//
																					// --a value of RETRIEVAL means that there is no processing being done, and that an external thread will be 
																					//	 free to query, retrieve, or delete data in the dock (i.e, fetching a reference to an entirely completed blueprint, if it's ready)
																					//
																					// So for instance, if an external thread (NOT the thread that this instance runs on) wants to check on the state
																					// of a blueprint, but then let the processing continue, it would have to do the following:
																					//
																					// 1.) Call setRunModeRequest, with a value of ReconBlueprintRunmodeRequest::RUN_IN_RETRIEVAL_MODE. 
																					//	   This will cause the while loop in runManagerOnThread() to set the run mode to RETRIEVAL, when it calls
																					//	   determineRunMode() at the end of the next while loop tick (that is, if it wasn't set to RETRIEVAL already).
																					//
																					// 2.) Call getRunMode(), to see if the value of currentRunMode is ReconBlueprintRunMode::RETRIEVAL. If it isn't, 
																					//	   don't bother continuing. But if it is the mode we want, continue.
																					//
																					// 3.) If the value of getRunMode() is ReconBlueprintRunMode::RETRIEVAL, we know we are good to go to query/copy/erase.
																					//
																					// 4.) When the external thread is done with whatever it's doing, it needs to call setRunModeRequest again, with value of
																					//	   ReconBlueprintRunmodeRequest::RUN_IN_PROCESSING_MODE, so that the dedicated processing thread can continue its work.
		

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
		ReconBlueprintRunmodeRequest getRunModeRequest();
		void determineRunMode();	// analyzes the value of the currentRequest member, to determine the currentRunMode that this instance should run in, during 
									// the next tick in the while loop of runManagerOnThread(). This is directly affected by a call to setRunModeRequest
		void setRunMode(ReconBlueprintRunmode in_currentRunMode);
};

#endif
