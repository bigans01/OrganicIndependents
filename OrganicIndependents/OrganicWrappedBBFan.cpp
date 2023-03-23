#include "stdafx.h"
#include "OrganicWrappedBBFan.h"

OrganicWrappedBBFan::OrganicWrappedBBFan()
{

}

OrganicWrappedBBFan::OrganicWrappedBBFan(FTriangleContainer* in_fTriangleContainerRef,
								TriangleMaterial in_materialID,
								ECBPolyPoint in_emptyNormal,
								BoundaryPolyIndicator in_boundaryPolyIndicator)
{
	// This function should set things up in a way similiar to the buildBBFanWithBoundaryIndicator function of this class.
	int totalTriangles = in_fTriangleContainerRef->fracturedTriangles.size();
	
	// Cycle through all triangles. The very first triangle should insert all 3 of its points; the remaining triangles 
	// should just insert the point at index 2.


	// Part 1: point insertion.
	auto anchorIter = in_fTriangleContainerRef->fracturedTriangles.begin();	// this iter will never change; it's used to determine if we're looking at the first triangle.
	auto currentIter = anchorIter;
	auto endIter = in_fTriangleContainerRef->fracturedTriangles.end();

	// Current point index for loading all points.
	int masterPointindex = 0;

	for (; currentIter != endIter; currentIter++)
	{

		// If we're looking at the very first triangle, insert all 3 points.
		if (currentIter == anchorIter)
		{
			for (int x = 0; x < 3; x++)
			{
				poly.fillPointIndex(masterPointindex, masterPointindex);

				/*
				std::cout << "!! Inserting point "; 
				currentIter->second.fracturePoints[x].printPointCoords();
				std::cout << " at index " << masterPointindex << std::endl;
				*/

				vertices[masterPointindex] = IndependentUtils::convertFTriangleDoublePointToBlockVertex(currentIter->second.fracturePoints[x]);
				masterPointindex++;
			}
		}

		// Otherwise, insert the last point (at index 2) of all other triangles.
		else
		{
			poly.fillPointIndex(masterPointindex, masterPointindex);

			/*
			std::cout << "!! Inserting point ";
			currentIter->second.fracturePoints[2].printPointCoords();
			std::cout << " at index " << masterPointindex << std::endl;
			*/

			vertices[masterPointindex] = IndependentUtils::convertFTriangleDoublePointToBlockVertex(currentIter->second.fracturePoints[2]);
			masterPointindex++;
		}
	}

	// Part 2: metadata updates.
	poly.numberOfTertiaries = totalTriangles;
	poly.materialID = in_materialID;
	poly.emptyNormal = in_emptyNormal;
	poly.faceAlignment = in_boundaryPolyIndicator;
}


void OrganicWrappedBBFan::buildBBFan(BlockCircuit* in_blockCircuitRef, TriangleMaterial in_materialID, ECBPolyPoint in_emptyNormal)
{
	// Step 1: load the points.
	//std::cout << "********* Loading points: " << std::endl;

	for (int x = 0; x < in_blockCircuitRef->finalCircuitPoints.numberOfPoints; x++)
	{
		//poly.pointArray[x] = (unsigned char)x;	// convert to unsigned char, then store.
		poly.fillPointIndex(x, x);
		vertices[x] = IndependentUtils::convertPolyPointToBlockVertex(in_blockCircuitRef->finalCircuitPoints.pointArray[x]);

		//std::cout << "point " << x << ": " << in_blockCircuitRef->finalCircuitPoints.pointArray[x].x << ", " << in_blockCircuitRef->finalCircuitPoints.pointArray[x].y << ", " << in_blockCircuitRef->finalCircuitPoints.pointArray[x].z << std::endl;
	}

	poly.numberOfTertiaries = in_blockCircuitRef->finalCircuitPoints.numberOfPoints - 2;	// store the number of triangles (equal to number of points - 2)
	//std::cout << "BBFan number of tertiaries: " << int(poly.numberOfTertiaries) << std::endl;
	poly.materialID = in_materialID;
	poly.emptyNormal = in_emptyNormal;
}

