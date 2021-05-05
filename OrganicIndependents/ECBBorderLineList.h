#pragma once

// migrated over from OrganicCoreLib on 11/11/2019
#include "ECBBorderLine.h"
#include "ECBBorder.h"
#include "ECBBorderCornerPoint.h"
#include <iostream>

#ifndef ECBBORDERLINELIST_H
#define ECBBORDERLINELIST_H

class ECBBorderLineList
{
public:
	// Type 1 borders (corners)
	ECBBorderCornerPoint  corner_UpperNW;
	ECBBorderCornerPoint  corner_UpperSW;
	ECBBorderCornerPoint  corner_UpperSE;
	ECBBorderCornerPoint  corner_UpperNE;

	ECBBorderCornerPoint  corner_LowerNW;
	ECBBorderCornerPoint  corner_LowerSW;
	ECBBorderCornerPoint  corner_LowerSE;
	ECBBorderCornerPoint  corner_LowerNE;

	// Type 2 borders (lines)
	ECBBorder Zaxis_lowerWest;
	ECBBorder Zaxis_lowerEast;
	ECBBorder Zaxis_upperWest;
	ECBBorder Zaxis_upperEast;

	ECBBorder Xaxis_lowerNorth;
	ECBBorder Xaxis_lowerSouth;
	ECBBorder Xaxis_upperNorth;
	ECBBorder Xaxis_upperSouth;

	ECBBorder Yaxis_northWest;
	ECBBorder Yaxis_northEast;
	ECBBorder Yaxis_southWest;
	ECBBorder Yaxis_southEast;

	// Type 3 borders (faces)
	ECBBorder WestFace;
	ECBBorder NorthFace;
	ECBBorder EastFace;
	ECBBorder SouthFace;
	ECBBorder TopFace;
	ECBBorder BottomFace;

};

#endif
