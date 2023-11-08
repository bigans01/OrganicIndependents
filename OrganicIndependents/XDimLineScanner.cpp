#include "stdafx.h"
#include "XDimLineScanner.h"

void XDimLineScanner::runScan()
{
	// each iteration in the loop below would be a scanning plane.
	for (auto& currentFixedX : fixedScanPoints)
	{
		auto currentXPoints = dimensionalUniquePointsRef->getAllPointsWithX(currentFixedX);

		/*
		std::cout << "(XDimLineScanner::runScan): Starting scan at fixed X value of: " << currentFixedX << std::endl;
		std::cout << "(XDimLineScanner::runScan): dimensionalScanBackwardKey value is: " << dimensionalScanBackwardKey << std::endl;
		std::cout << "(XDimLineScanner::runScan): dimensionalScanForwardKey value is: " << dimensionalScanForwardKey << std::endl;

		std::cout << "(XDimLineScanner::runScan): points for X value of " << currentFixedX << " are: " << std::endl;
		

		for (auto& printPoints : currentXPoints)
		{
			printPoints.printPointData();
			std::cout << std::endl;
		}
		*/

		// Need two passes:
		// 1. Determine the number of exterior points at this X-coordinate value.
		//	Case A:	If the number of EXTERIOR points is 2, these are the endpoints, and we choose any of the two points as the starting point,
		//			and determine the sign values for the remaining 2 dimensions; these sign values will be constantly used to 
		//			apply against the line scanner as it needs to increment/decrement the point keys. You will
		//			also need to calibrate the key values of the two remaining dimensions (Y and Z).
		//  Case B: More than 2 EXTERIOR points. Find the two points that have the maximum distance between them.
		//		    These will be the endpoints. Calculate the Y/Z for these endpoints, and calibrate them. 
		// 3. Copy the remaining points into a TrackedMap

		// Step 1.1: Determine the number of exterior points.
		int numberOfExteriorPoints = 0;
		for (auto& firstPass : currentXPoints)
		{
			if (firstPass.pointType == FTrianglePointType::EXTERIOR)
			{
				numberOfExteriorPoints++;
			}
		}

		// First statement is the typical case.
		if (numberOfExteriorPoints == 2)
		{
			//std::cout << "Typical case (2 exterior points), found for the scanned area at X " << currentFixedX << std::endl;
			handleNormalYZScan(&currentXPoints, currentFixedX);
		}
		else if (numberOfExteriorPoints > 2)
		{
			//std::cout << "Outlier case (More than 2 exterior points), found for the scanned are at X " << currentFixedX << std::endl;
			handleAbnormalYZScan(&currentXPoints, currentFixedX);
		}

		dimensionalScanBackwardKey++;
		dimensionalScanForwardKey++;
	}
}

void XDimLineScanner::handleNormalYZScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimX)
{
	std::vector<FTrianglePoint> exteriorPointVector;
	std::map<int, FTrianglePoint> interiorPoints;
	for (auto& yzScannedPoint : *in_pointsVectorRef)
	{
		// if it's an exterior point, put into the exterior vector.
		if (yzScannedPoint.pointType == FTrianglePointType::EXTERIOR)
		{
			exteriorPointVector.push_back(yzScannedPoint);
		}

		// otherwise, put it into the interior container.
		else if 
		(
			(yzScannedPoint.pointType == FTrianglePointType::INTER_FROM_XRAYCAST)
			||
			(yzScannedPoint.pointType == FTrianglePointType::INTER_FROM_YRAYCAST)
			||
			(yzScannedPoint.pointType == FTrianglePointType::INTER_FROM_ZRAYCAST)
		)
		{
			interiorPoints[interiorPoints.size()] = yzScannedPoint;
		}
	}

	// There's no need to reorganize the points; so just use exteriorPointVector and interiorPoints above for the line scanning.
	determineYandZSigns(*exteriorPointVector.begin(), *exteriorPointVector.rbegin());
	auto currentSliceYZDim = determineStartingYZDimForSlice(*exteriorPointVector.begin(), *exteriorPointVector.rbegin());
	produceXSliceLines(&exteriorPointVector, 
					&interiorPoints, 
					in_currentDimX, 
					currentSliceYZDim);
}

