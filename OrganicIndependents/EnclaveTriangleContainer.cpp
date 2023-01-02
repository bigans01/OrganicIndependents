#include "stdafx.h"
#include "EnclaveTriangleContainer.h"


EnclaveTriangleContainer::EnclaveTriangleContainer()
{

}

EnclaveTriangleContainer::EnclaveTriangleContainer(FTriangleContainer in_fTriangleContainer)
{

}


void EnclaveTriangleContainer::insertTriangle(EnclaveTriangle in_enclaveTriangle)
{
	triangles[triangleCount++] = in_enclaveTriangle;
}

void EnclaveTriangleContainer::insertTriangleDirect(int in_triangleID, EnclaveTriangle in_enclaveTriangle)
{
	triangles[in_triangleID] = in_enclaveTriangle;
	triangleCount = int(triangles.size());
}

bool EnclaveTriangleContainer::checkForYSliceBlocks(int in_yCoord)
{
	bool wereSlicesFoundAtY = false;
	auto trianglesBegin = triangles.begin();
	auto trianglesEnd = triangles.end();
	for (; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		if (trianglesBegin->second.doBlocksExistAtY(in_yCoord) == true)
		{
			wereSlicesFoundAtY = true;
		}
	}
	return wereSlicesFoundAtY;
}

void EnclaveTriangleContainer::printTrianglesInContainer()
{
	auto trianglesBegin = triangles.begin();
	auto trianglesEnd = triangles.end();
	for (; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		trianglesBegin->second.printPoints();
	}
}

void EnclaveTriangleContainer::printTouchedBlocksPerTriangle()
{
	auto trianglesBegin = triangles.begin();
	auto trianglesEnd = triangles.end();
	for (; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		std::cout << "Printing blocks for triangle: " << trianglesBegin->first << std::endl;
		trianglesBegin->second.printBlockKeys();
	}
}