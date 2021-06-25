#pragma once

#ifndef PRIMARYSEGMENTTRACKER_H
#define PRIMARYSEGMENTTRACKER_H

#include "EnclaveBlockVertex.h"
#include "ECBPolyPoint.h"
#include "SegmentMeta.h"

class PrimarySegmentTracker
{
public:
	SegmentMeta lineSegments[3];	// store up to 3 line segments (21 bytes)
	unsigned char currentLineSegmentIndex = 0;	// current index will start at 0
	short reservedShort = 0;			// currently unused; reserved for later
	void addNewSegment(ECBPolyPoint in_lineSegmentPointA, ECBPolyPoint in_lineSegmentPointB, int in_lineID);
	void swapBeginAndEndInSegments();
	int getIndexOfLineID(int in_lineID);
};

#endif
