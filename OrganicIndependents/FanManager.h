#pragma once

#ifndef FANMANAGER_H
#define FANMANAGER_H

#include <mutex>
#include "EnclaveBlockVertex.h"
#include "ThinFan.h"
#include "FatFan.h"
#include "TertiaryTriangleContainer.h"
#include "PointSearchData.h"
#include <iostream>
#include "OrganicWrappedBBFan.h"
#include "BlockSearchMeta.h"

class IndependentUtils;
class FanManager
{
public:
	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	// -------------------------------------------------------------------------------> Overload #1
	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	FanManager& operator=(FanManager&& fanManager_a)
	{
		totalFans = fanManager_a.totalFans;
		currentMode = fanManager_a.currentMode;
		totalPoints = fanManager_a.totalPoints;
		currentPointMode = fanManager_a.currentPointMode;

		for (int x = 0; x < 16; x++)
		{
			triangleArray[x] = fanManager_a.triangleArray[x];
		}

		// Fan Array checks and copies
		isExpandedFanArrayActive = fanManager_a.isExpandedFanArrayActive;
		expandedFanArraySize = fanManager_a.expandedFanArraySize;


		if (isExpandedFanArrayActive == true)
		{
			if (currentMode == FanArrayMode::THIN)
			{
				expandedThinFanArray.reset(new ThinFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedThinFanArray[x] = fanManager_a.expandedThinFanArray[x];
				}
			}
			else if (currentMode == FanArrayMode::FAT)
			{
				expandedFatFanArray.reset(new FatFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedFatFanArray[x] = fanManager_a.expandedFatFanArray[x];
				}
			}
		}

		// EnclaveBlockVertex array checks and copies
		expandedVertexArraySize = fanManager_a.expandedVertexArraySize;
		if (currentPointMode == PointArrayMode::LOCAL_POINTS)
		{
			for (int x = 0; x < totalPoints; x++)		// only need to copy over an amount of points equal to totalPoints
			{
				structarray[x] = fanManager_a.structarray[x];
			}
		}
		else if (currentPointMode == PointArrayMode::NONLOCAL_POINTS)
		{
			expandedVertexArray.reset(new EnclaveBlockVertex[expandedVertexArraySize]);
			for (int x = 0; x < totalPoints; x++)
			{
				expandedVertexArray[x] = fanManager_a.expandedVertexArray[x];		// copy over the contents
			}
		}

		return *this;
	}

	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	// -------------------------------------------------------------------------------> Overload #2 (direct copy, FanManager to FanManager)
	// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
	FanManager& operator=(const FanManager& fanManager_a)
	{
		totalFans = fanManager_a.totalFans;
		currentMode = fanManager_a.currentMode;
		totalPoints = fanManager_a.totalPoints;
		currentPointMode = fanManager_a.currentPointMode;


		for (int x = 0; x < 16; x++)
		{
			triangleArray[x] = fanManager_a.triangleArray[x];
		}

		// Fan Array checks and copies
		isExpandedFanArrayActive = fanManager_a.isExpandedFanArrayActive;
		expandedFanArraySize = fanManager_a.expandedFanArraySize;
		if (isExpandedFanArrayActive == true)
		{
			if (currentMode == FanArrayMode::THIN)
			{
				expandedThinFanArray.reset(new ThinFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedThinFanArray[x] = fanManager_a.expandedThinFanArray[x];
				}
			}
			else if (currentMode == FanArrayMode::FAT)
			{
				expandedFatFanArray.reset(new FatFan[expandedFanArraySize]);
				for (int x = 0; x < expandedFanArraySize; x++)
				{
					expandedFatFanArray[x] = fanManager_a.expandedFatFanArray[x];
				}
			}
		}

		// EnclaveBlockVertex array checks and copies
		expandedVertexArraySize = fanManager_a.expandedVertexArraySize;
		if (currentPointMode == PointArrayMode::LOCAL_POINTS)
		{
			for (int x = 0; x < totalPoints; x++)		// only need to copy over an amount of points equal to totalPoints
			{
				structarray[x] = fanManager_a.structarray[x];
			}
		}
		else if (currentPointMode == PointArrayMode::NONLOCAL_POINTS)
		{
			expandedVertexArray.reset(new EnclaveBlockVertex[expandedVertexArraySize]);
			for (int x = 0; x < totalPoints; x++)
			{
				expandedVertexArray[x] = fanManager_a.expandedVertexArray[x];		// copy over the contents
			}
		}

		return *this;
	}

