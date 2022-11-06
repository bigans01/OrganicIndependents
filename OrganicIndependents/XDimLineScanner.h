#pragma once

#ifndef XDIMLINESCANNER_H
#define XDIMLINESCANNER_H

#include "DimensionalLineScannerBase.h"
#include "YZDimMoveVal.h"
#include "YZDim.h"
#include "XSliceLine.h"

class XDimLineScanner : public DimensionalLineScannerBase
{
	public:
		void runScan();
	private:
		// the signs below will each be either -1, 0, or 1 (they must be absolute slope directions, or 0);
		// these will get reset for every scanned slice
		int yDimSign = 0;
		int zDimSign = 0;

		void handleNormalYZScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimX);		// the typical case that is used, when there are exactly 2 exterior points detected at a fixed X coordinate.
		void handleAbnormalYZScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimX);	// the non-typical case, when there are more than 2 exterior points detected at a fixed X coordinate.
		void determineYandZSigns(FTrianglePoint in_pointA, FTrianglePoint in_pointB);
		YZDim determineStartingYZDimForSlice(FTrianglePoint in_pointA, FTrianglePoint in_pointB);
		void produceXSliceLines(std::vector<FTrianglePoint>* in_endPointsVectorRef,
							std::map<int, FTrianglePoint>* in_nonendPointsMapRef,
							float in_currentDimX,
							YZDim in_sliceStartingYZDim);
};

#endif
