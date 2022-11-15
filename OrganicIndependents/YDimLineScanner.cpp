#include "stdafx.h"
#include "YDimLineScanner.h"

void YDimLineScanner::runScan()
{
	for (auto& currentFixedY : fixedScanPoints)
	{
		auto currentYPoints = dimensionalUniquePointsRef->getAllPointsWithY(currentFixedY);

		std::cout << "(YDimLineScanner::runScan): Starting scan at fixed Y value of: " << currentFixedY << std::endl;
		std::cout << "(YDimLineScanner::runScan): dimensionalScanBackwardKey value is: " << dimensionalScanBackwardKey << std::endl;
		std::cout << "(YDimLineScanner::runScan): dimensionalScanForwardKey value is: " << dimensionalScanForwardKey << std::endl;

		std::cout << "(YDimLineScanner::runScan): points for Y value of " << currentFixedY << " are: " << std::endl;

		for (auto& printPoints : currentYPoints)
		{
			printPoints.printPointData();
			std::cout << std::endl;
		}

		// Need two passes:
		// 1. Determine the number of exterior points at this Y-coordinate value.
		//	Case A:	If the number of EXTERIOR points is 2, these are the endpoints, and we choose any of the two points as the starting point,
		//			and determine the sign values for the remaining 2 dimensions; these sign values will be constantly used to 
		//			apply against the line scanner as it needs to increment/decrement the point keys. You will
		//			also need to calibrate the key values of the two remaining dimensions (X and Z).
		//  Case B: More than 2 EXTERIOR points. Find the two points that have the maximum distance between them.
		//		    These will be the endpoints. Calculate the X/Z for these endpoints, and calibrate them. 
		// 3. Copy the remaining points into a TrackedMap

		// Step 1.1: Determine the number of exterior points.
		int numberOfExteriorPoints = 0;
		for (auto& firstPass : currentYPoints)
		{
			if (firstPass.pointType == FTrianglePointType::EXTERIOR)
			{
				numberOfExteriorPoints++;
			}
		}

		// First statement is the typical case.
		if (numberOfExteriorPoints == 2)
		{
			std::cout << "Typical case (2 exterior points), found for the scanned area at X " << currentFixedY << std::endl;
			handleNormalXZScan(&currentYPoints, currentFixedY);
		}
		else if (numberOfExteriorPoints > 2)
		{
			std::cout << "Outlier case (More than 2 exterior points), found for the scanned are at X " << currentFixedY << std::endl;
			handleAbnormalXZScan(&currentYPoints, currentFixedY);
		}

		dimensionalScanBackwardKey++;
		dimensionalScanForwardKey++;
	}
}

void YDimLineScanner::handleNormalXZScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimY)
{
	std::vector<FTrianglePoint> exteriorPointVector;
	std::map<int, FTrianglePoint> interiorPoints;
	for (auto& xzScannedPoint : *in_pointsVectorRef)
	{
		// if it's an exterior point, put into the exterior vector.
		if (xzScannedPoint.pointType == FTrianglePointType::EXTERIOR)
		{
			std::cout << "Found exterior point: ";
			xzScannedPoint.printPointData();
			std::cout << std::endl;

			exteriorPointVector.push_back(xzScannedPoint);
		}

		// otherwise, put it into the interior container.
		else if (xzScannedPoint.pointType == FTrianglePointType::INTERIOR)
		{
			interiorPoints[interiorPoints.size()] = xzScannedPoint;
		}
	}

	// There's no need to reorganize the points; so just use exteriorPointVector and interiorPoints above for the line scanning.
	determineXandZSigns(*exteriorPointVector.begin(), *exteriorPointVector.rbegin());
	auto currentSliceXZDim = determineStartingXZDimForSlice(*exteriorPointVector.begin(), *exteriorPointVector.rbegin());
	produceYSliceLines(&exteriorPointVector,
					&interiorPoints,
					in_currentDimY,
					currentSliceXZDim);
}

