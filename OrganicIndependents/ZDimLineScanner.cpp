#include "stdafx.h"
#include "ZDimLineScanner.h"

void ZDimLineScanner::runScan()
{
	for (auto& currentFixedZ : fixedScanPoints)
	{
		auto currentZPoints = dimensionalUniquePointsRef->getAllPointsWithZ(currentFixedZ);
		
		/*
		std::cout << "(ZDimLineScanner::runScan): Starting scan at fixed Z value of: " << currentFixedZ << std::endl;
		std::cout << "(ZDimLineScanner::runScan): dimensionalScanBackwardKey value is: " << dimensionalScanBackwardKey << std::endl;
		std::cout << "(ZDimLineScanner::runScan): dimensionalScanForwardKey value is: " << dimensionalScanForwardKey << std::endl;

		std::cout << "(ZDimLineScanner::runScan): points for Z value of " << currentFixedZ << " are: " << std::endl;
		

		for (auto& printPoints : currentZPoints)
		{
			printPoints.printPointData();
			std::cout << std::endl;
		}
		*/

		// Need two passes:
		// 1. Determine the number of exterior points at this Z-coordinate value.
		//	Case A:	If the number of EXTERIOR points is 2, these are the endpoints, and we choose any of the two points as the starting point,
		//			and determine the sign values for the remaining 2 dimensions; these sign values will be constantly used to 
		//			apply against the line scanner as it needs to increment/decrement the point keys. You will
		//			also need to calibrate the key values of the two remaining dimensions (X and Y).
		//  Case B: More than 2 EXTERIOR points. Find the two points that have the maximum distance between them.
		//		    These will be the endpoints. Calculate the X/Y for these endpoints, and calibrate them. 
		// 3. Copy the remaining points into a TrackedMap

		// Step 1.1: Determine the number of exterior points.
		int numberOfExteriorPoints = 0;
		for (auto& firstPass : currentZPoints)
		{
			if (firstPass.pointType == FTrianglePointType::EXTERIOR)
			{
				numberOfExteriorPoints++;
			}
		}

		// First statement is the typical case.
		if (numberOfExteriorPoints == 2)
		{
			//std::cout << "Typical case (2 exterior points), found for the scanned area at X " << currentFixedZ << std::endl;
			handleNormalXYScan(&currentZPoints, currentFixedZ);
		}
		else if (numberOfExteriorPoints > 2)
		{
			//std::cout << "Outlier case (More than 2 exterior points), found for the scanned are at X " << currentFixedZ << std::endl;
			handleAbnormalXYScan(&currentZPoints, currentFixedZ);
		}

		dimensionalScanBackwardKey++;
		dimensionalScanForwardKey++;
	}
}

void ZDimLineScanner::handleNormalXYScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimZ)
{
	std::vector<FTrianglePoint> exteriorPointVector;
	std::map<int, FTrianglePoint> interiorPoints;
	for (auto& xyScannedPoint : *in_pointsVectorRef)
	{
		// if it's an exterior point, put into the exterior vector.
		if (xyScannedPoint.pointType == FTrianglePointType::EXTERIOR)
		{
			exteriorPointVector.push_back(xyScannedPoint);
		}

		// otherwise, put it into the interior container.
		else if 
		(
			(xyScannedPoint.pointType == FTrianglePointType::INTER_FROM_XRAYCAST)
			||
			(xyScannedPoint.pointType == FTrianglePointType::INTER_FROM_YRAYCAST)
			||
			(xyScannedPoint.pointType == FTrianglePointType::INTER_FROM_ZRAYCAST)
		)
		{
			interiorPoints[interiorPoints.size()] = xyScannedPoint;
		}
	}

	// There's no need to reorganize the points; so just use exteriorPointVector and interiorPoints above for the line scanning.
	determineXandYSigns(*exteriorPointVector.begin(), *exteriorPointVector.rbegin());
	auto currentSliceXYDim = determineStartingXYDimForSlice(*exteriorPointVector.begin(), *exteriorPointVector.rbegin());
	produceZSliceLines(&exteriorPointVector,
					&interiorPoints,
					in_currentDimZ,
					currentSliceXYDim);
}

