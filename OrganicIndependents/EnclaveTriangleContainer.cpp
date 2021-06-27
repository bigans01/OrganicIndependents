#include "stdafx.h"
#include "EnclaveTriangleContainer.h"

void EnclaveTriangleContainer::insertTriangle(EnclaveTriangle in_enclaveTriangle)
{
	triangles[triangleCount++] = in_enclaveTriangle;
}

void EnclaveTriangleContainer::insertTriangleDirect(int in_triangleID, EnclaveTriangle in_enclaveTriangle)
{
	triangles[in_triangleID] = in_enclaveTriangle;
	triangleCount = triangles.size();
}