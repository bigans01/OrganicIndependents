#pragma once

#ifndef BLUEPRINTSYNCTRACKER_H
#define BLUEPRINTSYNCTRACKER_H

#include "BlueprintState.h"
#include <chrono>

/*

Description:

This class is designed to keep track of when a blueprint was last updated (i.e, had a block adde/deleted) and when it was utilized (i.e, actually used for rendering or an operation).
The timestamps of this class in various manneers, according to the enum value passed to setSyncState.

The lastUtilized timestamp is key to mainly determine how long a blueprint has been "out of play" (i.e, loaded but unused); blueprints that are "out of play" should 
be considered candiates for garbage collection. Therefore, the lastUtilized timestamp is needed when an OrganicSystem does its garbage collection checks for blueprints.

*/

class BlueprintSyncTracker
{
	public:
		void setSyncState(BlueprintState in_blueprintState);	// sets the sync state and timestamps appropriately; should be called by the EnclaveColelctionBlueprint's constructor when it is built,
																// with a value of FIRST_LOAD, to signify the initial state that the blueprint is in.
		BlueprintState getSyncState();
	private:
		std::chrono::time_point<std::chrono::steady_clock> lastUtilized;	// represents the last time the blueprint was used for something, (i.e, loaded for rendering)
		std::chrono::time_point<std::chrono::steady_clock> lastUpdated;		// the last time something in the blueprint was updated, such as deleting a block or changing GL data/normals, etc.
		BlueprintState bpSyncState = BlueprintState::FIRST_LOAD;	// default value, but should be able to be changed in other ways (i.e, BP_SYNC)
};

#endif
