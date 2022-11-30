#pragma once

#ifndef FANBASE_H
#define FANBASE_H

#include "FanData.h"

class FanBase
{
public:

	// virtual functions
	virtual int getPointAtIndex(int in_pointArrayIndex) = 0;
	virtual void fillPointIndex(int in_pointArrayIndex, int in_pointID) = 0;
	virtual FanData getFanData() = 0;

	// base class functions
	int getNumberOfTrianglesInFan()
	{
		return numberOfTertiaries;
	}

	TriangleMaterial materialID = TriangleMaterial::NOVAL;
	unsigned char numberOfTertiaries = 0;
	//unsigned char faceAlignment = 0;
	BoundaryPolyIndicator faceAlignment;

	// for the normal
	ECBPolyPoint emptyNormal;

	virtual void printPoints() = 0;


};

#endif