void XDimLineScanner::handleAbnormalYZScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimX)
{
	std::vector<FTrianglePoint> exteriorPointVector;
	std::map<int, FTrianglePoint> interiorPoints;
	for (auto& yzScannedPoint : *in_pointsVectorRef)
	{
		// if it's an exterior point, put into the exterior vector.
		if (yzScannedPoint.pointType == FTrianglePointType::EXTERIOR)
		{
			exteriorPointVector.push_back(yzScannedPoint);
		}

		// otherwise, put it into the interior container.
		else if 
		(
			(yzScannedPoint.pointType == FTrianglePointType::INTER_FROM_XRAYCAST)
			||
			(yzScannedPoint.pointType == FTrianglePointType::INTER_FROM_YRAYCAST)
			||
			(yzScannedPoint.pointType == FTrianglePointType::INTER_FROM_ZRAYCAST)
		)
		{
			interiorPoints[interiorPoints.size()] = yzScannedPoint;
		}
	}

	// Call reorganizePoints to get the sorted out points.
	auto correctedPoints = reorganizePoints(exteriorPointVector, interiorPoints);

	// there should be two endpoints
	determineYandZSigns(*correctedPoints.endpoints.begin(), *correctedPoints.endpoints.rbegin());
	auto currentSliceYZDim = determineStartingYZDimForSlice(*correctedPoints.endpoints.begin(), *correctedPoints.endpoints.rbegin());
	produceXSliceLines(&correctedPoints.endpoints, 
					&correctedPoints.nonEndpoints, 
					in_currentDimX, 
					currentSliceYZDim);
}

YZDim XDimLineScanner::determineStartingYZDimForSlice(FTrianglePoint in_pointA, FTrianglePoint in_pointB)
{
	// Below: FTDEBUG (uncomment when needed)
	/*
	// Print the points.
	std::cout << "Points used for determineStartingYZDimForSlice: " << std::endl;
	in_pointA.printPointData();
	std::cout << std::endl;
	in_pointB.printPointData();
	std::cout << std::endl;
	*/

	// Check Y.
	int nonModuloY = int(floor(in_pointA.point.y / fixedDimensionalInterval));
	float moduloY = fmod(in_pointA.point.y, fixedDimensionalInterval);

	// If the point is exactly on a grid line (moduloY is 0), we need to check against in_pointB,
	// to see if the y value of point B is less than the x value in point A. If this is the case, we will need to decrement
	// the value of nonModuloY by 1.
	if (moduloY == 0.0f)
	{
		if (in_pointB.point.y < in_pointA.point.y)
		{
			//std::cout << "(XDimLineScanner::determineStartingYZDimForSlice): Had to adjust nonModuloY." << std::endl;
			nonModuloY--;	// decrement by one.
		}
	}

	// Check Z.
	int nonModuloZ = int(floor(in_pointA.point.z / fixedDimensionalInterval));
	float moduloZ = fmod(in_pointA.point.z, fixedDimensionalInterval);

	// Same thing as above, but for Z.
	if (moduloZ == 0.0f)
	{
		if (in_pointB.point.z < in_pointA.point.z)
		{
			//std::cout << "(XDimLineScanner::determineStartingYZDimForSlice): Had to adjust nonModuloZ." << std::endl;
			nonModuloZ--;
		}
	}

	//std::cout << "Starting Y and Z dim values for pointA for this slice, at X " << in_pointA.point.x << " are: " << std::endl;
	//std::cout << "Y: " << nonModuloY << std::endl;
	//std::cout << "Z: " << nonModuloZ << std::endl;

	YZDim returnDim(nonModuloY, nonModuloZ);
	return returnDim;
}

void XDimLineScanner::determineYandZSigns(FTrianglePoint in_pointA, FTrianglePoint in_pointB)
{
	ECBPolyPoint pointAConverted(in_pointA.point.x, in_pointA.point.y, in_pointA.point.z);
	ECBPolyPoint pointBConverted(in_pointB.point.x, in_pointB.point.y, in_pointB.point.z);
	ECBPolyPoint absoluteDirections = IndependentUtils::findNormalizedSlope(pointAConverted, pointBConverted);
	yDimSign = absoluteDirections.y;
	zDimSign = absoluteDirections.z;

	// Below: FTDEBUG (uncomment when needed)
	/*
	std::cout << "Point A for slope: ";
	in_pointA.printPointData();
	std::cout << std::endl;

	std::cout << "Point B for slope: ";
	in_pointB.printPointData();
	std::cout << std::endl;

	std::cout << "yDimSign for this slice scan: " << yDimSign << std::endl;
	std::cout << "zDimSign for this slice scan: " << zDimSign << std::endl;
	*/
}

