#pragma once

#ifndef FANBASE_H
#define FANBASE_H

#include "FanData.h"
#include <iostream>

class FanBase
{
public:
	virtual int getPointAtIndex(int in_pointArrayIndex) = 0;
	virtual void fillPointIndex(int in_pointArrayIndex, int in_pointID) = 0;
	virtual FanData getFanData() = 0;

	unsigned short materialID = 0;
	unsigned char numberOfTertiaries = 0;
	unsigned char faceAlignment = 0;

	// for the normal
	ECBPolyPoint emptyNormal;

	virtual void printPoints() = 0;
};

#endif