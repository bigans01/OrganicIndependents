#include "stdafx.h"
#include "PrimaryLineT1LinkArray.h"

void PrimaryLineT1LinkArray::insertNewLink(PrimaryLineT1 in_link)
{
	PrimaryLineT1Link newLink;				// create a new link
	newLink.primaryLineData = in_link;		// insert primary line data into link
	linkArray[totalNumberOfLinks++] = newLink;	// add the new link, and increment totalNumberOfLinks
}

void PrimaryLineT1LinkArray::setNextLinks()
{
	for (int x = 0; x < totalNumberOfLinks; x++)
	{
		if (x == (totalNumberOfLinks - 1))	// do the following only if we're on the last value
		{
			linkArray[x].indexOfNextLink = 0;	// the next link of the last element in the array will always be 0 (which points to the very first link in the array)
		}
		else
		{
			linkArray[x].indexOfNextLink = (x + 1);		// otherwise, set it to 1 or 2
		}
	}
}

ECBPolyPointPair PrimaryLineT1LinkArray::getCurrentEndAndNextBeginPoints()
{
	ECBPolyPointPair returnPair;
	ECBPolyPoint newPointA = linkArray[currentLinkIndex].primaryLineData.endPointRealXYZ;
	int indexOfNext = linkArray[currentLinkIndex].indexOfNextLink;
	ECBPolyPoint newPointB = linkArray[indexOfNext].primaryLineData.beginPointRealXYZ;
	returnPair.pointA = newPointA;
	returnPair.pointB = newPointB;
	//std::cout << "New pair for line---> " << int(linkArray[currentLinkIndex].primaryLineData.IDofLine) << " is: " << std::endl;
	//std::cout << "Point A: " << newPointA.x << ", " << newPointA.y << ", " << newPointA.z << ", " << std::endl;
	//std::cout << "Point B: " << newPointB.x << ", " << newPointB.y << ", " << newPointB.z << ", " << std::endl;
	return returnPair;
}

void PrimaryLineT1LinkArray::switchToNextLink()
{
	//std::cout << "Switching to next link..." << std::endl;
	currentLinkIndex++;
}