void ZDimLineScanner::handleAbnormalXYScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimZ)
{
	std::vector<FTrianglePoint> exteriorPointVector;
	std::map<int, FTrianglePoint> interiorPoints;
	for (auto& xyScannedPoint : *in_pointsVectorRef)
	{
		// if it's an exterior point, put into the exterior vector.
		if (xyScannedPoint.pointType == FTrianglePointType::EXTERIOR)
		{
			exteriorPointVector.push_back(xyScannedPoint);
		}

		// otherwise, put it into the interior container.
		else if 
		(
			(xyScannedPoint.pointType == FTrianglePointType::INTER_FROM_XRAYCAST)
			||
			(xyScannedPoint.pointType == FTrianglePointType::INTER_FROM_YRAYCAST)
			||
			(xyScannedPoint.pointType == FTrianglePointType::INTER_FROM_ZRAYCAST)
		)
		{
			interiorPoints[interiorPoints.size()] = xyScannedPoint;
		}
	}

	// Call reorganizePoints to get the sorted out points.
	auto correctedPoints = reorganizePoints(exteriorPointVector, interiorPoints);

	// there should be two endpoints
	determineXandYSigns(*correctedPoints.endpoints.begin(), *correctedPoints.endpoints.rbegin());
	auto currentSliceXYDim = determineStartingXYDimForSlice(*correctedPoints.endpoints.begin(), *correctedPoints.endpoints.rbegin());
	produceZSliceLines(&correctedPoints.endpoints,
		&correctedPoints.nonEndpoints,
		in_currentDimZ,
		currentSliceXYDim);
}

XYDim ZDimLineScanner::determineStartingXYDimForSlice(FTrianglePoint in_pointA, FTrianglePoint in_pointB)
{
	// Print the points.
	// Below: FTDEBUG (uncomment when needed)
	/*
	std::cout << "Points used for determineXYDimForSlice: " << std::endl;
	in_pointA.printPointData();
	std::cout << std::endl;
	in_pointB.printPointData();
	std::cout << std::endl;
	*/

	// Check X.
	int nonModuloX = int(floor(in_pointA.point.x / fixedDimensionalInterval));
	float moduloX = fmod(in_pointA.point.x, fixedDimensionalInterval);

	// If the point is exactly on a grid line (moduloX is 0), we need to check against in_pointB,
	// to see if the x value of point B is less than the x value in point A. If this is the case, we will need to decrement
	// the value of nonModuloY by 1.
	if (moduloX == 0.0f)
	{
		if (in_pointB.point.x < in_pointA.point.x)
		{
			//std::cout << "(ZDimLineScanner::determineStartingXYDimForSlice): Had to adjust nonModuloX." << std::endl;
			nonModuloX--;
		}
	}

	// Check Y.
	int nonModuloY = int(floor(in_pointA.point.y / fixedDimensionalInterval));
	float moduloY = fmod(in_pointA.point.y, fixedDimensionalInterval);

	// Same thing as above, but for Y.
	if (moduloY == 0.0f)
	{
		if (in_pointB.point.y < in_pointA.point.y)
		{
			//std::cout << "(ZDimLineScanner::determineStartingXYDimForSlice): Had to adjust nonModuloY." << std::endl;
			nonModuloY--;	// decrement by one.
		}
	}

	//std::cout << "Starting X and Y dim values for pointA for this slice, at Z " << in_pointA.point.z << " are: " << std::endl;
	//std::cout << "X: " << nonModuloX << std::endl;
	//std::cout << "Y: " << nonModuloY << std::endl;

	XYDim returnDim(nonModuloX, nonModuloY);
	return returnDim;
}

void ZDimLineScanner::determineXandYSigns(FTrianglePoint in_pointA, FTrianglePoint in_pointB)
{
	ECBPolyPoint pointAConverted(in_pointA.point.x, in_pointA.point.y, in_pointA.point.z);
	ECBPolyPoint pointBConverted(in_pointB.point.x, in_pointB.point.y, in_pointB.point.z);
	ECBPolyPoint absoluteDirections = IndependentUtils::findNormalizedSlope(pointAConverted, pointBConverted);
	xDimSign = absoluteDirections.x;
	yDimSign = absoluteDirections.y;

	// Below: FTDEBUG (uncomment when needed)
	/*
	std::cout << "Point A for slope: ";
	in_pointA.printPointData();
	std::cout << std::endl;

	std::cout << "Point B for slope: ";
	in_pointB.printPointData();
	std::cout << std::endl;

	std::cout << "xDimSign for this slice scan: " << xDimSign << std::endl;
	std::cout << "yDimSign for this slice scan: " << yDimSign << std::endl;
	*/
}

