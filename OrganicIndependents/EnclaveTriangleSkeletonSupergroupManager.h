#pragma once

#ifndef ENCLAVETRIANGLESKELETONSUPERGROUPMANAGER_H
#define ENCLAVETRIANGLESKELETONSUPERGROUPMANAGER_H

#include "EnclaveTriangleSkeletonSupergroup.h"

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
			triangleSkeletonSupergroups[currentGroupCounter] = groupManagerBSupergroupsBegin->second;
			appendedSet += currentGroupCounter;
			currentGroupCounter++;
		}
		return appendedSet;
	}

	bool containsRenderableTriangles()
	{
		bool hasRenderables = false;
		auto superGroupsBegin = triangleSkeletonSupergroups.begin();
		auto superGroupsEnd = triangleSkeletonSupergroups.end();
		for (; superGroupsBegin != superGroupsEnd; superGroupsBegin++)
		{
			if (superGroupsBegin->second.skeletonMap.size() != 0)
			{
				hasRenderables = true;
				break;
			}
		}
		return hasRenderables;
	}

	void resetSupergroups()
	{
		triangleSkeletonSupergroups.clear();
		currentGroupCounter = 0;
	}

	void eraseSupergroup(int in_supergroupID)
	{
		triangleSkeletonSupergroups.erase(in_supergroupID);
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
private:
	int currentGroupCounter = 0;
};

#endif
