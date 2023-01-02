#pragma once

#ifndef ENCLAVETRIANGLECONTAINER_H
#define ENCALVETRIANGLECONTAINER_H

#include "EnclaveTriangle.h"
#include "FTriangleContainer.h"

class EnclaveTriangleContainer
{
	public:
		EnclaveTriangleContainer();
		EnclaveTriangleContainer(FTriangleContainer in_fTriangleContainer);

		std::map<int, EnclaveTriangle> triangles;
		int triangleCount = 0;

		void insertTriangle(EnclaveTriangle in_enclaveTriangle);							// used by EnclavePolyFracturer::EnclavePolyFracturer
		void insertTriangleDirect(int in_triangleID, EnclaveTriangle in_enclaveTriangle);	// used by OrganicRawEnclave::spawnEnclaveTriangleContainers

		bool checkForYSliceBlocks(int in_yCoord);
		void printTrianglesInContainer();
		void printTouchedBlocksPerTriangle();
};

#endif
