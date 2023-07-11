#pragma once

#ifndef ENCLAVETRIANGLECONTAINERSUPERGROUPMANAGER_H
#define ENCLAVETRIANGLECONTAINERSUPERGROUPMANAGER_H

#include "EnclaveTriangleContainerSupergroup.h"
#include "EnclaveTriangle.h"
#include "EnclaveTriangleSkeletonSupergroupManager.h"
#include "OrganicTransformUtils.h"

class EnclaveTriangleContainerSupergroupManager
{
public:
	std::map<int, EnclaveTriangleContainerSupergroup> enclaveTriangleSupergroups;
	void insertEnclaveTriangle(int in_polyID, int in_supergroupID, EnclaveTriangleContainer in_enclaveTriangleContainer)
	{
		enclaveTriangleSupergroups[in_polyID].insertEnclaveTriangleContainer(in_supergroupID, in_enclaveTriangleContainer);
	}
	void insertEnclaveTriangleDirectIntoSuperGroup(int in_polyID, int in_supergroupID, int in_containerID, EnclaveTriangle in_enclaveTriangle)
	{
		enclaveTriangleSupergroups[in_polyID].insertTriangleDirectIntoContainer(in_supergroupID, in_containerID, in_enclaveTriangle);
	}
	EnclaveTriangleSkeletonSupergroupManager produceEnclaveTriangleSkeletons()
	{
		EnclaveTriangleSkeletonSupergroupManager producedManager;
		auto supergroupsBegin = enclaveTriangleSupergroups.begin();
		auto supergroupsEnd = enclaveTriangleSupergroups.end();
		for (; supergroupsBegin != supergroupsEnd; supergroupsBegin++)
		{
			auto currentTriangleContainerBegin = supergroupsBegin->second.containerMap.begin();
			auto currentTriangleContainerEnd = supergroupsBegin->second.containerMap.end();
			for (; currentTriangleContainerBegin != currentTriangleContainerEnd; currentTriangleContainerBegin++)
			{
				EnclaveTriangleSkeletonContainer newContainer;
				auto currentTrianglesBegin = currentTriangleContainerBegin->second.triangles.begin();
				auto currentTrianglesEnd = currentTriangleContainerBegin->second.triangles.end();
				for (; currentTrianglesBegin != currentTrianglesEnd; currentTrianglesBegin++)
				{
					EnclaveTriangleSkeleton skeleton = OrganicTransformUtils::deflateEnclaveTriangle(currentTrianglesBegin->second);
					//returnContainerMap[etcSGMBegin->first].insertSkeletonContainer(currentTriangleContainerBegin->first, skeleton);
					newContainer.insertSkeleton(currentTrianglesBegin->first, skeleton);
				}
				producedManager.insertSkeletonContainerIntoSupergroup(supergroupsBegin->first, currentTriangleContainerBegin->first, newContainer);
			}
		}
		return producedManager;
	}
};

#endif
