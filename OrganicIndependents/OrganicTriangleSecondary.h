#pragma once

#ifndef ORGANICTRIANGLESECONDARY_H
#define ORGANICTRIANGLESECONDARY_H

#include <map>
#include "OrganicTriangleTertiary.h"
#include "EnclaveTriangleContainer.h"

class OrganicTriangleSecondary
{
public:
	std::map<int, OrganicTriangleTertiary> secondaryMapContainer;
	void loadDataFromEnclaveTriangleContainer(EnclaveTriangleContainer* in_containerRef);
};

#endif
