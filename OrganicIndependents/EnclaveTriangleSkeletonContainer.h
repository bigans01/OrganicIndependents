#pragma once

#ifndef ENCLAVETRIANGLESKELETONCONTAINER_H
#define ENCLAVETRIANGLESKELETONCONTAINER_H

#include "EnclaveTriangleSkeleton.h"

class EnclaveTriangleSkeletonContainer
{
public:
	std::map<int, EnclaveTriangleSkeleton> skeletons;
	void insertSkeleton(int in_skeletonContainerID, EnclaveTriangleSkeleton in_skeleton)
	{
		skeletons[in_skeletonContainerID] = in_skeleton;
	};
};

#endif
