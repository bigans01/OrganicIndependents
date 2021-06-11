#pragma once

#include "DynamicBorderCornerPoint.h"
#include "DynamicBorder.h"

#ifndef DYNAMICBORDERLINELIST_H
#define DYNAMICBORDERLINELIST_H

class DynamicBorderLineList
{
	public:
		DynamicBorderLineList() {};
		void constructBorders(float in_dimLength)
		{
			dimLength = in_dimLength;
			setBorders();
		}

		// Type 1 borders (corners)
		DynamicBorderCornerPoint  corner_UpperNW;
		DynamicBorderCornerPoint  corner_UpperSW;
		DynamicBorderCornerPoint  corner_UpperSE;
		DynamicBorderCornerPoint  corner_UpperNE;
		
		DynamicBorderCornerPoint  corner_LowerNW;
		DynamicBorderCornerPoint  corner_LowerSW;
		DynamicBorderCornerPoint  corner_LowerSE;
		DynamicBorderCornerPoint  corner_LowerNE;

		// Type 2 borders (lines)
		DynamicBorder Zaxis_lowerWest;
		DynamicBorder Zaxis_lowerEast;
		DynamicBorder Zaxis_upperWest;
		DynamicBorder Zaxis_upperEast;
		
		DynamicBorder Xaxis_lowerNorth;
		DynamicBorder Xaxis_lowerSouth;
		DynamicBorder Xaxis_upperNorth;
		DynamicBorder Xaxis_upperSouth;
		
		DynamicBorder Yaxis_northWest;
		DynamicBorder Yaxis_northEast;
		DynamicBorder Yaxis_southWest;
		DynamicBorder Yaxis_southEast;

		// Type 1 borders (faces)
		DynamicBorder WestFace;
		DynamicBorder NorthFace;
		DynamicBorder EastFace;
		DynamicBorder SouthFace;
		DynamicBorder TopFace;
		DynamicBorder BottomFace;

		float dimLength = 0.0f;
		void setBorders();
};

#endif