void OrganicWrappedBBFan::buildBBFanWithBoundaryIndicator(BlockCircuit* in_blockCircuitRef,
	TriangleMaterial in_materialID,
	ECBPolyPoint in_emptyNormal,
	BoundaryPolyIndicator in_boundaryPolyIndicator)
{
	// Step 1: load the points.
	//std::cout << "********* Loading points: " << std::endl;

	for (int x = 0; x < in_blockCircuitRef->finalCircuitPoints.numberOfPoints; x++)
	{
		//poly.pointArray[x] = (unsigned char)x;	// convert to unsigned char, then store.
		poly.fillPointIndex(x, x);
		vertices[x] = IndependentUtils::convertPolyPointToBlockVertex(in_blockCircuitRef->finalCircuitPoints.pointArray[x]);

		//std::cout << "(OrganicWrappedBBFan) inserting point " << x << ": " << in_blockCircuitRef->finalCircuitPoints.pointArray[x].x << ", " << in_blockCircuitRef->finalCircuitPoints.pointArray[x].y << ", " << in_blockCircuitRef->finalCircuitPoints.pointArray[x].z << std::endl;
	}

	poly.numberOfTertiaries = in_blockCircuitRef->finalCircuitPoints.numberOfPoints - 2;	// store the number of triangles (equal to number of points - 2)
	//std::cout << "BBFan number of tertiaries: " << int(poly.numberOfTertiaries) << std::endl;
	poly.materialID = in_materialID;
	poly.emptyNormal = in_emptyNormal;
	poly.faceAlignment = in_boundaryPolyIndicator;
}


void OrganicWrappedBBFan::runBoundaryChecks()
{
	// we're going to assume that there are tertiaries remaining; we shouldn't be calling this function if there are 0 tertiaries.
	// Take the points, round them for safety, and then analyze them.
	ECBPolyPoint convertedPoint0 = IndependentUtils::roundPolyPointToHundredths(IndependentUtils::convertEnclaveBlockVertexToFloats(vertices[0]));
	ECBPolyPoint convertedPoint1 = IndependentUtils::roundPolyPointToHundredths(IndependentUtils::convertEnclaveBlockVertexToFloats(vertices[1]));
	ECBPolyPoint convertedPoint2 = IndependentUtils::roundPolyPointToHundredths(IndependentUtils::convertEnclaveBlockVertexToFloats(vertices[2]));

	// Check for NEG_Z
	if
	(
		convertedPoint0.z == 0.0f
		&&
		convertedPoint1.z == 0.0f
		&&
		convertedPoint2.z == 0.0f
	)
	{
		poly.faceAlignment.setBoundaryIndicator(BoundaryOrientation::NEG_Z);
	}

	// Check for POS_X
	else if
	(
		convertedPoint0.x == 1.0f
		&&
		convertedPoint1.x == 1.0f
		&&
		convertedPoint2.x == 1.0f
	)
	{
		poly.faceAlignment.setBoundaryIndicator(BoundaryOrientation::POS_X);
	}

	// Check for POS_Z
	else if
	(
		convertedPoint0.z == 1.0f
		&&
		convertedPoint1.z == 1.0f
		&&
		convertedPoint2.z == 1.0f
	)
	{
		poly.faceAlignment.setBoundaryIndicator(BoundaryOrientation::POS_Z);
	}

	// Check for NEG_X
	else if
	(
		convertedPoint0.x == 0.0f
		&&
		convertedPoint1.x == 0.0f
		&&
		convertedPoint2.x == 0.0f
	)
	{
		poly.faceAlignment.setBoundaryIndicator(BoundaryOrientation::NEG_X);
	}

	// Check for POS_Y
	else if
	(
		convertedPoint0.y == 1.0f
		&&
		convertedPoint1.y == 1.0f
		&&
		convertedPoint2.y == 1.0f
	)
	{
		poly.faceAlignment.setBoundaryIndicator(BoundaryOrientation::POS_Y);
	}

	// Check for NEG_Y
	else if
	(
		convertedPoint0.y == 0.0f
		&&
		convertedPoint1.y == 0.0f
		&&
		convertedPoint2.y == 0.0f
	)
	{
		poly.faceAlignment.setBoundaryIndicator(BoundaryOrientation::NEG_Y);
	}
}