	void processTertiaryData(TertiaryTriangleContainer in_polyMetaData, short in_materialID);									// used by EnclaveBlock::processTertiaryData
	void insertBBFanFromRawEnclave(OrganicWrappedBBFan in_wrappedFan);															// used by EnclaveBlock::insertBBFanFromRawEnclave
	PointSearchData checkIfPointExists(EnclaveBlockVertex in_blockVertex);														// used by EnclaveBlock::checkIfPointExists
	PointSearchData checkIfNearbyPointExists(EnclaveBlockVertex in_blockVertex);												// used by EnclaveBlock::checkIfNearbyPointExists
	BlockSearchMeta checkForExactPoint(ECBPolyPoint in_point, int in_debugFlag);												// used by EnclaveBlock::checkForExactPoint
	BlockSearchMeta checkIfNearbyPointExistsOnLine(ECBPolyPoint in_point, EnclaveKeyDef::EnclaveKey in_key, int in_debugFlag);	// used by EnclaveBlock::checkIfNearbyPointExistsOnLine

	EnclaveBlockVertex fetchPoint(int in_pointIndex);			// *should* be used by both EnclaveBlock::fetchPoint functions.
	int getTotalNumberOfTertiariesInBlock();					// used by EnclaveBlock::getTotalNumberOfTertiariesInBlock()
	void listSecondaries();										// used by EnclaveBlock::listSecondaries() -- NOT CRITICAL, for debugging.
	void listPoints();											// used by EnclaveBlock::listPoints() -- NOT CRITICAL, for debugging.
	int getNumberOfTotalFans();									// used by EnclaveBlock::getNumberOfBBFans(); returns the number of total fans.
	FanBase* retrieveSecondaryFromIndex(int in_index);			// retrieves a secondary from a particular index; dependent on FanArrayMode to determine where to get the data from.
	int getNumberOfTertiariesInTriangleAtIndex(int in_index);	// used by EnclaveBlock::getNumberOfTertiariesInTriangleAtIndex.
	ECBPolyPoint getEmptyNormalFromTriangle(int in_index);		// used by EnclaveBlock::getEmptyNormalFromTriangle

private:
	enum class FanArrayMode
	{
		LOCALIZED,	// still uses the default array, not the smart point, for coherency.
		THIN,		// uses the expandedFanArray (ThinFan), and up to 256 points in expandedVertexArray; 
		FAT,		// uses the expandedFanArray, and up to "int" points in expandedVertexArray; 
	};
	enum class PointArrayMode
	{
		LOCAL_POINTS,
		NONLOCAL_POINTS
	};

	// for LOCALIZED fan array mode; used by default, unless we need more space.
	ThinFan triangleArray[16];	// stores up to 16 polygons that can be stored within the block = 128 bytes
	EnclaveBlockVertex structarray[32];

	unsigned short totalFans = 0;								// the total number of triangles the manager has; the equivalent of numBBFanByte in EnclaveBlock.

	std::unique_ptr<ThinFan[]> expandedThinFanArray;			// for THIN array mode
	std::unique_ptr<FatFan[]> expandedFatFanArray;				// for FAT array mode
	unsigned short expandedFanArraySize = 16;					// used to represent the size of the arrays for either the ThinFans or FatFans; should start at 16,
	bool isExpandedFanArrayActive = false;
	FanArrayMode currentMode = FanArrayMode::LOCALIZED;				// default is always localized; it's unnecessary to use FAT fans unless we're in FAT mode; LOCALIZED/THIN = THIN fans.

	unsigned short expandedVertexArraySize = 16;
	PointArrayMode currentPointMode = PointArrayMode::LOCAL_POINTS;
	std::unique_ptr<EnclaveBlockVertex[]> expandedVertexArray;	// for when we have > 32 points; this gets expanded by 32 points each.
	unsigned short totalPoints = 0;									// the total number of points the manager is using.

	// |||||||||||||| state checking/manipulation functions.

	FanBase* retrieveNextAvailableSecondary();

	// triangle expansion functions.
	void checkForTriangleExpansion();
	void expandTriangleArray();

	void checkForVertexExpansion();
	int addNewPoint(EnclaveBlockVertex in_blockVertex);

	ECBPolyPoint convertVertexToPolyPoint(EnclaveBlockVertex in_blockVertex);
};

#endif
