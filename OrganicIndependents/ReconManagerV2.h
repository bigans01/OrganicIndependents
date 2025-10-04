#pragma once

#ifndef RECONMANAGERV2_H
#define RECONMANAGERV2_H

#include "ReconstitutedBlueprint.h"
#include "MessageContainer.h"
#include "EnclaveCollectionBlueprint.h"
#include "ThreadSafeQueue.h"
#include "ReconManagerWorkPipe.h"

class ReconManagerV2
{
	public:
		bool hasBlueprintsToProcess();	// should return true, if the ticketPipe has tickets in it; used by OrganicSystem to determine
										// whether or not a thread should go to sleep or be awake.

		int getNumberOfReadyBlueprints();	// thread-safe call that returns the number of blueprints in readyBlueprints.
											// Should be utilized by OrganicSystem::checkReconPipeline (OrganicCoreLib)

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> getReadyBlueprintKeys();	// thread-safe call that fetches an unordered_set of keys found
																											// in the ready blueprints map; the return value should be used to iterate through
																											// and call transferBlueprintOut for each iteration. Should be utilized by OrganicSystem::checkReconPipeline (OrganicCoreLib)

		void insertMessageContainerForProcessing(MessageContainer in_containerForProcessing);	// inserts a MessageContainer into the 
																								// container processing queue (processableContainers); must utilize a 
																								// lock_guard via processingContainerMutex when writing to processableContainers. 
																								// Should only be really called by a networking/main thread, separate from the dedicated blueprint thread,
																								// when implemented in the designed manner.

		void checkForNewContainerTickets();	// create new tickets that go into the ticketPipe, if there are messages in processableContainers AND the
											// value of remainingContainerTicketCounter is 0. Should be called by the main thread in an OrganicSystem 
											// to see if there are tickets to load.

		void checkForCompletedBlueprintsReadyToExport();

		void runManagerOnThread();	// starts the manager on a dedicated thread; sets the value of continueRunningFlag to True, so that the while loop continues until that value is False.
		void runPipeProcessingTick();	// allows for manually running of a tick; currently public for testing purposes in OrganicCoreLib,
										// but will need to move to private.

		bool getManagerRunState();
		void setManagerRunState(bool in_value);

		void haltReconstitutionLoop();		// sets the value of shouldReconstitutionLoopRun to 0; this should cause the
											// loop in ReconstitutionManager::runManagerOnThread() to halt during the call to 
											// checkForReconstitutionHalt(), thereby freeing up the CPU from this thread until it is resumed. 										

		void resumeReconstitutionLoop();	// sets the value of shouldReconstitutionLoopRun to 1; this will effectively allow the thread to resume
											// if it's waiting on the CV to be notified in the call to checkForReconstitutionHalt()


		// Below: this function should be called by the main thread of OrganicSystem, to transfer a blueprint out; 
		// the referenced map in mutex should be from an ECBMap object, in order to guarantee thread-safe transfer to that map.
		// This function should never be called by the thread that is running runManagerOnThread(), only the thread that consumes
		// blueprints (i.e., the main OrganicSystem thread)
		void transferBlueprintOut(EnclaveKeyDef::EnclaveKey in_transferKey,
			std::mutex* in_transferTargetMutex,
			std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher>* in_targetBlueprintMap
		);

		void printPipelineTickets();	// print all tickets currently in the pipeline.

		// debugging/misc
		void manuallyPushBlueprintMoveTicket();	// for testing purposes via OrganicCoreLib/OrganicSystem, this allows for a single 
												// RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK ticket to be put into the pipe.
	private:
		// the work pipe.
		ReconManagerWorkPipe ticketPipe;

		std::mutex readyBlueprintsMutex;	// safety: used when writing to / reading from readyBlueprints.
		std::mutex generatedSetMutex;	// safety: used when writing to / reading from generatedBPSet.
		std::mutex dockMutex;					// safety: should be used with any function that needs to access the reconstitutionDock.
		std::mutex generatedBlueprintsMutex;	// safety: should be used when accessing the generatedBlueprints map of this class.
		std::unordered_map<EnclaveKeyDef::EnclaveKey, ReconstitutedBlueprint, EnclaveKeyDef::KeyHasher> reconstitutionDock;	// an unordered_map map of ReconstitutedBlueprint instances,
																															// this is where Message instances handled by the call to handlebDMMessageForDockEntry
																															// end up getting sent to.

