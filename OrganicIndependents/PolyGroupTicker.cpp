#include "stdafx.h"
#include "PolyGroupTicker.h"

void PolyGroupTicker::resetTicker()
{
	totalPolys = 0;
	currentPolys = 0;
}

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