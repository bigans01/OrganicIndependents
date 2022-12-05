#pragma once

#ifndef ORGANICTRIANGLESECONDARYSUPERGROUPMANAGER_H
#define ORGANICTRIANGLESECONDARYSUPERGROUPMANAGER_H

#include "OrganicTriangleSecondarySupergroup.h"
#include "EnclaveBlockSkeleton.h"
#include "EnclaveBlock.h"
#include "PolyUtils.h"

class OrganicTriangleSecondarySupergroupManager
{
public:
	std::map<int, OrganicTriangleSecondarySupergroup> secondarySupergroups;		// the public available map of all the supergroups.
	void insertSecondary(int in_polyID, int in_supergroupID, OrganicTriangleSecondary in_organicTriangleSecondary);	// insert an OrganicTriangleSecondary, with the given poly id, into the given supergroup id.
	void generateBlockTrianglesFromSecondaries(std::map<int, EnclaveBlockSkeleton>* in_skeletonMapRef,	// generates the triangles for EnclaveBlocks, and loads them into the passed in map, 
		std::map<int, EnclaveBlock>* in_enclaveBlockMapRef,												// and also updates the number of total triangles in the ORE.
		int* in_totalTrianglesRef);																		// DO NOT USE this function to produce block copies for arbitrary purposes;
																										// this function should only really be used by
																										// the function OrganicRawEnclave::createBlocksFromOrganicTriangleSecondaries

	void simulateExposedBlockGeneration(std::map<int, EnclaveBlock>* in_enclaveBlockMapRef);	// generates the triangles for EnclaveBlocks, and puts them into the passed in map --
																								// and that's it. Can be used in multiple queries, as it does not affect any component of an ORE directly.
	bool willSecondariesProduceFans();			// check any instances of OrganicWrappedBBFan would be produced by this instance.
	std::set<int> generateTouchedBlockList();	// functionally the same as simulateExposedBlockGeneration, but does not return EnclaveBlock instances -- just their int-equivalent EnclaveKeys.
};

#endif