void YDimLineScanner::handleAbnormalXZScan(std::vector<FTrianglePoint>* in_pointsVectorRef, float in_currentDimY)
{
	std::vector<FTrianglePoint> exteriorPointVector;
	std::map<int, FTrianglePoint> interiorPoints;
	for (auto& xzScannedPoint : *in_pointsVectorRef)
	{
		// if it's an exterior point, put into the exterior vector.
		if (xzScannedPoint.pointType == FTrianglePointType::EXTERIOR)
		{
			std::cout << "Found exterior point: ";
			xzScannedPoint.printPointData();
			std::cout << std::endl;

			exteriorPointVector.push_back(xzScannedPoint);
		}

		// otherwise, put it into the interior container.
		else if (xzScannedPoint.pointType == FTrianglePointType::INTERIOR)
		{
			interiorPoints[interiorPoints.size()] = xzScannedPoint;
		}
	}

	// Call reorganizePoints to get the sorted out points.
	auto correctedPoints = reorganizePoints(exteriorPointVector, interiorPoints);

	// there should be two endpoints
	determineXandZSigns(*correctedPoints.endpoints.begin(), *correctedPoints.endpoints.rbegin());
	auto currentSliceXZDim = determineStartingXZDimForSlice(*correctedPoints.endpoints.begin(), *correctedPoints.endpoints.rbegin());
	produceYSliceLines(&correctedPoints.endpoints,
					&correctedPoints.nonEndpoints,
					in_currentDimY,
					currentSliceXZDim);

}

XZDim YDimLineScanner::determineStartingXZDimForSlice(FTrianglePoint in_pointA, FTrianglePoint in_pointB)
{
	// Print the points.
	std::cout << "Points used for determineXZDimForSlice: " << std::endl;
	in_pointA.printPointData();
	std::cout << std::endl;
	in_pointB.printPointData();
	std::cout << std::endl;

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
			std::cout << "(YDimLineScanner::determineStartingXZDimForSlice): Had to adjust nonModuloX." << std::endl;
			nonModuloX--;
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
			std::cout << "(YDimLineScanner::determineStartingYZDimForSlice): Had to adjust nonModuloZ." << std::endl;
			nonModuloZ--;
		}
	}

	std::cout << "Starting X and Z dim values for pointA for this slice, at Y" << in_pointA.point.y << " are: " << std::endl;
	std::cout << "X: " << nonModuloX << std::endl;
	std::cout << "Z: " << nonModuloZ << std::endl;

	XZDim returnDim(nonModuloX, nonModuloZ);
	return returnDim;
}

void YDimLineScanner::determineXandZSigns(FTrianglePoint in_pointA, FTrianglePoint in_pointB)
{
	ECBPolyPoint pointAConverted(in_pointA.point.x, in_pointA.point.y, in_pointA.point.z);
	ECBPolyPoint pointBConverted(in_pointB.point.x, in_pointB.point.y, in_pointB.point.z);
	ECBPolyPoint absoluteDirections = IndependentUtils::findNormalizedSlope(pointAConverted, pointBConverted);
	xDimSign = absoluteDirections.y;
	zDimSign = absoluteDirections.z;

	std::cout << "Point A for slope: ";
	in_pointA.printPointData();
	std::cout << std::endl;

	std::cout << "Point B for slope: ";
	in_pointB.printPointData();
	std::cout << std::endl;

	std::cout << "yDimSign for this slice scan: " << xDimSign << std::endl;
	std::cout << "zDimSign for this slice scan: " << zDimSign << std::endl;
}

