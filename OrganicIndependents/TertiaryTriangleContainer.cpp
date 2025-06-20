#include "stdafx.h"
#include "TertiaryTriangleContainer.h"

void TertiaryTriangleContainer::sanitizePoints(int in_debugFlag)
{
	// check if the first two points are the same.
	int duplicateFlag = 0;	// set as 1 if they are the same.
	EnclaveBlockVertex pointA = triangleVertices[0];
	EnclaveBlockVertex pointB = triangleVertices[1];
	if (in_debugFlag == 1)
	{
		std::cout << "pointA: " << int(pointA.getVertexX()) << ", " << int(pointA.getVertexY()) << ", " << int(pointA.getVertexZ()) << std::endl;
		std::cout << "pointB: " << int(pointB.getVertexX()) << ", " << int(pointB.getVertexY()) << ", " << int(pointB.getVertexZ()) << std::endl;
	}


	if
		(
		(pointA.getVertexX() == pointB.getVertexX())
			&&
			(pointA.getVertexY() == pointB.getVertexY())
			&&
			(pointA.getVertexZ() == pointB.getVertexZ())
			)
	{
		duplicateFlag = 1;
		if (in_debugFlag == 1)
		{
			std::cout << "___+++++++++++++>> duplicate point found!!! " << std::endl;
		}
	}
	if (duplicateFlag == 1)
	{
		int numberOfShifts = (numberOfTertiaryTriangles + 1);	// if you had 2 triangles, you'd shift 3 times.
		int replacedElementIndex = 0;	// start at the bottom
		int copiedElementIndex = 1;
		for (int x = 0; x < numberOfShifts; x++)
		{
			triangleVertices[replacedElementIndex] = triangleVertices[copiedElementIndex];
			replacedElementIndex++;
			copiedElementIndex++;
		}
		numberOfTertiaryTriangles--;	// reduce the number of triangles after everything is shifted
	}

	if (in_debugFlag == 1)
	{
		std::cout << "___+++++++++++++>> duplicate point check complete. " << std::endl;
	}
}

void TertiaryTriangleContainer::listPoints()
{
	int loopCount = numberOfTertiaryTriangles + 2;
	for (int x = 0; x < loopCount; x++)
	{
		EnclaveBlockVertex currentPoint = triangleVertices[x];
		std::cout << "Point " << x << ": " << int(currentPoint.getVertexX()) << ", " << int(currentPoint.getVertexY()) << ", " << int(currentPoint.getVertexZ()) << std::endl;
	}
}
