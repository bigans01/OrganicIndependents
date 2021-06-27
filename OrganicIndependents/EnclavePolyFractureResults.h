#pragma once

#ifndef ENCLAVEPOLYFRACTURERESULTS_H
#define ENCLAVEPOLYFRACTURERESULTS_H

#include "EnclaveTriangleContainer.h"

class EnclavePolyFractureResults
{
public:
	EnclavePolyFractureResults();	// makes compiler happy, default constructor.
	EnclavePolyFractureResults(EnclaveTriangleContainer in_enclaveTriangleContainer);
	EnclaveTriangleContainer resultingTriangles;
};

#endif