bool OrganicWrappedBBFan::checkForAndEraseAnomalousTriangles()
{
	// We need a bool to see if anys remain after this operation. If there aren't any, we can return false to indicate that the fan shouldn't be added at all.
	bool containsValidFans = true;

	// For a yet-unknown reason, it's actually possible to have 0
	// tertiaries in an instantiated fan. This still needs to be investigated (8/11/2022)
	if (poly.numberOfTertiaries > 0)
	{

		// Step 0: perform first/last point matching check.
		//		I.e, compare point at index 0 to the last point.
		//		The value of the last index should be the number of triangles + 2, minus one.
		//		For example, for 3 triangles, it would be at point index 4 -- (3 + 2) - 1.
		performFirstLastMatchCheck();

		// Step 1: Create all TemporalTriangles, and load them into a map.
		int totalNumberOfInitialTemporals = poly.numberOfTertiaries;
		int currentLeading0Value = 1;
		int currentLeading1Value = 2;
		std::map<int, TemporalTriangle> temporalTriangleMap;
		std::vector<int> temporalRemovalVector;

		for (int x = 0; x < totalNumberOfInitialTemporals; x++)
		{
			ECBPolyPoint sharedPoint = IndependentUtils::convertEnclaveBlockVertexToFloats(vertices[0]);	// the shared point for each TemporalTriangle
																											// should always be the very first point in vertices.
			ECBPolyPoint currentLeadingPoint0 = IndependentUtils::convertEnclaveBlockVertexToFloats(vertices[currentLeading0Value]);
			ECBPolyPoint currentLeadingPoint1 = IndependentUtils::convertEnclaveBlockVertexToFloats(vertices[currentLeading1Value]);



			TemporalTriangle currentTemporal(sharedPoint,
				currentLeadingPoint0,
				currentLeadingPoint1,
				currentLeading0Value,
				currentLeading1Value);
			temporalTriangleMap[temporalTriangleMap.size()] = currentTemporal;

			// make sure to increment the current leading values, so the next triangle points to the appropriate spot.
			currentLeading0Value++;
			currentLeading1Value++;
		}

		// Step 2: For each TemporalTriangle, check if its valid. If it isn't valid, we'll have to:
		//	A. "chop" out the associated vertex value in vertices[] array; meaning, erase that vertex 
		//		and move any vertexes with a higher index than that one down by one.
		//	B. 	after the "chop" is done, all TemporalTriangles need to call downshiftLeadingPointIndices(),
		//		so that when isTemporalValid() is called on their turn, they are looking at updated stats.
		//	C.	If the value of isTemporalValid() is false, we must also decrement the valueof poly.numberOfTertiaries.
		auto temporalTrianglesBegin = temporalTriangleMap.begin();
		auto temporalTrianglesEnd = temporalTriangleMap.end();
		for (; temporalTrianglesBegin != temporalTrianglesEnd; temporalTrianglesBegin++)
		{
			bool isCurrentTemporalValid = temporalTrianglesBegin->second.isTemporalValid();
			if (isCurrentTemporalValid == false)
			{
				// Do the "chop out" in the vertices array; the index to chop out should be from the current chopOutTarget.
				int chopOutTarget = temporalTrianglesBegin->second.fetchTemporalMidPointIndex();
				chopOutPointAndShiftVertices(chopOutTarget);

				// All TemporalTriangles need to have downshiftLeadingPointIndices() called.
				auto temporalTrianglesRedoBegin = temporalTriangleMap.begin();
				auto temporalTrianglesRedoEnd = temporalTriangleMap.end();
				for (; temporalTrianglesRedoBegin != temporalTrianglesRedoEnd; temporalTrianglesRedoBegin++)
				{
					temporalTrianglesRedoBegin->second.downshiftLeadingPointIndices();
				}

				// For debug only: output the current temporal into the removal vector.
				temporalRemovalVector.push_back(temporalTrianglesBegin->first);

				// Finally, decrement the value of poly.numberOfTertiaries by 1.
				poly.numberOfTertiaries--;
			}
		}

		// For Debug: remove "bad" temporals.
		auto removalVectorActionBegin = temporalRemovalVector.begin();
		auto removalVectorActionEnd = temporalRemovalVector.end();
		for (; removalVectorActionBegin != removalVectorActionEnd; removalVectorActionBegin++)
		{
			temporalTriangleMap.erase(*removalVectorActionBegin);
		}

		//std::cout << "(OrganicWrappedBBFan::checkForAndEraseAnomalousTriangles) -> initial number of TemporalTriangles: " << totalNumberOfInitialTemporals << std::endl;
		//std::cout << "(OrganicWrappedBBFan::checkForAndEraseAnomalousTriangles) -> current number of TemporalTriangles: " << temporalTriangleMap.size() << std::endl;

		// If the value of poly.numberOfTertiaries is 0, there are no valid fans to insert.
		if (poly.numberOfTertiaries == 0)
		{
			containsValidFans = false;
		}
	}

	// If there were no fans to begin with at all,
	// then this thing is definitely false.
	else if (poly.numberOfTertiaries == 0)
	{
		containsValidFans = false;
	}
	return containsValidFans;
}

