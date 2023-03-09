#pragma once

#ifndef FTRACEBORDERLINELIST_H
#define FTRACEBORDERLINELIST_H

#include "ECBBorder.h"
#include "ECBBorderCornerPoint.h"

class FTraceBorderLineList
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

		void printCorners()
		{
				std::cout << "corner_UpperNW: " << corner_UpperNW.cornerPoint.x << ", " << corner_UpperNW.cornerPoint.y << ", " << corner_UpperNW.cornerPoint.z << ", " << std::endl;
				std::cout << "corner_UpperSW: " << corner_UpperSW.cornerPoint.x << ", " << corner_UpperSW.cornerPoint.y << ", " << corner_UpperSW.cornerPoint.z << ", " << std::endl;
				std::cout << "corner_UpperSE: " << corner_UpperSE.cornerPoint.x << ", " << corner_UpperSE.cornerPoint.y << ", " << corner_UpperSE.cornerPoint.z << ", " << std::endl;
				std::cout << "corner_UpperNE: " << corner_UpperNE.cornerPoint.x << ", " << corner_UpperNE.cornerPoint.y << ", " << corner_UpperNE.cornerPoint.z << ", " << std::endl;
				std::cout << "corner_LowerNW: " << corner_LowerNW.cornerPoint.x << ", " << corner_LowerNW.cornerPoint.y << ", " << corner_LowerNW.cornerPoint.z << ", " << std::endl;
				std::cout << "corner_LowerSW: " << corner_LowerSW.cornerPoint.x << ", " << corner_LowerSW.cornerPoint.y << ", " << corner_LowerSW.cornerPoint.z << ", " << std::endl;
				std::cout << "corner_LowerSE: " << corner_LowerSE.cornerPoint.x << ", " << corner_LowerSE.cornerPoint.y << ", " << corner_LowerSE.cornerPoint.z << ", " << std::endl;
				std::cout << "corner_LowerNE: " << corner_LowerNE.cornerPoint.x << ", " << corner_LowerNE.cornerPoint.y << ", " << corner_LowerNE.cornerPoint.z << ", " << std::endl;
		}
};

#endif
