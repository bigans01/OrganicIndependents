#include "stdafx.h"
#include "MicroPolyPointContainer.h"

void MicroPolyPointContainer::insertNewPoint(ECBPolyPoint in_point)
{
	if (numberOfPoints < 16)
	{
		pointArray[numberOfPoints++] = in_point;
		//std::cout << "!!!! BUFFER WARNING (numberOfPoints is: " << numberOfPoints << ") " << std::endl;
	}
}

void MicroPolyPointContainer::loadPointsFromOtherContainer(MicroPolyPointContainer* in_containerRef)
{
	for (int x = 0; x < in_containerRef->numberOfPoints; x++)
	{
		//if (numberOfPoints <= 6)
		//{
			//std::cout << "!!!:::: current number of Points: " << numberOfPoints << std::endl;
		insertNewPoint(in_containerRef->pointArray[x]);
		//}
	}

}