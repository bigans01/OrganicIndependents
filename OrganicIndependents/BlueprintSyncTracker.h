#pragma once

#ifndef BLUEPRINTSYNCTRACKER_H
#define BLUEPRINTSYNCTRACKER_H

#include "BlueprintState.h"

class BlueprintSyncTracker
{
	public:
		void setSyncState(BlueprintState in_blueprintState);
	private:
		BlueprintState bpSyncState = BlueprintState::FIRST_LOAD;	// default value, but should be able to be changed in other ways (i.e, BP_SYNC)
};

#endif
