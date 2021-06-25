#include "stdafx.h"
#include "PrimarySegmentTracker.h"

void PrimarySegmentTracker::addNewSegment(ECBPolyPoint in_lineSegmentPointA, ECBPolyPoint in_lineSegmentPointB, int in_lineID)
{
	//std::cout << "-----> current index will be: (line ID " << in_lineID << ") :" << (int)currentLineSegmentIndex << std::endl;
	//EnclaveBlockVertex convertedPointA = OrganicUtils::convertPolyPointToBlockVertex(in_lineSegmentPointA);		// convert the points
	//EnclaveBlockVertex convertedPointB = OrganicUtils::convertPolyPointToBlockVertex(in_lineSegmentPointB);
	//if (currentLineSegmentIndex == 2)
	//{
		//std::cout << "!!!! Warning -...> segment index is already 2!!! undefined behavior anticipated!!!" << std::endl;
	//}
	lineSegments[currentLineSegmentIndex].beginVertex = in_lineSegmentPointA;	// store new vertex values
	lineSegments[currentLineSegmentIndex].endVertex = in_lineSegmentPointB;
	lineSegments[currentLineSegmentIndex].lineID = (unsigned char)in_lineID;	// store the lineID
	currentLineSegmentIndex++;		// increment the index
}

void PrimarySegmentTracker::swapBeginAndEndInSegments()
{
	// get a copy of first segment
	SegmentMeta firstCopy = lineSegments[0];
	lineSegments[0].beginVertex = firstCopy.endVertex;
	lineSegments[0].endVertex = firstCopy.beginVertex;

	// get a copy of second segment
	SegmentMeta secondCopy = lineSegments[1];
	lineSegments[1].beginVertex = secondCopy.endVertex;
	lineSegments[1].endVertex = secondCopy.beginVertex;
}

int PrimarySegmentTracker::getIndexOfLineID(int in_lineID)
{
	int returnValue = 100;
	for (int x = 0; x < currentLineSegmentIndex; x++)
	{
		if (lineSegments[x].lineID == in_lineID)
		{
			returnValue = x;
		}
	}
	return returnValue;
}