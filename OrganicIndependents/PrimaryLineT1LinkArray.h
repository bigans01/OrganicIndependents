#pragma once

#ifndef PRIMARYLINET1LINKARRAY_H
#define PRIMARYLINET1LINKARRAY_H

#include "PrimaryLineT1Link.h"
#include "ECBPolyPointPair.h"

class PrimaryLineT1LinkArray
{
public:
	PrimaryLineT1Link linkArray[3];		// contains up to 3 links
	int totalNumberOfLinks = 0;			// the actual number of links within linkArray
	int currentLinkIndex = 0;			// the current index we are on; we'll need special logic for when this is equal to (totalNumberOfLinks - 1), so that we may stop
	void insertNewLink(PrimaryLineT1 in_link);	// inserts a new link
	void setNextLinks();		// sets the indexOfNextLink value in each PrimaryLineT1Link
	ECBPolyPointPair getCurrentEndAndNextBeginPoints();
	void switchToNextLink();
};

#endif
