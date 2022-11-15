#pragma once

#ifndef ZDIMLINESCANNER_H
#define ZDIMLINESCANNER_H

#include "DimensionalLineScannerBase.h"
#include "XYDimMoveVal.h"
#include "XYDim.h"
#include "ZSliceLine.h"

class ZDimLineScanner : public DimensionalLineScannerBase
{
	public:
		void runScan();
	private:
		// the signs below will each be either -1, 0, or 1 (they must be absolute slope directions, or 0);
		// these will get reset for every scanned slice
		int xDimSign = 0;
		int yDimSign = 0;

		void handleNormalXYScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimZ);		// the typical case that is used, when there are exactly 2 exterior points detected at a fixed Z coordinate.
		void handleAbnormalXYScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimZ);	// the non-typical case, when there are more than 2 exterior points detected at a fixed Z coordinate.
		void determineXandYSigns(FTrianglePoint in_pointA, FTrianglePoint in_pointB);
		XYDim determineStartingXYDimForSlice(FTrianglePoint in_pointA, FTrianglePoint in_pointB);
		void produceZSliceLines(std::vector<FTrianglePoint>* in_endPointsVectorRef,
							std::map<int, FTrianglePoint>* in_nonendPointsMapRef,
							float in_currentDimZ,
							XYDim in_sliceStartingXYDim);

};

#endif