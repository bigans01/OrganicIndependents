#pragma once

#ifndef TertiaryTriangleContainer_H
#define TertiaryTriangleContainer_H

#include "EnclaveBlockVertex.h"

class TertiaryTriangleContainer
{
public:
	EnclaveBlockVertex triangleVertices[8];		// store up to 8 vertices
	int numberOfTertiaryTriangles = 0;				// the number of sub triangles
	void sanitizePoints(int in_debugFlag);		// eliminate duplicate points
	void listPoints();
};

#endif
