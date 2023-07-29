#pragma once

#ifndef ENCLAVEBLOCK_H
#define ENCLAVEBLOCK_H

#include "EnclaveBlockVertex.h"
#include "TertiaryTriangleContainer.h"
#include "PointSearchData.h"
#include "PrimarySegmentTracker.h"
#include "BlockSearchMeta.h"
#include "PolyGroupTicker.h"
#include "BlockFlags.h"
#include "BlockFlagStates.h"
#include "OrganicWrappedBBFan.h"
#include "FanBase.h"
#include "ThinFan.h"
#include "FanManager.h"
#include "BlockSubType.h"
#include "OrganicFanGroup.h"

class EnclaveBlock
{
	public:
		// constructors
		EnclaveBlock();			// default constructor.
		EnclaveBlock(Message in_blockDataMessage);	// for creating a block from a Message that is a BDM_BLOCK_UNTAGGED type.


		// meta data member variables.
		BlockFlags blockflags;						// is block air, is block solid, is block set to render, etc
		FanManager manager;							// stores EnclaveBlockVertexes and FanBases; automatically expands to allocate storage based on needs of the EnclaveBlock.

		int resetBlock();	// resets all underlying components of the EnclaveBlock, and returns the amount of previous triangles that existed.
							// This is needed when needing to deduct the amount of triangles to render in an ORE, after changes have been made.
							// See it's usage in the functions of the OrganicTriangleSecondarySupergroupManager class.

		// functions for interfacing with the underlying FanManager.
		void processTertiaryData(TertiaryTriangleContainer in_polyMetaData, TriangleMaterial in_materialID);	// adds a new EnclaveBlockTriangle to the block; this will eventually need the mutex, EnclaveCollectionMap::enclaveCollectionMapMutex passed to it, so that it may then pass it to the call to checkForOutsourcingLimits (heap allocation safety on the maps)
		void insertBBFanFromRawEnclave(OrganicWrappedBBFan in_wrappedFan);
		int insertFanGroup(OrganicFanGroup in_fanGroupToAdd);
		PointSearchData checkIfPointExists(EnclaveBlockVertex in_blockVertex);						// check if the input parameter exists in the localVertexArray; returns the location of the found point (only really used if it exists)
		PointSearchData checkIfNearbyPointExists(EnclaveBlockVertex in_blockVertex);
		BlockSearchMeta checkForExactPoint(ECBPolyPoint in_point, int in_debugFlag);
		bool checkIfPolysExistOnBoundary(BoundaryOrientation in_boundaryOrientationToCheck);		// checks whether or not any ThinFans or FatFans are marked
																									// as being on particular face (i.e, POS_X)
		BlockSearchMeta checkIfNearbyPointExistsOnLine(ECBPolyPoint in_point, EnclaveKeyDef::EnclaveKey in_key, int in_debugFlag);
		EnclaveBlockVertex fetchPoint(unsigned char in_pointIndex);									// for fetching with unsigned char
		EnclaveBlockVertex fetchPoint(int in_pointIndex);											// for fetching with int	(may be consolidated later)
		int getTotalNumberOfTertiariesInBlock();
		void listSecondaries();
		void listPoints();												// retrieves the number of secondaries, from either LOCAL or OUTSOURCED data
		int getNumberOfBBFans();
		int getNumberOfTotalTriangles();
		FanBase* retrieveSecondaryFromIndex(int in_index);				// retrieves a secondary from a particular index (LOCAL or OUTSOURCED)
		int getNumberOfTertiariesInTriangleAtIndex(int in_index);
		ECBPolyPoint getEmptyNormalFromTriangle(int in_index);

		// utility functions
		ECBPolyPoint convertVertexToPolyPoint(EnclaveBlockVertex in_blockVertex);
		void setBlockMode(BlockSubType in_modeToSet);
		BlockSubType getBlockMode();
		Message writeEnclaveBlockToBDMMessage();	// converts the contents of the block to Message of the type BDM_BLOCK_UNTAGGED. 

	private:
		BlockSubType blockMode = BlockSubType::BLOCK_NORMAL;	// this value is used to determine what should be returned 
																// when functions such as OrganicRawEnclave::getBlockStatus are called;
																// a value of BLOCK_NORMAL indicates that the triangles in this EnclaveBlock
																// may be used in a SPolyShellProducer instance to produce a valid shell that can be analyzed.
																// If the value is BLOCK_INDEPENDENT; it means all the triangles in this block are "customized",
																// meaning there is no 100% reliability that this EnclaveBlock will work with an SPolyShellProducer instance --
																// thus, it needs to be flagged as independent (BLOCK_INDEPENDENT).
};

#endif