void YDimLineScanner::produceYSliceLines(std::vector<FTrianglePoint>* in_endPointsVectorRef,
	std::map<int, FTrianglePoint>* in_nonendPointsMapRef,
	float in_currentDimY,
	XZDim in_sliceStartingXZDim)
{
	// This function assumes that the vector pointed to by in_endPointsVectorRef contains exactly two points.

	// The total number of scans required is equal to the total number of points minus 1.
	int totalNumberOfScans = int(in_endPointsVectorRef->size()) + int(in_nonendPointsMapRef->size()) - 1;
	std::cout << "(YDimLineScanner::produceYSliceLines): number of line spawns to call: " << totalNumberOfScans << std::endl;

	// (???) Before moving over the last point in the vector, use both points in the vector to determine what the starting
	// X and Z values will be.

	// Move over the point at the rbegin of the vector, into the in_nonendPointsMapRef; then erase it.
	(*in_nonendPointsMapRef)[in_nonendPointsMapRef->size()] = *in_endPointsVectorRef->rbegin();
	in_endPointsVectorRef->pop_back();

	std::cout << "(YDimLineScanner::produceXDimLines): size of in_endpointsVectorRef is now: " << in_endPointsVectorRef->size() << std::endl;
	std::cout << "(YDimLineScanner::produceXDimLines): size of in_nonendPointsMapRef is now: " << in_nonendPointsMapRef->size() << std::endl;

	// now, iterate an amount of times equal to totalNumberOfScans;
	// the very first point in the iterations will be the begin point of the in_endPointsVectorRef.
	FTrianglePoint currentPointA = *in_endPointsVectorRef->begin();

	// For the YZDim below, it will always start as the in_sliceStartingXZDim, which should already be calibrated correctly.
	XZDim currentLineXZDim = in_sliceStartingXZDim;
	for (int x = 0; x < totalNumberOfScans; x++)
	{
		// find the point that is the closest distance.
		int indexOfClosestPoint = findClosestPointIndex(currentPointA, in_nonendPointsMapRef);

		// use the indexOfClosestPoint to fetch the point we will be using; remember we will have to erase this point at the end.
		FTrianglePoint currentPointB = (*in_nonendPointsMapRef)[indexOfClosestPoint];

		std::cout << "(YDimLineScanner::produceXDimLines): preparing new line with the following points, at fixed Y: " << in_currentDimY << std::endl;
		std::cout << "Current line, pointA: ";
		currentPointA.printPointData();
		std::cout << std::endl;

		std::cout << "Current line, pointB: ";
		currentPointB.printPointData();
		std::cout << std::endl;

		// do line logic here, using currentPointA, currentPointB, and other required data...
		// ..

		std::cout << "XZDim used for next line (currentLine) will be: Y -> " << currentLineXZDim.x << " | Z -> " << currentLineXZDim.z << std::endl;

		YSliceLine currentLine(currentLineXZDim,
								currentPointA.point,
								currentPointB.point,
								dimensionalScanBackwardKey,
								dimensionalScanForwardKey,
								fixedDimensionalInterval);

		// get the output from the produced currentLine, and apply it.
		auto currentLineTargetData = currentLine.retrieveProducedTargets();
		for (auto& currentTargetKey : currentLineTargetData.affectedKeys)
		{
			(*dimensionalStagerRef)[currentTargetKey].insertLine(currentLineTargetData.insertableLine);

			std::cout << ">> Printing out lines at affected key, ";
			EnclaveKeyDef::EnclaveKey weirdKey = currentTargetKey;
			weirdKey.printKey();
			std::cout << ": " << std::endl;
			(*dimensionalStagerRef)[currentTargetKey].printLines();
		}

		// remember, we must increment the value of currentLineXZDim by the move vals.
		currentLineXZDim += currentLine.fetchYLineMoveVal();


		// when we are done producing the line and extracting/handling its output,
		// we must remember to do the following:
		// 1.) set the currentPointA to be the currentPointB
		// 2.) erase the point that was at the index we fetched from.
		currentPointA = currentPointB;
		(*in_nonendPointsMapRef).erase(indexOfClosestPoint);
	}
}