void OrganicWrappedBBFan::performFirstLastMatchCheck()
{
	int lastPointIndex = (poly.numberOfTertiaries + 2) - 1;
	ECBPolyPoint initialPoint = IndependentUtils::convertEnclaveBlockVertexToFloats(vertices[0]);
	ECBPolyPoint initialLastPoint = IndependentUtils::convertEnclaveBlockVertexToFloats(vertices[lastPointIndex]);
	if (initialPoint == initialLastPoint)
	{
		// the points match; decrement the number of tertiaries by one.
		poly.numberOfTertiaries--;
	}
}

void OrganicWrappedBBFan::chopOutPointAndShiftVertices(int in_targetIndexToChop)
{
	// The final index of vertices, would be 7. The number of shifts down would be equal to 7 - the value of in_targetIndexToChop;
	// this would be applied from the end of the vertices array, so work backwards.
	int numberOfShifts = 7 - in_targetIndexToChop;
	int currentReplacementIndex = in_targetIndexToChop;
	int currentIndexThatDoesTheReplacement = in_targetIndexToChop + 1;
	for (int x = 0; x < numberOfShifts; x++)
	{
		vertices[currentReplacementIndex] = vertices[currentIndexThatDoesTheReplacement];
		currentReplacementIndex++;
		currentIndexThatDoesTheReplacement++;
	}
}


int OrganicWrappedBBFan::checkIfRunIsValidForTwoSegments(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)
{
	int isValid = 0;

	// find the index to compare to; for example, if segment at index 0 is for line 1, and index 1 is for line 2, and the value of "in_lineID" is 1, the index this line will compare to is 1 (line 2).
	int comparingIndex = 0;
	int indexOfInputLine = 1;
	if (blockSegmentTracker.lineSegments[0].lineID == in_lineID)
	{
		indexOfInputLine = 0;
		comparingIndex = 1;
	}

	ECBPPOrientationResults endpointOrientation = IndependentUtils::GetPointOrientation(blockSegmentTracker.lineSegments[indexOfInputLine].endVertex, in_blockBorderLineList);
	ECBPPOrientationResults beginpointOrientation = IndependentUtils::GetPointOrientation(blockSegmentTracker.lineSegments[comparingIndex].beginVertex, in_blockBorderLineList);

	BorderMDFaceList endpointFaceList = IndependentUtils::getFaceList(endpointOrientation, in_borderDataMap);
	BorderMDFaceList beginpointFaceList = IndependentUtils::getFaceList(beginpointOrientation, in_borderDataMap);

	int matchResult = IndependentUtils::checkIfFaceListsMatch(endpointFaceList, beginpointFaceList, 1);

	if ((endpointOrientation.osubtype == ECBPPOrientations::FREE) || matchResult == 1)
	{
		isValid = 0;
	}
	else
	{
		isValid = 1;
	}

	return isValid;
}

SegmentResult OrganicWrappedBBFan::checkIfRunIsValidForTwoSegmentsSpecial(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)
{
	int isValid = 0;

	// find the index to compare to; for example, if segment at index 0 is for line 1, and index 1 is for line 2, and the value of "in_lineID" is 1, the index this line will compare to is 1 (line 2).
	int comparingIndex = 0;
	int indexOfInputLine = 1;
	if (blockSegmentTracker.lineSegments[0].lineID == in_lineID)
	{
		indexOfInputLine = 0;
		comparingIndex = 1;
	}

	ECBPPOrientationResults endpointOrientation = IndependentUtils::GetPointOrientation(blockSegmentTracker.lineSegments[indexOfInputLine].endVertex, in_blockBorderLineList);
	ECBPPOrientationResults beginpointOrientation = IndependentUtils::GetPointOrientation(blockSegmentTracker.lineSegments[comparingIndex].beginVertex, in_blockBorderLineList);

	BorderMDFaceList endpointFaceList = IndependentUtils::getFaceList(endpointOrientation, in_borderDataMap);
	BorderMDFaceList beginpointFaceList = IndependentUtils::getFaceList(beginpointOrientation, in_borderDataMap);

	int matchResult = IndependentUtils::checkIfFaceListsMatch(endpointFaceList, beginpointFaceList, 1);

	if ((endpointOrientation.osubtype == ECBPPOrientations::FREE) || matchResult == 1)
	{
		isValid = 0;
	}
	else
	{
		isValid = 1;
	}

	SegmentResult result;
	result.isValid = isValid;
	result.otherSegment = blockSegmentTracker.lineSegments[comparingIndex];
	return result;
}

