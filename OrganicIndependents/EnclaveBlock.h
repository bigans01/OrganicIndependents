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

class EnclaveBlock
{
public:
	// meta data member variables.
	PolyGroupTicker groupTicker;
	unsigned char BBFanMetaByte = 0;			// secondary poly metadata
	BlockFlags blockflags;						// is block air, is block solid, is block set to render, etc
	unsigned char blockid = 0;					// the type of block (this will be replaced/destroyed later)
	unsigned char usedPoints = 0;				// the number of used points (up to 16)
	unsigned char t1_flags = 0;					// flags for t1 type objects
	FanManager manager;							// stores EnclaveBlockVertexes and FanBases; automatically expands to allocate storage based on needs of the EnclaveBlock.
	PrimarySegmentTracker blockSegmentTracker;  // keeps track of line segments that have entered this block.

	// functions for interfacing with the underlying FanManager.
	void processTertiaryData(TertiaryTriangleContainer in_polyMetaData, TriangleMaterial in_materialID);	// adds a new EnclaveBlockTriangle to the block; this will eventually need the mutex, EnclaveCollectionMap::enclaveCollectionMapMutex passed to it, so that it may then pass it to the call to checkForOutsourcingLimits (heap allocation safety on the maps)
	void insertBBFanFromRawEnclave(OrganicWrappedBBFan in_wrappedFan);
	PointSearchData checkIfPointExists(EnclaveBlockVertex in_blockVertex);						// check if the input parameter exists in the structarray; returns the location of the found point (only really used if it exists)
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
	int getNumberOFLineBitsSet();
	ECBPolyPoint convertVertexToPolyPoint(EnclaveBlockVertex in_blockVertex);
};

#endif

