#pragma once

#ifndef RECONBLUEPRINTRUNMODEREQUEST_H
#define RECONBLUEPRINTRUNMODEREQUEST_H

/*

Summary: This enum class is used with the ReconstitutionManager class, so that an external 
thread can make a request to a separate thread that is running ReconstitutionManager via call to
ReconstitutionManager::runManagerOnThread. The value that is passed when an external thread calls
ReconstitutionManager::setRunModeRequest will determine the value of currentRunMode when the while 
loop in ReconstitutionManager::runManagerOnThread calls the determineRunMode() function on it's running instance.

*/

enum class ReconBlueprintRunmodeRequest
{
	RUN_IN_PROCESSING_MODE,		// when the value of currentRequest in an instance of ReconstitutionManager equals this value,
								// AND the ReconstitutionManager is running via runManagerOnThread(), the instance will run in
								// a currentRunMode of ReconBlueprintRunmode::PROCESSING, after a call to ReconstitutionManager::determineRunMode().

	RUN_IN_RETRIEVAL_MODE		// when the value of currentRequest in an instance of ReconstitutionManager equals this value,
								// AND the ReconstitutionManager is running via runManagerOnThread(), the instance will run in
								// a currentRunMode of ReconBlueprintRunmode::RETRIEVAL, after a call to ReconstitutionManager::determineRunMode().
};

#endif