int OrganicWrappedBBFan::checkIfRunIsValidForTwoSegmentsReverse(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap)
{
	int isValid = 0;

	// find the index to compare to; for example, if segment at index 0 is for line 1, and index 1 is for line 2, and the value of "in_lineID" is 1, the index this line will compare to is 1 (line 2).


	/////////////////

	int comparingIndex = 0;
	int indexOfInputLine = 1;
	if (reverseSegmentTracker.lineSegments[0].lineID == in_lineID)
	{
		indexOfInputLine = 0;
		comparingIndex = 1;
	}


	ECBPPOrientationResults endpointOrientation = IndependentUtils::GetPointOrientation(reverseSegmentTracker.lineSegments[indexOfInputLine].endVertex, in_blockBorderLineList);
	ECBPPOrientationResults beginpointOrientation = IndependentUtils::GetPointOrientation(reverseSegmentTracker.lineSegments[comparingIndex].beginVertex, in_blockBorderLineList);

	//std::cout << "reverse points are (begin): " << reverseSegmentTracker.lineSegments[comparingIndex].beginVertex.x << ", " << reverseSegmentTracker.lineSegments[comparingIndex].beginVertex.y << ", " << reverseSegmentTracker.lineSegments[comparingIndex].beginVertex.z << std::endl;
	//std::cout << "reverse points are (end): " << reverseSegmentTracker.lineSegments[indexOfInputLine].endVertex.x << ", " << reverseSegmentTracker.lineSegments[indexOfInputLine].endVertex.y << ", " << reverseSegmentTracker.lineSegments[indexOfInputLine].endVertex.z << std::endl;

	BorderMDFaceList endpointFaceList = IndependentUtils::getFaceList(endpointOrientation, in_borderDataMap);
	BorderMDFaceList beginpointFaceList = IndependentUtils::getFaceList(beginpointOrientation, in_borderDataMap);

	int matchResult = IndependentUtils::checkIfFaceListsMatch(endpointFaceList, beginpointFaceList, 1);

	if ((endpointOrientation.osubtype == ECBPPOrientations::FREE) || matchResult == 1)
	{
		isValid = 0;
	}
	else
	{
		isValid = 1;
	}

	////////////////
	/*
	if (reverseSegmentTracker.lineSegments[0].lineID == in_lineID)
	{
		std::cout << "!!! Checking against segment 0! " << std::endl;
		std::cout << "[0] begin is: " << reverseSegmentTracker.lineSegments[0].beginVertex.x << ", " << reverseSegmentTracker.lineSegments[0].beginVertex.y << ", " << reverseSegmentTracker.lineSegments[0].beginVertex.z << std::endl;
		std::cout << "[0] end   is: " << reverseSegmentTracker.lineSegments[0].endVertex.x << ", " << reverseSegmentTracker.lineSegments[0].endVertex.y << ", " << reverseSegmentTracker.lineSegments[0].endVertex.z << std::endl;
		ECBPPOrientationResults endpointOrientation = IndependentUtils::GetPointOrientation(reverseSegmentTracker.lineSegments[0].endVertex, in_blockBorderLineList);
		BorderMDFaceList endpointFaceList = IndependentUtils::getFaceList(endpointOrientation, in_borderDataMap);
		//int matchResult = IndependentUtils::checkIfFaceListsMatch(endpointFaceList, beginpointFaceList, 1);
		if ((endpointOrientation.osubtype == ECBPPOrientations::FREE))
		{
			std::cout << "# endpoint IS FREE " << std::endl;
			isValid = 0;
		}
		else
		{
			std::cout << "# endpoint is NOT FREE " << std::endl;
			isValid = 1;
		}
	}

	if (reverseSegmentTracker.lineSegments[1].lineID == in_lineID)
	{
		std::cout << "!!! Checking against segment 1! " << std::endl;
		ECBPPOrientationResults endpointOrientation = IndependentUtils::GetPointOrientation(reverseSegmentTracker.lineSegments[1].endVertex, in_blockBorderLineList);
		BorderMDFaceList endpointFaceList = IndependentUtils::getFaceList(endpointOrientation, in_borderDataMap);
		//int matchResult = IndependentUtils::checkIfFaceListsMatch(endpointFaceList, beginpointFaceList, 1);
		if ((endpointOrientation.osubtype == ECBPPOrientations::FREE))
		{
			isValid = 0;
		}
		else
		{
			isValid = 1;
		}
	}
	*/





	return isValid;


}

