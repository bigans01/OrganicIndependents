#pragma once

#ifndef RECONBLUEPRINTRUNMODE_H
#define RECONBLUEPRINTRUNMODE_H

/*

Summary: used by ReconstitutionManager to determine what operating mode it should be running in (see currentRunMode member in that class).
Switching between run modes should only ever need to be done when a ReconstitutionManager is being run on a dedicated thread; the mode
should only be changed when the ReconstitutionManager is running on a dedicated thread via a call to runManagerOnThread of that class, AND
a separate external thread calls ReconstitutionManager::setRunModeRequest.

*/

enum class ReconBlueprintRunmode
{
	PROCESSING,	// to set to this value, use ReconBlueprintRunmodeRequest::RUN_IN_PROCESSING_MODE
	RETRIEVAL	// to set to this value, use ReconBlueprintRunmodeRequest::RUN_IN_RETRIEVAL_MODE
};

#endif
