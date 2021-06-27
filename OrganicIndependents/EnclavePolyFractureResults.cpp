#include "stdafx.h"
#include "EnclavePolyFractureResults.h"

EnclavePolyFractureResults::EnclavePolyFractureResults()
{

}

EnclavePolyFractureResults::EnclavePolyFractureResults(EnclaveTriangleContainer in_enclaveTriangleContainer)
{
	resultingTriangles = in_enclaveTriangleContainer;
}