void XDimLineScanner::produceXSliceLines(std::vector<FTrianglePoint>* in_endPointsVectorRef,
										std::map<int, FTrianglePoint>* in_nonendPointsMapRef,
										float in_currentDimX,
										YZDim in_sliceStartingYZDim)
{
	// This function assumes that the vector pointed to by in_endPointsVectorRef contains exactly two points.

	// The total number of scans required is equal to the total number of points minus 1.
	int totalNumberOfScans = int(in_endPointsVectorRef->size()) + int(in_nonendPointsMapRef->size()) - 1;
	//std::cout << "(XDimLineScanner::produceXDimLines): number of line spawns to call: " << totalNumberOfScans << std::endl;

	// (???) Before moving over the last point in the vector, use both points in the vector to determine what the starting
	// Y and Z values will be.

	// Move over the point at the rbegin of the vector, into the in_nonendPointsMapRef; then erase it.
	(*in_nonendPointsMapRef)[in_nonendPointsMapRef->size()] = *in_endPointsVectorRef->rbegin();
	in_endPointsVectorRef->pop_back();

	//std::cout << "(XDimLineScanner::produceXDimLines): size of in_endpointsVectorRef is now: " << in_endPointsVectorRef->size() << std::endl;
	//std::cout << "(XDimLineScanner::produceXDimLines): size of in_nonendPointsMapRef is now: " << in_nonendPointsMapRef->size() << std::endl;

	// now, iterate an amount of times equal to totalNumberOfScans;
	// the very first point in the iterations will be the begin point of the in_endPointsVectorRef.
	FTrianglePoint currentPointA = *in_endPointsVectorRef->begin();

	// For the YZDim below, it will always start as the in_sliceStartingYZDim, which should already be calibrated correctly.
	YZDim currentLineYZDim = in_sliceStartingYZDim;	
	for (int x = 0; x < totalNumberOfScans; x++)
	{
		// find the point that is the closest distance.
		int indexOfClosestPoint = findClosestPointIndex(currentPointA, in_nonendPointsMapRef);

		// use the indexOfClosestPoint to fetch the point we will be using; remember we will have to erase this point at the end.
		FTrianglePoint currentPointB = (*in_nonendPointsMapRef)[indexOfClosestPoint];

		// Below: FTDEBUG (uncomment when needed)
		/*
		std::cout << "(XDimLineScanner::produceXSliceLines): preparing new line with the following points, at fixed X: " << in_currentDimX << std::endl;
		std::cout << "Current line, pointA: ";
		currentPointA.printPointData();
		std::cout << std::endl;

		std::cout << "Current line, pointB: ";
		currentPointB.printPointData();
		std::cout << std::endl;
		

		// do line logic here, using currentPointA, currentPointB, and other required data...
		// ..

		std::cout << "YZDim used for next line (currentLine) will be: Y -> " << currentLineYZDim.y << " | Z -> " << currentLineYZDim.z << std::endl;
		*/

		currentLineYZDim = determineStartingYZDimForSlice(FTrianglePoint(currentPointA.point, FTrianglePointType::NOVAL),
														FTrianglePoint(currentPointB.point, FTrianglePointType::NOVAL));

		XSliceLine currentLine(currentLineYZDim,
								currentPointA,
								currentPointB,
								dimensionalScanBackwardKey,
								dimensionalScanForwardKey,
								fixedDimensionalInterval);

		// get the output from the produced currentLine, and apply it.
		auto currentLineTargetData = currentLine.retrieveProducedTargets();
		for (auto& currentTargetKey : currentLineTargetData.affectedKeys)
		{
			(*dimensionalStagerRef)[currentTargetKey].insertLine(currentLineTargetData.insertableLine);
			
			// Below: FTDEBUG (uncomment when needed)
			//(*dimensionalStagerRef)[currentTargetKey].printLines();
		}

		// remember, we must increment the value of currentLineYZDim by the move vals.
		//currentLineYZDim += currentLine.fetchXLineMoveVal();
							

		// when we are done producing the line and extracting/handling its output,
		// we must remember to do the following:
		// 1.) set the currentPointA to be the currentPointB
		// 2.) erase the point that was at the index we fetched from.
		currentPointA = currentPointB;
		(*in_nonendPointsMapRef).erase(indexOfClosestPoint);
	}
}