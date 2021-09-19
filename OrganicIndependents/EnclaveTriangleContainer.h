#pragma once

#ifndef ENCLAVETRIANGLECONTAINER_H
#define ENCALVETRIANGLECONTAINER_H

#include "EnclaveTriangle.h"
#include <map>

class EnclaveTriangleContainer
{
public:
	//EnclaveTriangle triangles[6];	// stores up to 6 triangles.
	std::map<int, EnclaveTriangle> triangles;
	int triangleCount = 0;

	void insertTriangle(EnclaveTriangle in_enclaveTriangle);							// used by EnclavePolyFracturer::EnclavePolyFracturer
	void insertTriangleDirect(int in_triangleID, EnclaveTriangle in_enclaveTriangle);	// used by OrganicRawEnclave::spawnEnclaveTriangleContainers

	bool checkForYSliceBlocks(int in_yCoord);
	void printTrianglesInContainer();
	void printTouchedBlocksPerTriangle();
};

#endif
