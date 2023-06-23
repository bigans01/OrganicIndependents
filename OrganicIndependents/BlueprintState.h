#pragma once

#ifndef BLUEPRINTSTATE_H
#define BLUEPRINTSTATE_H

enum class BlueprintState
{
	REQUEST_PENDING,	// used to signify that the blueprint doesn't exist, but a request was made to the correspoding OrganicServer instance.
	LOAD_PENDING,		// used when a blueprint is being loaded, but is not yet ready, in an OrganicSystem.
	UPDATE_PENDING,		// used to signify that an existing, loaded blueprint is being updated (i.e, deleting a block, modifying an ORE)
	FIRST_LOAD,			// indicates that the blueprint is "freshly loaded" and ready, but has not been processsed/rendered/etc. This should be the default value
						// when a blueprint is first loaded.

	BP_UNSYNC_REMOTE,	// For a blueprint loaded in a remote mode, this indicates that the lastUtilized and lastUpdated timestamps of a blueprint's BlueprintSyncTracker are not matching;
						
	BP_UNSYNC_LOCAL,	// For a blueprint loaded in a local mode, this indicates that the lastUtilized and lastUpdated timestamps of a blueprint's BlueprintSyncTracker are not matching;

	BP_SYNC_REMOTE,		// Indicates that the blueprint is loaded in a remote manner, AND it is sync'd.
						
	BP_SYNC_LOCAL		// Indicates that the blueprint is loaded in a local manner, AND it is sync'd.
};

#endif