#include "stdafx.h"
#include "BlueprintSyncTracker.h"

void BlueprintSyncTracker::setSyncState(BlueprintState in_blueprintState)
{
	// certain states should have certain effects on the lastUtilized and lastUpdated timestamps;
	// we need a case statement to handle these.
	switch (in_blueprintState)
	{
		// with BP_SYNC, both will be updated to the current time.
		case BlueprintState::BP_SYNC_LOCAL:
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			lastUtilized = currentTime;
			lastUpdated = currentTime;
			break;
		}

		// with BP_SYNC, both will be updated to the current time.
		case BlueprintState::BP_SYNC_REMOTE:
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			lastUtilized = currentTime;
			lastUpdated = currentTime;
			break;
		}

		// FIRST_LOAD behaves the same way that BP_SYNC does.
		case BlueprintState::FIRST_LOAD:
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			lastUtilized = currentTime;
			lastUpdated = currentTime;
			break;
		}

		// In the case of BP_UNSYNC_LOCAL, it means that the lastUpdated time is ahead of the lastUtilized time, and the blueprint is in a local state.
		case BlueprintState::BP_UNSYNC_LOCAL:
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			lastUpdated = currentTime;
			break;
		}

		// In the case of BP_UNSYNC_REMOTE, it means that the lastUpdated time is ahead of the lastUtilized time, and the blueprint is in a remote state.
		case BlueprintState::BP_UNSYNC_REMOTE:
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			lastUpdated = currentTime;
			break;
		}
	}

	// lastly, now that the appropriate timestamps have been updated, go ahead and set the actual state.
	bpSyncState = in_blueprintState;
}

BlueprintState BlueprintSyncTracker::getSyncState()
{
	return bpSyncState;
}