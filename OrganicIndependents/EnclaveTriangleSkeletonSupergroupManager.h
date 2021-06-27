#pragma once

#ifndef ENCLAVETRIANGLESKELETONSUPERGROUPMANAGER_H
#define ENCLAVETRIANGLESKELETONSUPERGROUPMANAGER_H

#include <map>
#include "EnclaveTriangleSkeletonSupergroup.h"
#include "OperableIntSet.h"

class EnclaveTriangleSkeletonSupergroupManager
{
public:
	std::map<int, EnclaveTriangleSkeletonSupergroup> triangleSkeletonSupergroups;
	void insertSkeletonContainerIntoSupergroup(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleSkeletonContainer in_skeletonContainer)
	{
		triangleSkeletonSupergroups[in_supergroupID].insertSkeletonContainer(in_skeletonContainerID, in_skeletonContainer);
	}
	OperableIntSet appendSkeletonContainers(EnclaveTriangleSkeletonSupergroupManager* in_groupManagerB)
	{
		OperableIntSet appendedSet;
		auto groupManagerBSupergroupsBegin = in_groupManagerB->triangleSkeletonSupergroups.begin();
		auto groupManagerBSupergroupsEnd = in_groupManagerB->triangleSkeletonSupergroups.end();
		for (; groupManagerBSupergroupsBegin != groupManagerBSupergroupsEnd; groupManagerBSupergroupsBegin++)
		{
			triangleSkeletonSupergroups[groupManagerBSupergroupsBegin->first] = groupManagerBSupergroupsBegin->second;
			appendedSet += groupManagerBSupergroupsBegin->first;
		}
		return appendedSet;
	}

	void printMetadata()
	{
		auto groupManagerBSupergroupsBegin = triangleSkeletonSupergroups.begin();
		auto groupManagerBSupergroupsEnd = triangleSkeletonSupergroups.end();
		for (; groupManagerBSupergroupsBegin != groupManagerBSupergroupsEnd; groupManagerBSupergroupsBegin++)
		{
			std::cout << "[Poly ID -> " << groupManagerBSupergroupsBegin->first << "],[Number of EnclaveTriangleSkeletonContainers -> " << groupManagerBSupergroupsBegin->second.skeletonMap.size() << std::endl;
		}
	}
};

#endif
