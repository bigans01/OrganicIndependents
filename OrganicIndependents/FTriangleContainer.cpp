#include "stdafx.h"
#include "FTriangleContainer.h"

void FTriangleContainer::insertConstructionLines(std::vector<FTriangleLine> in_constructionLines)
{
	constructionLines = in_constructionLines;
}

void FTriangleContainer::printConstructionLines()
{
	std::cout << "Printing construction lines..." << std::endl;
	for (auto& currentLine : constructionLines)
	{
		currentLine.printLine();
	}
}

void FTriangleContainer::produceFTriangles(FTriangleType in_productionType,
										ECBPolyPoint in_parentEmptyNormal,
										BoundaryOrientation in_parentOrientation,
										EnclaveKeyDef::EnclaveKey in_containerBounds)
{
	// Below: a big triangle that is way too close to a grid's line border; should still run fine.
	
	FTriangleOutput testFTriangle(ECBPolyPoint(31.99f, 16.0f, -0.01f),
		ECBPolyPoint(112.0f, 48.0f, 0.0f),
		ECBPolyPoint(64.0f, 32.0f, 80.0f),
		in_productionType,
		ECBPolyPoint(0, 1.0f, 0.0f),
		BoundaryOrientation::NONE,
		PerfectClampEnum::NONE);

	fracturedTriangles[0] = testFTriangle;
	


	// the number of triangles to produce is always equal to the number of lines - 2.
	int totalTrianglesToProduce = constructionLines.size() - 2;

	std::cout << "Producing " << totalTrianglesToProduce  << " initial FTriangles for key at: ";
	in_containerBounds.printKey();
	std::cout << std::endl;

	// the root point will be point A of the very first line.
	DoublePoint rootPoint = constructionLines.begin()->pointA;

	// the auto iterator for the leading line will be at begin() + 1.
	auto leadingLineIter = constructionLines.begin();
	leadingLineIter++;

	for (int x = 0; x < totalTrianglesToProduce; x++)
	{
		OutputTriangleFrame newFrame(rootPoint, leadingLineIter->pointA, leadingLineIter->pointB);

		std::cout << "Created new frame, with points: ";
		std::cout << "0: " << rootPoint.x << ", " << rootPoint.y << ", " << rootPoint.z << std::endl;
		std::cout << "1: " << leadingLineIter->pointA.x << ", " << leadingLineIter->pointA.y << ", " << leadingLineIter->pointA.z << std::endl;
		std::cout << "2: " << leadingLineIter->pointB.x << ", " << leadingLineIter->pointB.y << ", " << leadingLineIter->pointB.z << std::endl;

		leadingLineIter++;
	}
}
