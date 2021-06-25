#pragma once

#ifndef POLYGROUPTICKER_H
#define POLYGROUPTICKER_H

#include "PolyGroupTickerState.h"

class PolyGroupTicker
{
public:
	unsigned char totalPolys = 0;		// the non-index ID of the poly that was last added in the previous group. For instance, if the very first group added the very first triangle, this value would be "1"; if the same group added two triangles, this would be "2". However,
										// the index value for "1" is actually 0, and "2" is actually 1.
	unsigned char currentPolys = 0;		// the latest non-index ID of a poly that was added by the current group. This value is copied into the currentPolys value during the copyCurrentPolys function call.
	void incrementCurrentPolys();
	int fetchIndexOfLastOfPreviousGroup();
	int fetchIndexOfLastOfCurrentGroup();
	void copyCurrentIntoLast();
	PolyGroupTickerState getTickerState();
};

#endif