void ZDimLineScanner::produceZSliceLines(std::vector<FTrianglePoint>* in_endPointsVectorRef,
	std::map<int, FTrianglePoint>* in_nonendPointsMapRef,
	float in_currentDimZ,
	XYDim in_sliceStartingXYDim)
{
	// This function assumes that the vector pointed to by in_endPointsVectorRef contains exactly two points.

	// The total number of scans required is equal to the total number of points minus 1.
	int totalNumberOfScans = int(in_endPointsVectorRef->size()) + int(in_nonendPointsMapRef->size()) - 1;
	//std::cout << "(ZDimLineScanner::produceXDimLines): number of line spawns to call: " << totalNumberOfScans << std::endl;

	// (???) Before moving over the last point in the vector, use both points in the vector to determine what the starting
	// Y and Z values will be.

	// Move over the point at the rbegin of the vector, into the in_nonendPointsMapRef; then erase it.
	(*in_nonendPointsMapRef)[in_nonendPointsMapRef->size()] = *in_endPointsVectorRef->rbegin();
	in_endPointsVectorRef->pop_back();

	//std::cout << "(ZDimLineScanner::produceXDimLines): size of in_endpointsVectorRef is now: " << in_endPointsVectorRef->size() << std::endl;
	//std::cout << "(ZDimLineScanner::produceXDimLines): size of in_nonendPointsMapRef is now: " << in_nonendPointsMapRef->size() << std::endl;

	// now, iterate an amount of times equal to totalNumberOfScans;
	// the very first point in the iterations will be the begin point of the in_endPointsVectorRef.
	FTrianglePoint currentPointA = *in_endPointsVectorRef->begin();

	// For the XYDim below, it will always start as the in_sliceStartingXYDim, which should already be calibrated correctly.
	XYDim currentLineXYDim = in_sliceStartingXYDim;
	for (int x = 0; x < totalNumberOfScans; x++)
	{
		// find the point that is the closest distance.
		int indexOfClosestPoint = findClosestPointIndex(currentPointA, in_nonendPointsMapRef);

		// use the indexOfClosestPoint to fetch the point we will be using; remember we will have to erase this point at the end.
		FTrianglePoint currentPointB = (*in_nonendPointsMapRef)[indexOfClosestPoint];

		// Below: FTDEBUG (uncomment when needed)
		/*
		std::cout << "(ZDimLineScanner::produceZSliceLines): preparing new line with the following points, at fixed Z: " << in_currentDimZ << std::endl;
		std::cout << "Current line, pointA: ";
		currentPointA.printPointData();
		std::cout << std::endl;

		std::cout << "Current line, pointB: ";
		currentPointB.printPointData();
		std::cout << std::endl;

		// do line logic here, using currentPointA, currentPointB, and other required data...
		// ..

		std::cout << "XYDim used for next line (currentLine) will be: X -> " << currentLineXYDim.x << " | Y -> " << currentLineXYDim.y << std::endl;
		*/

		currentLineXYDim = determineStartingXYDimForSlice(FTrianglePoint(currentPointA.point, FTrianglePointType::NOVAL),
														  FTrianglePoint(currentPointB.point, FTrianglePointType::NOVAL));

		ZSliceLine currentLine(currentLineXYDim,
								currentPointA,
								currentPointB,
								dimensionalScanBackwardKey,
								dimensionalScanForwardKey,
								fixedDimensionalInterval);

		// get the output from the produced currentLine, and apply it.
		auto currentLineTargetData = currentLine.retrieveProducedTargets();
		for (auto& currentTargetKey : currentLineTargetData.affectedKeys)
		{
			// for each key, insert a copy of the FTriangleLine (insertableLine) into the appropriate keyed stager.
			(*dimensionalStagerRef)[currentTargetKey].insertLine(currentLineTargetData.insertableLine);

			// Below: FTDEBUG (uncomment when needed)
			//(*dimensionalStagerRef)[currentTargetKey].printLines();
		}

		// remember, we must increment the value of currentLineXYDim by the move vals.
		//currentLineXYDim += currentLine.fetchZLineMoveVal();


		// when we are done producing the line and extracting/handling its output,
		// we must remember to do the following:
		// 1.) set the currentPointA to be the currentPointB
		// 2.) erase the point that was at the index we fetched from.
		currentPointA = currentPointB;
		(*in_nonendPointsMapRef).erase(indexOfClosestPoint);
	}
}