#include "stdafx.h"
#include "EnclaveTriangleContainerSupergroupManager.h"

Message EnclaveTriangleSkeletonSupergroupManager::convertSkeletonSGMToBDM(EnclaveKeyDef::EnclaveKey in_blueprintKey, 
																		  EnclaveKeyDef::EnclaveKey in_oreKey)	// converts the contents of this instance to a Message of the type BDM_ORE_SKELETONSGM.
{
	Message etcSGMToMsg(MessageType::BDM_ORE_SKELETONSGM);

	// Each individual skeleton needs 3 things:
	// -the supergroupID
	// -the skeletonContainer ID
	// -the ID of the skeleton itself within the skeletonContainer.
	//
	// To do this, we must do a triple iterator loop. Keep track of the number of skeletons there are as well. The 
	// number of skeletons needs to be inserted back into the Message, so that any function that will read this Message type
	// can loop accordingly.
	int totalNumberOfSkeletons = 0;
	for (auto& currentSuperGroup : triangleSkeletonSupergroups)
	{
		for (auto& currentSkeletonContainer : currentSuperGroup.second.skeletonMap)
		{
			for (auto& currentSkeleton : currentSkeletonContainer.second.skeletons)
			{
				// The next four lines build all the metadata needed to build each EnclaveTriangleSkeleton;
				// If a function is to read a Message of the type BDM_ORE_SKELETONSGM, it must loop an 
				// amount of times equal to the number of skeletons. In other words, all the skeleton must be read from the Message -- and that data ONLY --
				// during the course of the loop.
				//
				// See the function, EnclaveCollectionBlueprint::printBDMForORESkeletonSGM, as a framework for how to appropriately read the data from this 
				// Message type.
				//
				// Before we add the skeleton data, append the specific index values (supergroupID, skeletonContainerID, the ID of the skeleton itself)
				etcSGMToMsg.insertInt(currentSuperGroup.first);
				etcSGMToMsg.insertInt(currentSkeletonContainer.first);
				etcSGMToMsg.insertInt(currentSkeleton.first);

				// Build the skeleton message data, and append it to the current etcSGMToMsg.
				etcSGMToMsg.appendOtherMessage(&currentSkeleton.second.convertETSkeletonToMessage());

				totalNumberOfSkeletons++;
			}
		}
	}

	// After we have acquired all the skeletons and sent them into the message, we must put the following into the 
	// front of the message, in this order:
	// 
	// -total number of skeletons
	// -the key of the ORE 
	// -the blueprint key.
	etcSGMToMsg.insertIntFront(totalNumberOfSkeletons);
	etcSGMToMsg.insertEnclaveKeyFront(in_oreKey);
	etcSGMToMsg.insertEnclaveKeyFront(in_blueprintKey);

	return etcSGMToMsg;
}