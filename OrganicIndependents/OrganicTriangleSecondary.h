#pragma once

#ifndef ORGANICTRIANGLESECONDARY_H
#define ORGANICTRIANGLESECONDARY_H

#include "OrganicTriangleTertiary.h"
#include "EnclaveTriangleContainer.h"

class OrganicTriangleSecondary
{
public:
	std::map<int, OrganicTriangleTertiary> enclaveTriangleTertiaryContainer;
	void loadDataFromEnclaveTriangleContainer(EnclaveTriangleContainer* in_containerRef);
};

#endif
