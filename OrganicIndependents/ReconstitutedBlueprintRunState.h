#pragma once

#ifndef RECONSTITUTEDBLUEPRINTRUNSTATE_H
#define RECONSTITUTEDBLUEPRINTRUNSTATE_H

/*

Summary: designed for use by ReconstitutionManager, this enum class is used to represent/signify what the runmode
should be for an instance of ReconstitutionManager that is running on a dedicated thread via ReconstitutionManager::runManagerOnThread().

The run mode is used by external threads to determine what set of functions it should be issuing to a ReconstitutionManager that is running 
on another thread.

*/

enum class ReconstitutedBlueprintRunState
{
	WAITING_FOR_RUN,		// this state indicates that the ReconstitutedBlueprint may attempt to undergo its processing.

	RECONSTITUTION_SUCCESS,	// this state indicates that the reconstitution operation on a ReconstitutionBlueprint was succesful; if this is the state
							// of a ReconstitutedBlueprint, it means that that instance can be freely loaded/moved around.

	RECONSTITUTION_FAILURE // this state indicates that the reconstitution operation on a ReconstitutionBlueprint resulted in a failure; it will need to be re-run
						   // again until it is successful, or discarded -- and it shouldn't be loaded freely anywhere.
};

#endif
