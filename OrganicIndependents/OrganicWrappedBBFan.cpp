#include "stdafx.h"
#include "OrganicWrappedBBFan.h"
#include "IndependentUtils.h"

void OrganicWrappedBBFan::buildBBFan(BlockCircuit* in_blockCircuitRef, short in_materialID, ECBPolyPoint in_emptyNormal)
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
	short in_materialID,
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