#pragma once

#include "BlockBorderCornerPoint.h"
#include "BlockBorder.h"

#ifndef ENCLAVEBORDERLINELIST_H
#define ENCLAVEBORDERLINELIST_H

class EnclaveBorderLineList
{
public:
	EnclaveBorderLineList();

	// Type 1 borders (corners)
	BlockBorderCornerPoint  corner_UpperNW;
	BlockBorderCornerPoint  corner_UpperSW;
	BlockBorderCornerPoint  corner_UpperSE;
	BlockBorderCornerPoint  corner_UpperNE;

	BlockBorderCornerPoint  corner_LowerNW;
	BlockBorderCornerPoint  corner_LowerSW;
	BlockBorderCornerPoint  corner_LowerSE;
	BlockBorderCornerPoint  corner_LowerNE;

	// Type 2 borders (lines)
	BlockBorder Zaxis_lowerWest;
	BlockBorder Zaxis_lowerEast;
	BlockBorder Zaxis_upperWest;
	BlockBorder Zaxis_upperEast;

	BlockBorder Xaxis_lowerNorth;
	BlockBorder Xaxis_lowerSouth;
	BlockBorder Xaxis_upperNorth;
	BlockBorder Xaxis_upperSouth;

	BlockBorder Yaxis_northWest;
	BlockBorder Yaxis_northEast;
	BlockBorder Yaxis_southWest;
	BlockBorder Yaxis_southEast;

	// Type 1 borders (faces)
	BlockBorder WestFace;
	BlockBorder NorthFace;
	BlockBorder EastFace;
	BlockBorder SouthFace;
	BlockBorder TopFace;
	BlockBorder BottomFace;
};

#endif
