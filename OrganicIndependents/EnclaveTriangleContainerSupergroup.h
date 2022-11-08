#pragma once

#ifndef ENCLAVETRIANGLECONTAINERSUPERGROUP_H
#define ENCLAVETRIANGLECONTAINERSUPERGROUP_H

#include "EnclaveTriangleContainer.h"

class EnclaveTriangleContainerSupergroup
{
public:
	std::map<int, EnclaveTriangleContainer> containerMap;
	void insertEnclaveTriangleContainer(int in_secondaryID, EnclaveTriangleContainer in_organicTriangleSecondary)
	{
		containerMap[in_secondaryID] = in_organicTriangleSecondary;
	}
	void insertTriangleDirectIntoContainer(int in_secondaryID, int in_containerID, EnclaveTriangle in_enclaveTriangle)
	{
		containerMap[in_secondaryID].insertTriangleDirect(in_containerID, in_enclaveTriangle);
	}
};

#endif
