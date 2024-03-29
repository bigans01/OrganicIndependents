#pragma once

#ifndef ENCLAVETRIANGLESKELETONSUPERGROUPMANAGER_H
#define ENCLAVETRIANGLESKELETONSUPERGROUPMANAGER_H

#include "EnclaveTriangleSkeletonSupergroup.h"
#include "ORETerrainTriangle.h"
#include "EnclaveTriangleContainer.h"	// OF-1

class EnclaveTriangleSkeletonSupergroupManager
{
	public:
		EnclaveTriangleSkeletonSupergroupManager() {};
		EnclaveTriangleSkeletonSupergroupManager(Message in_buildingMessage);	// This constructor takes in a unopened Message of the type, MessageType::BDM_ORE_SKELETONSGM,
																				// and uses it to produce all the contents of the SGM. The ORE key and blueprint keys
																				// normally associated with this Message should have been stripped away already, before this function is called.
																				// See usage of this constructor in the function, OrganicRawEnclave::reconstituteAsSMatter.
																				

		std::map<int, EnclaveTriangleSkeletonSupergroup> triangleSkeletonSupergroups;

		// OF-2
		//
		// Converts each element of the input EnclaveTriangleContainer into an equivalent EnclaveTriangleSkeleton,
		// before inserting said skeleton into a EnclaveTriangleSkeletonContainer. Functionally the same as insertSkeletonContainerIntoSupergroup,
		// it is required as part of the usage for the OrganicRawEnclave::insertTiledTriangleIntoRTHandler function.
		// This function will probably remain until the end of this class, when this class is deemed as no longer necessary.
		void insertEnclaveTriangleContainerIntoSGM(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleContainer in_skeletonContainer)
		{
			// convert each ET in the container, to an equivalent EnclaveTriangleSkeletonContainer.
			EnclaveTriangleSkeletonContainer convertedContainer;
			for (auto& currentETEntry : in_skeletonContainer.triangles)
			{
				EnclaveTriangleSkeleton convertedSkeleton;
				convertedSkeleton.points[0] = currentETEntry.second.points[0];
				convertedSkeleton.points[1] = currentETEntry.second.points[1];
				convertedSkeleton.points[2] = currentETEntry.second.points[2];
				convertedSkeleton.emptyNormal = currentETEntry.second.emptyNormal;
				convertedSkeleton.materialID = currentETEntry.second.enclaveTriangleMaterialID;
				convertedSkeleton.skeletonBoundaryIndicatorValue = currentETEntry.second.enclaveTriangleBoundaryPolyIndicator;
				convertedSkeleton.isPolyPerfectlyClamped = currentETEntry.second.isEnclaveTrianglePolyPerfectlyClamped;

				convertedContainer.insertSkeleton(currentETEntry.first, convertedSkeleton);
			}

			triangleSkeletonSupergroups[in_supergroupID].insertSkeletonContainer(in_skeletonContainerID, convertedContainer);
		}

		void insertSkeletonContainerIntoSupergroup(int in_supergroupID, int in_skeletonContainerID, EnclaveTriangleSkeletonContainer in_skeletonContainer)
		{
			triangleSkeletonSupergroups[in_supergroupID].insertSkeletonContainer(in_skeletonContainerID, in_skeletonContainer);
		}


		// Below: used by the function OrganicRawEnclave::produceAndReturnTerrainTriangles,
		// which is subsequently called by the OREUtils::produceTerrainTrianglesFromOREEnclaveTriangles 
		// function in OrganicCoreLib. It is meant to abstract-away the triangle creation process,
		// so that functions like the one just mentioned aren't directly accesesing the members in OrganicRawEnclave.
		std::vector<ORETerrainTriangle> produceTerrainTriangles()
		{
			std::vector<ORETerrainTriangle> returnTriangles;
			for (auto& currentSupegroup : triangleSkeletonSupergroups)
			{
				for (auto& currentSkeletonContainer : currentSupegroup.second.skeletonMap)
				{
					for (auto& currentSkeleton : currentSkeletonContainer.second.skeletons)
					{
						ORETerrainTriangle newOTT(currentSkeleton.second.emptyNormal,
												currentSkeleton.second.materialID,
												currentSkeleton.second.points[0],
												currentSkeleton.second.points[1],
												currentSkeleton.second.points[2]);

						returnTriangles.push_back(newOTT);
					}
				}
			}

			return returnTriangles;
		}

		// Below: get the total number of triangles this container would produce; needed by
		// the function OrganicRawEnclave::getNumberOfTrianglesByLOD(), so that it doesn't have to manually
		// cycle through the members of this class.
		int getTriangleCountFromContainers()
		{
			int totalTriangleCount = 0;
			for (auto& currentSupegroup : triangleSkeletonSupergroups)
			{
				for (auto& currentSkeletonContainer : currentSupegroup.second.skeletonMap)
				{
					for (auto& currentSkeleton : currentSkeletonContainer.second.skeletons)
					{
						totalTriangleCount++;
					}
				}

			}
			return totalTriangleCount;
		}

		// Append some skeleton containers from another manager; the IDs of the supergroups from the other manager
		// must be used here. If not done correctly, then the ECBPolyReformer class processing functions,
		// processContouredPolysAgainstPersistentMass and processPersistentPolysAgainstContouredMass will not function correctly,
		// when they call retrieveHandlerTiledTriangles. If those functions can't find a correclating supergroup ID in this manager,
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
