#pragma once

#ifndef ENCLAVETRIANGLESKELETONSUPERGROUPMANAGER_H
#define ENCLAVETRIANGLESKELETONSUPERGROUPMANAGER_H

#include "EnclaveTriangleSkeletonSupergroup.h"

class EnclaveTriangleSkeletonSupergroupManager
{
	public:
		EnclaveTriangleSkeletonSupergroupManager() {};
		EnclaveTriangleSkeletonSupergroupManager(Message in_buildingMessage);	// This constructor takes in a unopened Message of the type, MessageType::BDM_ORE_SKELETONSGM,
																				// and uses it to produce all the contents of the SGM. The ORE key and blueprint keys
																				// normally associated with this Message should have been stripped away already, before this function is called.
																				// See usage of this constructor in the function, OrganicRawEnclave::reconstituteAsSMatter.
																				

		std::map<int, EnclaveTriangleSkeletonSupergroup> triangleSkeletonSupergroups;
		void insertSkeletonContainerIntoSupergroup(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleSkeletonContainer in_skeletonContainer)
		{
			triangleSkeletonSupergroups[in_supergroupID].insertSkeletonContainer(in_skeletonContainerID, in_skeletonContainer);
		}

		// Append some skeleton containers from another manager; the IDs of the supergroups from the other manager
		// must be used here. If not done correctly, then the ECBPolyReformer class processing functions,
		// processContouredPolysAgainstPersistentMass and processPersistentPolysAgainstContouredMass will not function correctly,
		// when they call retriveAllEnclaveTrianglesForSupergroup. If those functions can't find a correclating supergroup ID in this manager,
		// it leads to ECBPolys not being inserted, which leads to unwanted rendering strangeness.
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

		int getTotalEntries()
		{
			int totalEntries = 0;
			auto groupManagerBSupergroupsBegin = triangleSkeletonSupergroups.begin();
			auto groupManagerBSupergroupsEnd = triangleSkeletonSupergroups.end();
			for (; groupManagerBSupergroupsBegin != groupManagerBSupergroupsEnd; groupManagerBSupergroupsBegin++)
			{
				//totalEntries += groupManagerBSupergroupsBegin->second.skeletonMap.size();
				for (auto& currentSkeletonContainer : groupManagerBSupergroupsBegin->second.skeletonMap)
				{
					totalEntries += currentSkeletonContainer.second.skeletons.size();
				}
			}
			return totalEntries;
		}

		Message convertSkeletonSGMToBDM(EnclaveKeyDef::EnclaveKey in_blueprintKey, EnclaveKeyDef::EnclaveKey in_oreKey);	// converts the contents of this instance to a Message of the type BDM_ORE_SKELETONSGM.
};

#endif
