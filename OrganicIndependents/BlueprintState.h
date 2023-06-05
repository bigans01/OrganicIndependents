#pragma once

#ifndef BLUEPRINTSTATE_H
#define BLUEPRINTSTATE_H

enum class BlueprintState
{
	REQUEST_PENDING,	// used to signify that the blueprint doesn't exist, but an entry has been put into the BlueprintRequestManager.
	UPDATE_PENDING,		// used when a blueprint is being loaded, but is not yet ready, in an OrganicSystem.
	FIRST_LOAD,			// indicates that the blueprint is "freshly loaded" and ready, but has not been processsed/rendered/etc. This is the default value
						// in the 

	BP_UNSYNC,			// indicates that the lastUtilized and lastUpdated timestamps of a blueprint's BlueprintSyncTracker are not matching;
						// this would mean that an update has been made to the blueprint, such as a block delete, but that it hasn't been re-rendered/processed since then.

	BP_SYNC,			// indicates that the blueprint's lastUpdated and lastUitilzed values are matching; we shouldn't have to reprocess the entire blueprint, just send the 
						// rendered data.
};

#endif