		std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher> generatedBlueprints;	// unordered map for generated blueprints;
																																	// this is the map where reconstiuted ECBPolys and OREs get appended/written/updated.

		std::unordered_map<EnclaveKeyDef::EnclaveKey, EnclaveCollectionBlueprint, EnclaveKeyDef::KeyHasher> readyBlueprints;


		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> generatedBPSet;

		// runtime related members
		std::mutex managerRuntimeMutex;		// safety: used when starting an instance of this class on a dedicated logging thread, and is used to check if the continueRunningFlag is set or not.
		bool continueRunningFlag = false;	// used to determine if the while loop in the call to 


		// loop halting members
		std::mutex reconPipelineLoopMutex;		// used to safely set the value of shouldReconstitutionLoopRun (and call notify on the CV)
		int shouldReconstitutionLoopRun = 1;	// when running the ReconstitutionManager on ReconstitutionManager::runManagerOnThread, 
												// the value of this will determine if the loop in this function halts when
												// checkForReconstitutionHalt is called; 0 will cause the thread to halt, 1 will allow it to continue.

		std::mutex reconstitutionLoopMutex;	   // mutex used when the CV needs to do waiting.
		std::condition_variable reconstitutionLoopCV;	// this CV gets waited on when haltReconstitutionLoop is called;
									                    // notify_all gets called on it when resumeReconstitutionLoop is called.

		ThreadSafeQueue<MessageContainer> processableContainers;	// a thread-safe queue of MessageContainers (which contain BDM) that will be processed by the call to executeContainerProcessing().


		void handlebDMMessageForDockEntry(EnclaveKeyDef::EnclaveKey in_targetBlueprintToLoad, Message in_messageDataCopy);	// thread safe; sends a BDM-style Message to the appropriate blueprint in the dock, 
																															// for processing.

		void checkForReconstitutionHalt();
		int getReconstitutionLoopRunValue();

		void processAndReconstituteNextContainer();	// process/reconstitute exactly one blueprint, from a MessageContainer.
		void attemptReconstitution(EnclaveKeyDef::EnclaveKey in_blueprintToReconstitute);	// attempts to fully reconstitute a blueprint (i.e, all ECBPolys and OREs). 
																								// For this to work successfully, the following must occur:
																								//
																								//	1. The target dock must already exist, and have a valid MessageType::BDM_BLUEPRINT_HEADER in its reconBlueprintHeader
																								//	2. The BDM_BLUEPRINT_HEADER must be stripped of its blueprint key, and contain the number of ECBPolys and ORES (they must be read in in that order)
																								//	3. The number of produced ECBPoly and ORE instances must matched the values that were read in from the BDM_BLUEPRINT_HEADER.
																								//	4. The value of reconBlueprintState in the ReconstitutedBlueprint being looked at must be WAITING_FOR_RUN.
																								//
																								//	At the end of the reconstitution attempt, this function will set the reconBlueprintState to RECONSTITUTION_SUCCESS in the event of a successful reconstitution,
																								//	or RECONSTITUTION_FAILURE if the process fails.
		

		EnclaveKeyDef::EnclaveKey processMessageContainer(MessageContainer* in_messageContainer);	// called by executeContainerProcessing() 

		// functions for reading, writing, and checking the generatedBPSet member.
		void insertGeneratedBPKey(EnclaveKeyDef::EnclaveKey in_keyToInsert);
		bool doAnyGeneratedBPKeysExist();
		EnclaveKeyDef::EnclaveKey fetchFirstBPKey();
		void eraseBPKey(EnclaveKeyDef::EnclaveKey in_keyToErase);
		void printBPKeys();
		int getBPKeyCount();

		// access methods/declarations for remainingContainerTicketCounter; this counter should get incremented by a certain amount, if the check in 
		// checkForNewContainerTickets is successful, and decremented by one each time a PROCESS_BLUEPRINTS ticket is processed.
		std::mutex ticketCounterMutex;
		int remainingContainerTicketCounter = 0;
		void incrementTicketCounter(int in_valueToAdd);
		void decrementTicketCounter();
		int getTicketCounterValue();

		void moveGeneratedBlueprintsToReady();	// transfer blueprints from generatedBlueprints to readyBlueprints; should only ever be called by
												// the runManagerOnThread function, when operating on a ticket that is RMWorkPipeTicketEnum::MOVE_BLUEPRINTS_FROM_DOCK.
		
		
		// misc. test variables
		bool manualMovePushCalled = false;
};

#endif
