#pragma once

#ifndef ENCLAVETRIANGLESKELETONSUPERGROUP_H
#define ENCLAVETRIANGLESKELETONSUPERGROUP_H
#include "EnclaveTriangleSkeletonContainer.h"

class EnclaveTriangleSkeletonSupergroup
{
public:
	std::map<int, EnclaveTriangleSkeletonContainer> skeletonMap;
	void insertSkeletonContainer(int in_skeletonContainerID, EnclaveTriangleSkeletonContainer in_skeletonContainer)
	{
		skeletonMap[in_skeletonContainerID] = in_skeletonContainer;
	}
};

#endif
