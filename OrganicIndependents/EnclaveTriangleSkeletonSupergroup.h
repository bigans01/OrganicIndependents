#pragma once

#ifndef ENCLAVETRIANGLESKELETONSUPERGROUP_H
#define ENCLAVETRIANGLESKELETONSUPERGROUP_H
#include "EnclaveTriangleSkeletonContainer.h"

class EnclaveTriangleSkeletonSupergroup
{
public:
	std::map<int, EnclaveTriangleSkeletonContainer> skeletonMap;
	void insertSkeletonContainer(int in_skeletonID, EnclaveTriangleSkeletonContainer in_skeletonContainer)
	{
		skeletonMap[in_skeletonID] = in_skeletonContainer;
	}
};

#endif
