#include "stdafx.h"
#include "PolyGroupTicker.h"

void PolyGroupTicker::incrementCurrentPolys()
{
	currentPolys++;
}

int PolyGroupTicker::fetchIndexOfLastOfPreviousGroup()
{
	int returnVal = int(totalPolys);
	returnVal--;
	return returnVal;
}
int PolyGroupTicker::fetchIndexOfLastOfCurrentGroup()
{
	int returnVal = int(currentPolys);
	returnVal--;
	return returnVal;
}

void PolyGroupTicker::copyCurrentIntoLast()
{
	totalPolys = currentPolys;
}

PolyGroupTickerState PolyGroupTicker::getTickerState()
{
	PolyGroupTickerState tickerState = PolyGroupTickerState::UNCHANGED;	// assume its unchanged.
	int lastOfPrevious = fetchIndexOfLastOfPreviousGroup();
	int lastOfCurrent = fetchIndexOfLastOfCurrentGroup();
	if (lastOfPrevious != lastOfCurrent)
	{
		tickerState = PolyGroupTickerState::CHANGED;
	}
	return tickerState;
}