int OrganicWrappedBBFan::checkIfRunIsValidForTwoSegmentsViaCopy(int in_lineID, BlockBorderLineList* in_blockBorderLineList, BorderDataMap* in_borderDataMap, PrimarySegmentTracker in_trackerCopy, PolyRunDirection in_direction)
{
	int isValid = 0;

	//std::cout << "Line ID to check: " << in_lineID << std::endl;
	int trackerSize = in_trackerCopy.currentLineSegmentIndex;

	// swap the segment coordinates in this copy, if we are in reverse direction
	if (in_direction == PolyRunDirection::REVERSE)
	{
		in_trackerCopy.swapBeginAndEndInSegments();
	}

	// below is for debug only.
	for (int x = 0; x < trackerSize; x++)
	{
		//std::cout << "Segment " << x << " line ID: " << int(in_trackerCopy.lineSegments[x].lineID) << std::endl;
	}

	// find the index to compare to; for example, if segment at index 0 is for line 1, and index 1 is for line 2, and the value of "in_lineID" is 1, the index this line will compare to is 1 (line 2).
	int comparingIndex = 0;
	int indexOfInputLine = 1;
	if (in_trackerCopy.lineSegments[0].lineID == in_lineID)
	{
		indexOfInputLine = 0;
		comparingIndex = 1;
	}


	//std::cout << "Index to compare the current line to will be: " << comparingIndex << std::endl;
	//std::cout << "*************Segment stats*************************" << std::endl;
	//std::cout << "Segment 0: " << in_trackerCopy.lineSegments[0].beginVertex.x << ", " << in_trackerCopy.lineSegments[0].beginVertex.y << ", " << in_trackerCopy.lineSegments[0].beginVertex.z << " | " << in_trackerCopy.lineSegments[0].endVertex.x << ", " << in_trackerCopy.lineSegments[0].endVertex.y << ", " << in_trackerCopy.lineSegments[0].endVertex.z << std::endl;
	//std::cout << "Segment 1: " << in_trackerCopy.lineSegments[1].beginVertex.x << ", " << in_trackerCopy.lineSegments[1].beginVertex.y << ", " << in_trackerCopy.lineSegments[1].beginVertex.z << " | " << in_trackerCopy.lineSegments[1].endVertex.x << ", " << in_trackerCopy.lineSegments[1].endVertex.y << ", " << in_trackerCopy.lineSegments[1].endVertex.z << std::endl;

	//std::cout << "Endpoint of the segment that was produced by (Line " << in_lineID << "): " << in_trackerCopy.lineSegments[indexOfInputLine].endVertex.x << ", " << in_trackerCopy.lineSegments[indexOfInputLine].endVertex.y << ", " << in_trackerCopy.lineSegments[indexOfInputLine].endVertex.z << std::endl;
	//std::cout << "Begin point of the segment being comparaed to (Line " << int(in_trackerCopy.lineSegments[comparingIndex].lineID) << "): " << in_trackerCopy.lineSegments[comparingIndex].beginVertex.x << ", " << in_trackerCopy.lineSegments[comparingIndex].beginVertex.y << ", " << in_trackerCopy.lineSegments[comparingIndex].beginVertex.z << std::endl;

	ECBPPOrientationResults endpointOrientation = IndependentUtils::GetPointOrientation(in_trackerCopy.lineSegments[indexOfInputLine].endVertex, in_blockBorderLineList);
	ECBPPOrientationResults beginpointOrientation = IndependentUtils::GetPointOrientation(in_trackerCopy.lineSegments[comparingIndex].beginVertex, in_blockBorderLineList);

	BorderMDFaceList endpointFaceList = IndependentUtils::getFaceList(endpointOrientation, in_borderDataMap);
	BorderMDFaceList beginpointFaceList = IndependentUtils::getFaceList(beginpointOrientation, in_borderDataMap);

	int matchResult = IndependentUtils::checkIfFaceListsMatch(endpointFaceList, beginpointFaceList, 1);

	if (endpointOrientation.osubtype == ECBPPOrientations::FREE)
	{
		//std::cout << "point is free, DO NOT RUN! " << std::endl;
	}
	if (matchResult == 1)
	{
		//std::cout << "!!! Warning, points match; cannot run FillTriangleRow! " << std::endl;
	}

	if ((endpointOrientation.osubtype == ECBPPOrientations::FREE) || matchResult == 1)
	{
		isValid = 0;
	}
	else
	{
		isValid = 1;
	}

	return isValid;
}