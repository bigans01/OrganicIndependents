#pragma once

#ifndef YDIMLINESCANNER_H
#define YDIMLINESCANNER_H

#include "DimensionalLineScannerBase.h"
#include "XZDimMoveVal.h"
#include "XZDim.h"
#include "YSliceLine.h"

class YDimLineScanner : public DimensionalLineScannerBase
{
	public:
		void runScan();
	private:
		// the signs below will each be either -1, 0, or 1 (they must be absolute slope directions, or 0);
		// these will get reset for every scanned slice
		int xDimSign = 0;
		int zDimSign = 0;

		void handleNormalXZScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimY);		// the typical case that is used, when there are exactly 2 exterior points detected at a fixed Y coordinate.
		void handleAbnormalXZScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimY);	// the non-typical case, when there are more than 2 exterior points detected at a fixed Y coordinate.
		void determineXandZSigns(FTrianglePoint in_pointA, FTrianglePoint in_pointB);
		XZDim determineStartingXZDimForSlice(FTrianglePoint in_pointA, FTrianglePoint in_pointB);
		void produceYSliceLines(std::vector<FTrianglePoint>* in_endPointsVectorRef,
							std::map<int, FTrianglePoint>* in_nonendPointsMapRef,
							float in_currentDimY,
							XZDim in_sliceStartingXZDim);
};

#endif